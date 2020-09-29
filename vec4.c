#include "doom_nukem.h"

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

t_xyzw	vec4_sub(t_xyzw a, t_xyzw b)
{
	return vec4(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w
	);
}

t_xyz	vec4_cross(t_xyzw a, t_xyzw b)
{
	return vec3(
		(a.y * b.z) - (a.z * b.y),
		(a.x * b.z) - (a.z * b.x),
		(a.x * b.y) - (a.y * b.x)
	);
}
