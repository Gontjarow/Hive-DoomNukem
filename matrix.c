/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 23:16:31 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/20 04:52:18 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

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
	int			i;

	y = 0;
	while (y < 4)
	{
		x = 0;
		while (x < 4)
		{
			matrix.m[y][x] = 0;
			i = 0;
			while (i < 4)
			{
				matrix.m[y][x] += (a.m[y][i] * b.m[i][x]);
				++i;
			}
			++x;
		}
		++y;
	}
	return (matrix);
}
