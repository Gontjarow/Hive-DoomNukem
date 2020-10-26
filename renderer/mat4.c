#include "renderer.h"

// Note: column-major order, see header for access macros
// Note: if w == 0, then xyzw is direction
// Note: if w == 1, then xyzw is position
// Note: https://youtu.be/o1n02xKP138?t=251

void		mat4p(t_matrix m)
{
	printf("\n{%8.3f | %8.3f | %8.3f | %8.3f \n"
			" %8.3f | %8.3f | %8.3f | %8.3f \n"
			" %8.3f | %8.3f | %8.3f | %8.3f \n"
			" %8.3f | %8.3f | %8.3f | %8.3f}\n",
			m.Xx, m.Yx, m.Zx, m.Tx,
			m.Xy, m.Yy, m.Zy, m.Ty,
			m.Xz, m.Yz, m.Zz, m.Tz,
			m.Xw, m.Yw, m.Zw, m.Tw);

}

t_matrix	identity_m()
{
	return (t_matrix){{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	}};
}

t_matrix	scale_m(double x, double y, double z)
{
	return (t_matrix){{
		{x, 0, 0, 0},
		{0, y, 0, 0},
		{0, 0, z, 0},
		{0, 0, 0, 1},
	}};
}

/*
** https://en.wikipedia.org/wiki/Transformation_matrix#Other_kinds_of_transformations
*/

t_matrix	translate_m(double x, double y, double z)
{
	return (t_matrix){{
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1},
	}};
}

/*
** Everything is according to right-hand rule.
** https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
*/

t_matrix	rotate_x(t_rad angle)
{
	t_rad c;
	t_rad s;

	c = cos(angle);
	s = sin(angle);
	return (t_matrix){{
		{1, 0, 0, 0},
		{0, c,-s, 0},
		{0, s, c, 0},
		{0, 0, 0, 1},
	}};
}

t_matrix	rotate_y(t_rad angle)
{
	t_rad c;
	t_rad s;

	c = cos(angle);
	s = sin(angle);
	return (t_matrix){{
		{ c, 0, s, 0},
		{ 0, 1, 0, 0},
		{-s, 0, c, 0},
		{ 0, 0, 0, 1},
	}};
}

t_matrix	rotate_z(t_rad angle)
{
	t_rad c;
	t_rad s;

	c = cos(angle);
	s = sin(angle);
	return (t_matrix){{
		{c,-s, 0, 0},
		{s, c, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	}};
}

/*
** Cell = Row * Column
** https://www.mathsisfun.com/algebra/matrix-multiplying.html
*/

t_matrix	multiply_m(t_matrix a, t_matrix b)
{
	t_matrix	matrix;
	int			x;
	int			y;

	y = 0;
	while (y < 4)
	{
		x = 0;
		while (x < 4)
		{
			matrix.m[y][x] =
				a.m[y][0] * b.m[0][x] +
				a.m[y][1] * b.m[1][x] +
				a.m[y][2] * b.m[2][x] +
				a.m[y][3] * b.m[3][x];
			++x;
		}
		++y;
	}
	return (matrix);
}

/*
** Matrix multiplication of 1x4 and 4x4
** Note: Same calculation as multiply_m, but limited to the inner loop.
*/

t_xyzw	apply_m(t_matrix m, t_xyzw v)
{
	return (t_xyzw){
		.x = m.Xx * v.x + m.Yx * v.y + m.Zx * v.z + m.Tx * v.w,
		.y = m.Xy * v.x + m.Yy * v.y + m.Zy * v.z + m.Ty * v.w,
		.z = m.Xz * v.x + m.Yz * v.y + m.Zz * v.z + m.Tz * v.w,
		.w = m.Xw * v.x + m.Yw * v.y + m.Zw * v.z + m.Tw * v.w,
	};
}

/*
** https://youtu.be/o1n02xKP138?t=968
*/

t_matrix project_pure_m()
{
	return (t_matrix){{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 0},
	}};
}

/*
** https://stanford.edu/class/ee267/lectures/lecture2.pdf
*/

t_matrix project_m(t_deg fov, double aspect, double znear, double zfar)
{
	double range;

	range = zfar - znear;
	return (t_matrix){{
		{fov,             0,                       0,                          0},
		{  0,  fov / aspect,                       0,                          0},
		{  0,             0,  (zfar + znear) / range,  -2 * zfar * znear / range},
		{  0,             0,                      -1,                          0},
	}};
}

/*
** https://www.songho.ca/opengl/gl_camera.html#lookat
** https://www.geertarien.com/blog/2017/07/30/breakdown-of-the-lookAt-function-in-OpenGL/
*/
t_matrix	lookat_m(t_xyz eye, t_xyz target, t_xyz up)
{
	t_xyz fwd;
	t_xyz lft;
	t_xyz nup;

	fwd = vec3_norm(vec3_sub(target, eye));
	lft = vec3_norm(vec3_cross(fwd, up));
	nup = vec3_cross(lft, fwd);
	return ((t_matrix){{
		{lft.x, lft.y, lft.z, -lft.x * eye.x - lft.y * eye.y - lft.z * eye.z},
		{nup.x, nup.y, nup.z, -nup.x * eye.x - nup.y * eye.y - nup.z * eye.z},
		{fwd.x, fwd.y, fwd.z, -fwd.x * eye.x - fwd.y * eye.y - fwd.z * eye.z},
		{    0,     0,     0,                                              1},
	}});
}