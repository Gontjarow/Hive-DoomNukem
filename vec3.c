#include "doom_nukem.h"

t_xyz	vec3(double x, double y, double z)
{
	return ((t_xyz){x, y, z});
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