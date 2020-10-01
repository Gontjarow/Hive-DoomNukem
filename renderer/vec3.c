#include "renderer.h"

void	vec3p(t_xyz v)
{
	printf("\n{%8.3f, %8.3f, %8.3f}\n", v.x, v.y, v.z);
}

t_xyz	vec3(double x, double y, double z)
{
	return ((t_xyz){x, y, z});
}

t_xy	vec32(t_xyz v)
{
	return vec2(v.x, v.y);
}

t_xyzw	vec34(t_xyz v, double w)
{
	return vec4(v.x, v.y, v.z, w);
}

t_xyz	vec3_add(t_xyz a, t_xyz b)
{
	return vec3(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	);
}

t_xyz	vec3_sub(t_xyz a, t_xyz b)
{
	return vec3(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	);
}

t_xyz	vec3_mul(t_xyz v, double scalar)
{
	return vec3(
		v.x * scalar,
		v.y * scalar,
		v.z * scalar
	);
}

t_xyz	vec3_div(t_xyz v, double divisor)
{
	return vec3_mul(v, 1 / divisor);
}

double	vec3_mag(t_xyz v)
{
	return sqrt(vec3_dot(v, v));
}

t_xyz	vec3_norm(t_xyz v)
{
	return vec3_div(v, vec3_mag(v));
}

double	vec3_dot(t_xyz a, t_xyz b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

t_xyz	vec3_cross(t_xyz a, t_xyz b)
{
	return vec3(
		(a.y * b.z) - (a.z * b.y),
		(a.x * b.z) - (a.z * b.x),
		(a.x * b.y) - (a.y * b.x)
	);
}

double	vec3_dist(t_xyz a, t_xyz b)
{
	return sqrt(
		(b.x - a.x) * (b.x - a.x) +
		(b.y - a.y) * (b.y - a.y) +
		(b.z - a.z) * (b.z - a.z));
}
