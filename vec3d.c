/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 22:21:21 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/19 23:16:28 by ngontjar         ###   ########.fr       */
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
