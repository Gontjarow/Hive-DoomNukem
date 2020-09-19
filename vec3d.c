/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 22:21:21 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/19 06:05:28 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

/*
** \brief {0.0, 0.0, 0.0}
*/
t_xyz	vec3_zero()
{
	return ((t_xyz){0, 0, 0});
}

/*
** \brief Vector addition
*/
t_xyz	vec3_add(t_xyz a, t_xyz b)
{
	return ((t_xyz){
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
	});
}

/*
** \brief Vector subtraction
*/
t_xyz	vec3_sub(t_xyz a, t_xyz b)
{
	return ((t_xyz){
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
	});
}

/*
** Uniformly scales a vector while maintaining direction.
** \param v vector
** \param scalar multiplies each component
*/
t_xyz	vec3_mul(t_xyz v, double scalar)
{
	return ((t_xyz){
		v.x * scalar,
		v.y * scalar,
		v.z * scalar,
	});
}

/*
** Uniformly scales a vector while maintaining direction.
** \param v vector
** \param scalar divides each component
*/
t_xyz	vec3_div(t_xyz v, double scalar)
{
	return ((t_xyz){
		v.x / scalar,
		v.y / scalar,
		v.z / scalar,
	});
}

/*
** Calculates the magnitude (length) of a vector.
** \param v any vector
*/
double	vec3_mag(t_xyz v)
{
	return (sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));
}

/*
** Normalizes a vector to unit length 1.
** \param v any vector
*/
t_xyz	vec3_norm(t_xyz v)
{
	double mag;

	mag = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	return ((t_xyz){
		v.x / mag,
		v.y / mag,
		v.z / mag
	});
}

/*
** Dot-product
** \return the cosine of the angle between the two vectors
** (1 when parallel,
** 0 when orthogonal,
** -1 when opposite),
** also the length of projecting one vector onto the other
*/
double	vec3_dot(t_xyz a, t_xyz b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

/*
** Cross-product
** \return a vector that is perpendicular to both vectors,
** or zero-vector from parallel (or zero-length) vectors.
** For unit vectors, the magnitude of the product
** is the sine of the angle between the two vectors.
** (1 when perpendicular,
** 0 when parallel,
** -1 when opposite)
*/
t_xyz	vec3_cross(t_xyz a, t_xyz b)
{
	return ((t_xyz){
		(a.y * b.z) - (a.z * b.y),
		(a.x * b.z) - (a.z * b.x),
		(a.x * b.y) - (a.y * b.x)
	});
}

/*
** Calculates the distance between two points.
*/
double	vec3_dist(t_xyz a, t_xyz b)
{
	return (sqrt(
		(b.x - a.x) * (b.x - a.x) +
		(b.y - a.y) * (b.y - a.y) +
		(b.z - a.z) * (b.z - a.z)));
}

/*
** Rotate around Z
** \param angle angle in radians
*/
t_matrix	rotate_z(t_rad angle)
{
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	return ((t_matrix){.m = {
		{ c, s, 0, 0},
		{-s, c, 0, 0},
		{ 0, 0, 1, 0},
		{ 0, 0, 0, 1}
	}});
}

/*
** Rotate around Y
** \param angle angle in radians
*/
t_matrix	rotate_y(t_rad angle)
{
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	return ((t_matrix){.m = {
		{c, 0, -s, 0},
		{0, 1, 0, 0},
		{s, 0, c, 0},
		{0, 0, 0, 1}
	}});
}

/*
** Rotate around X
** \param angle angle in radians
*/
t_matrix	rotate_x(t_rad angle)
{
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	return ((t_matrix){.m = {
		{1,  0, 0, 0},
		{0,  c, s, 0},
		{0, -s, c, 0},
		{0,  0, 0, 1}
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
** Apply Matrix to Vector
** \see https://youtu.be/ih20l3pJoeU?t=1832
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
