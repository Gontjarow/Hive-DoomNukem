/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 23:16:31 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/20 08:07:08 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

/*
** Identity matrix
** \return a matrix that causes no changes
*/
t_matrix	identity()
{
	return ((t_matrix){.m = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}});
}

/*
** Rotate around Z
** \see https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
** \param angle angle in radians
*/
t_matrix	rotate_z(t_rad angle)
{
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	return ((t_matrix){.m = {
		{ c, -s, 0, 0},
		{ s,  c, 0, 0},
		{ 0,  0, 1, 0},
		{ 0,  0, 0, 1}
	}});
}

/*
** Rotate around Y
** \see https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
** \param angle angle in radians
*/
t_matrix	rotate_y(t_rad angle)
{
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	return ((t_matrix){.m = {
		{ c, 0, s, 0},
		{ 0, 1, 0, 0},
		{-s, 0, c, 0},
		{ 0, 0, 0, 1}
	}});
}

/*
** Rotate around X
** \see https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
** \param angle angle in radians
*/
t_matrix	rotate_x(t_rad angle)
{
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	return ((t_matrix){.m = {
		{1,  0,  0, 0},
		{0,  c, -s, 0},
		{0,  s,  c, 0},
		{0,  0,  0, 1}
	}});
}

/*
** Translation matrix
*/
t_matrix	translation(double x, double y, double z)
{
	return ((t_matrix){.m = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{x, y, z, 1},
	}});
}

/*
** Scaling matrix
*/
t_matrix	scale(double x, double y, double z)
{
	return ((t_matrix){.m = {
		{x, 0, 0, 0},
		{0, y, 0, 0},
		{0, 0, z, 0},
		{0, 0, 0, 1},
	}});
}

/*
** Projection Matrix
** \see https://youtu.be/ih20l3pJoeU?t=1675
** \param near near plane
** \param far far plane
** \param fov FOV in degrees
*/
t_matrix	perspective(t_deg fov, double near, double far)
{
	double	ar = (double)GAME_WIN_HEIGHT / (double)GAME_WIN_WIDTH;
	t_rad	rad = 1.0 / tan(fov * 0.5 * DEG_TO_RAD);

	return ((t_matrix){.m = {
		{ar * rad,    0,                            0,  0},
		{       0,  rad,                            0,  0},
		{       0,    0,           far / (far - near),  1},
		{       0,    0, (-far * near) / (far - near),  0}
	}});
}

/*
** \brief Apply Matrix to Vector
** \see https://youtu.be/ih20l3pJoeU?t=1832
** Implies the fourth element if V is 1.
** \param v any vector
** \param m any matrix
*/
t_xyz	vec3_transform(t_xyz v, t_matrix m)
{
	t_xyz	out;
	double	w;

	out = (t_xyz){
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]
	};
	w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	if (w != 0.0)
		vec3_div(out, w);
	return (out);
}

/*
** Matrix Multiplication
** \see https://www.mathsisfun.com/algebra/matrix-multiplying.html
** \param a any matrix
** \param b any matrix
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
** Point-at Matrix
** \see https://youtu.be/HXSuNxpCzdM?t=1122
*/
t_matrix	point_at(t_xyz pos, t_xyz target, t_xyz up)
{
	t_xyz fwd;
	t_xyz adjust;
	t_xyz newup;
	t_xyz right;

	fwd = vec3_norm(vec3_sub(target, pos));
	adjust = vec3_mul(fwd, vec3_dot(up, fwd));
	newup = vec3_norm(vec3_sub(up, adjust));
	right = vec3_cross(newup, fwd);
	return ((t_matrix){.m = {
		{right.x, right.y, right.z, 0},
		{newup.x, newup.y, newup.z, 0},
		{  fwd.x,   fwd.y,   fwd.z, 0},
		{  pos.x,   pos.y,   pos.z, 1}
	}});
}

/*
** Quick Inverse-Matrix
** Used specifically to create a look-at matrix from point-at.
** \see https://youtu.be/HXSuNxpCzdM?t=1466
*/
t_matrix	inverse_m(t_matrix m)
{
	t_matrix out;

	out = (t_matrix){.m = {
		{m.m[0][0], m.m[1][0], m.m[2][0], 0},
		{m.m[0][1], m.m[1][1], m.m[2][1], 0},
		{m.m[0][2], m.m[1][2], m.m[2][2], 0},
		{0, 0, 0, 1}
	}};
	out.m[3][0] = -(m.m[3][0] * out.m[0][0] +
					m.m[3][1] * out.m[1][0] +
					m.m[3][2] * out.m[2][0]);
	out.m[3][1] = -(m.m[3][0] * out.m[0][1] +
					m.m[3][1] * out.m[1][1] +
					m.m[3][2] * out.m[2][1]);
	out.m[3][2] = -(m.m[3][0] * out.m[0][2] +
					m.m[3][1] * out.m[1][2] +
					m.m[3][2] * out.m[2][2]);
	return (out);
}
