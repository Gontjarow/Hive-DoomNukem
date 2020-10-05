#include "renderer.h"

void		swap_xyzw(t_xyzw *a, t_xyzw *b)
{
	t_xyzw c;
	c = *a;
	*a = *b;
	*b = c;
}

// Height-based vertex sorting, required by draw_tri.
void		sort_tri(t_face *face)
{
	if (face->vert[1].y < face->vert[0].y)
		swap_xyzw(&face->vert[0], &face->vert[1]);
	if (face->vert[2].y < face->vert[1].y)
		swap_xyzw(&face->vert[1], &face->vert[2]);
	if (face->vert[1].y < face->vert[0].y)
		swap_xyzw(&face->vert[0], &face->vert[1]);
	assert(face->vert[0].y <= face->vert[1].y && face->vert[1].y <= face->vert[2].y);
}

double		clamp(double n, double min, double max)
{
	assert(min < max);
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	else
		return (n);
}

// Raster-space bounding box
t_xy		bb_min(t_face face)
{
	int i;
	t_vert lowest;
	t_vert current;

	lowest = face.vert[0];
	i = 1;
	while (i < face.verts)
	{
		current = face.vert[i];
		if (current.y < lowest.y) lowest.y = current.y;
		if (current.x < lowest.x) lowest.x = current.x;
		++i;
	}
	return vec2(
		clamp(lowest.x, 0, GAME_WIN_WIDTH),
		clamp(lowest.y, 0, GAME_WIN_HEIGHT));
}

// Raster-space bounding box
t_xy		bb_max(t_face face)
{
	int i;
	t_vert highest;
	t_vert current;

	highest = face.vert[0];
	i = 1;
	while (i < face.verts)
	{
		current = face.vert[i];
		if (current.y > highest.y) highest.y = current.y;
		if (current.x > highest.x) highest.x = current.x;
		++i;
	}
	return vec2(
		clamp(highest.x, 0, GAME_WIN_WIDTH),
		clamp(highest.y, 0, GAME_WIN_HEIGHT));
}

// Note: left < 0, edge == 0, right > 0
double edge(t_xy p, t_xy a, t_xy b)
{
	return ((p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x));
}

// Note: Triangles are assumed to be in CCW order as per Wavefront.
// When this is the case, the INSIDE of the triangle
// is on the left side of each edge. (negative space)
int inside(t_xy p, t_face face)
{
	t_xy v0 = vec42(face.vert[0]);
	t_xy v1 = vec42(face.vert[1]);
	t_xy v2 = vec42(face.vert[2]);

	return (edge(p, v0, v1) <= 0
		&& (edge(p, v1, v2) <= 0)
		&& (edge(p, v2, v0) <= 0));
}

t_xyz	bary(t_xy p, t_face face)
{
	t_xy v0 = vec42(face.vert[0]);
	t_xy v1 = vec42(face.vert[1]);
	t_xy v2 = vec42(face.vert[2]);
	t_xyz weight;

	weight.x = edge(p, v0, v1);
	weight.y = edge(p, v1, v2);
	weight.z = edge(p, v2, v0);
	return (weight);
}
