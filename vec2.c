#include "doom_nukem.h"

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
