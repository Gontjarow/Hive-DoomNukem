#include "renderer.h"

void	vec2p(t_xy v)
{
	printf("\n{%8.3f, %8.3f}\n", v.x, v.y);
}

t_xy	vec2(double x, double y)
{
	return ((t_xy){x, y});
}

t_xyz	vec23(t_xy v, double z)
{
	return vec3(v.x, v.y, z);
}

t_xy	vec2_add(t_xy a, t_xy b)
{
	return vec2(
		a.x + b.x,
		a.y + b.y
	);
}

t_xy	vec2_sub(t_xy a, t_xy b)
{
	return vec2(
		a.x - b.x,
		a.y - b.y
	);
}

t_xy	vec2_mul(t_xy v, double factor)
{
	return vec2(
		v.x * factor,
		v.y * factor
	);
}

t_xy	vec2_div(t_xy v, double divisor)
{
	return vec2_mul(v, 1 / divisor);
}

double	vec2_cross(t_xy a, t_xy b)
{
	return ((a.x * b.y) - (a.y * b.x));
}

double	vec2_slope(t_xy a, t_xy b)
{
	return ((a.y - b.y) / (a.x - b.x));
}

double	vec2_invslope(t_xy a, t_xy b)
{
	return ((a.x - b.x) / (a.y - b.y));
}

t_xy	vec2_clamp(t_xy v, double min, double max)
{
	assert(min < max);
	v.x = (v.x > min) ? v.x : min;
	v.x = (v.x < max) ? v.x : max;
	v.y = (v.y > min) ? v.y : min;
	v.y = (v.y < max) ? v.y : max;
	return (v);
}

/*
** 2D rotation matrix
** [ sin  | -cos ]
** [ cos  |  sin ]
*/

t_xy	vec2_rot(t_xy v, double cos, double sin)
{
	return ((t_xy){
		v.x * sin - v.y * cos,
		v.x * cos + v.y * sin
	});
}

/*
** wtf
*/

t_xy	vec2_intersect(t_xy v1, t_xy v2, t_xy near, t_xy far)
{
	t_xy cross;
	t_xy wall_length;
	t_xy view_length;

	cross.x = vec2_cross(v1, v2);
	cross.y = vec2_cross(near, far);
	wall_length = vec2_sub(v1, v2);
	view_length = vec2_sub(near, far);
	double determinant = vec2_cross(wall_length, view_length);

	// Are we sure cross.x doesn't need a temp variable instead?
	cross.x = vec2_cross(vec2(cross.x, wall_length.x), vec2(cross.y, view_length.x)) / determinant;
	cross.y = vec2_cross(vec2(cross.x, wall_length.y), vec2(cross.y, view_length.y)) / determinant;

	return (cross);
}