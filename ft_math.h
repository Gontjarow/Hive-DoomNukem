/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 23:13:19 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/19 23:13:24 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MATH_H
# define FT_MATH_H

# include "doom_nukem.h"

# define RAD_TO_DEG 57.29577951308232087679815481410517033
# define DEG_TO_RAD 0.01745329251994329576923690768488612

typedef struct	s_xyz
{
	double	x;
	double	y;
	double	z;
}				t_xyz;

typedef t_xyz	t_euler; // Euler angles (X, Y, Z)
typedef double	t_deg; // Degrees
typedef double	t_rad; // Radians

typedef struct	s_matrix
{
	double	m[4][4];
}				t_matrix;

// Vector 3 stuff

t_xyz			vec3_zero();
t_xyz			vec3_add(t_xyz a, t_xyz b);
t_xyz			vec3_sub(t_xyz a, t_xyz b);
t_xyz			vec3_mul(t_xyz v, double scalar);
t_xyz			vec3_div(t_xyz v, double scalar);
double			vec3_mag(t_xyz v);
t_xyz			vec3_norm(t_xyz v);
double			vec3_dot(t_xyz a, t_xyz b);
t_xyz			vec3_cross(t_xyz a, t_xyz b);
double			vec3_dist(t_xyz a, t_xyz b);

// Matrix stuff

t_matrix		rotate_z(t_rad angle);
t_matrix		rotate_y(t_rad angle);
t_matrix		rotate_x(t_rad angle);
t_matrix		translation(double x, double y, double z);
t_matrix		scale(double x, double y, double z);
t_matrix		perspective(t_deg fov, double near, double far);
t_xyz			vec3_transform(t_xyz v, t_matrix m);
t_matrix		multiply_m(t_matrix a, t_matrix b);

#endif
