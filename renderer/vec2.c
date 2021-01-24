#include "renderer.h"

void	vec2p(const char *name, t_xy v)
{
	printf("%s{%8.3f, %8.3f}\n", name, v.x, v.y);
}

t_xy	vec2(double x, double y)
{
	return ((t_xy){x, y});
}

t_xyz	vec23(t_xy v, double z)
{
	return (vec3(v.x, v.y, z));
}

t_xy	vec2_add(t_xy a, t_xy b)
{
	return (vec2(a.x + b.x, a.y + b.y));
}

t_xy	vec2_sub(t_xy a, t_xy b)
{
	return (vec2(a.x - b.x, a.y - b.y));
}

t_xy	vec2_mul(t_xy v, double factor)
{
	return (vec2(v.x * factor, v.y * factor));
}

t_xy	vec2_div(t_xy v, double divisor)
{
	return (vec2_mul(v, 1 / divisor));
}

double	vec2_dot(t_xy a, t_xy b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

double	vec2_cross(t_xy a, t_xy b)
{
	return ((a.x * b.y) - (a.y * b.x));
}

double	vec2_mag(t_xy v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

t_xy	vec2_norm(t_xy v)
{
	return (vec2_div(v, vec2_mag(v)));
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

// t_xy	vec2_rot(t_xy v, double cos, double sin)
// {
// 	return ((t_xy){
// 		v.x * sin - v.y * cos,
// 		v.x * cos + v.y * sin
// 	});
// }

t_xy	vec2_rot(t_xy v, double angle)
{
	double c = cos(angle);
	double s = sin(angle);
	return ((t_xy){
		v.x * s - v.y * c,
		v.x * c + v.y * s
	});
}
