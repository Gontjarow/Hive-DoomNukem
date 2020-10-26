#include "renderer.h"

void	vec4p(t_xyzw v)
{
	printf("\n{%8.3f, %8.3f, %8.3f, %8.3f}\n", v.x, v.y, v.z, v.w);
}

t_xyzw	vec4(double x, double y, double z, double w)
{
	return ((t_xyzw){x, y, z, w});
}

t_xyz	vec43(t_xyzw v)
{
	return vec3(v.x, v.y, v.z);
}

t_xy	vec42(t_xyzw v)
{
	return vec2(v.x, v.y);
}

t_xyzw	vec4_add(t_xyzw a, t_xyzw b)
{
	return vec4(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	);
}

t_xyzw	vec4_sub(t_xyzw a, t_xyzw b)
{
	return vec4(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w
	);
}

t_xyzw	vec4_mul(t_xyzw v, double factor)
{
	return vec4(
		v.x * factor,
		v.y * factor,
		v.z * factor,
		v.w * factor
	);
}

t_xyzw	vec4_div(t_xyzw v, double divisor)
{
	return vec4(
		v.x / divisor,
		v.y / divisor,
		v.z / divisor,
		v.w / divisor
	);
}

double	vec4_mag(t_xyzw v)
{
	return sqrt(vec4_dot(v, v));
}

t_xyzw	vec4_norm(t_xyzw v)
{
	return vec4_div(v, vec4_mag(v));
}

double	vec4_dot(t_xyzw a, t_xyzw b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

t_xyz	vec4_cross(t_xyzw a, t_xyzw b)
{
	return vec3(
		(a.y * b.z) - (a.z * b.y),
		(a.x * b.z) - (a.z * b.x),
		(a.x * b.y) - (a.y * b.x)
	);
}
