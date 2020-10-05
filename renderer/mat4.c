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
		{ s, 0, s, 0},
		{ 0, 1, 0, 0},
		{ 0, 0, 1, 0},
		{-s, 0, c, 1},
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
	return ((t_xyzw){
		.x = v.x * m.Xx + v.y * m.Xy + v.z * m.Xz + v.z * m.Xw,
		.y = v.x * m.Yx + v.y * m.Yy + v.z * m.Yz + v.z * m.Yw,
		.z = v.x * m.Zx + v.y * m.Zy + v.z * m.Zz + v.z * m.Zw,
		.w = v.x * m.Tx + v.y * m.Ty + v.z * m.Tz + v.z * m.Tw,
	});
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
