#include "renderer.h"

void draw(unsigned int *pixel, t_xy start, t_xy end, int color)
{
	t_xy	dir;
	t_xy	abs;
	float	step;
	int		i;
	int		index;

	dir = vec2_sub(end, start);
	abs = vec2(fabs(dir.x), fabs(dir.y));
	step = (abs.x > abs.y) ? abs.x : abs.y;
	dir = vec2_div(dir, step);
	i = 0;
	while (i <= step)
	{
		index = (int)start.x + (int)start.y * GAME_WIN_WIDTH;
		if ((int)start.x >= 0 && (int)start.x < GAME_WIN_WIDTH
		&& ((int)start.y >= 0 && (int)start.y < GAME_WIN_HEIGHT)
		&& (index >= 0 && index < GAME_WIN_WIDTH * GAME_WIN_HEIGHT))
			pixel[index] = color;
		start.x += dir.x;
		start.y += dir.y;
		++i;
	}
}

// Fixed, exactly 3-vert triangle.
// Note: wavefront.obj triangles have verts in counter-clockwise order.
void		draw_tri(unsigned int *pixel, t_face face, int color)
{
	t_xy min = bb_min(face);
	t_xy max = bb_max(face);

	for (int y = min.y; y < max.y; ++y)
	for (int x = min.x; x < max.x; ++x)
	{
		if (inside(vec2(x, y), face))
		{
			pixel[x + y * GAME_WIN_WIDTH] = color;
		}
	}
}

void		draw_tri_color(unsigned int *pixel, t_face face)
{
	t_xy v0 = vec42(face.vert[0]);
	t_xy v1 = vec42(face.vert[1]);
	t_xy v2 = vec42(face.vert[2]);
	t_xy min = bb_min(face);
	t_xy max = bb_max(face);

	for (int y = min.y; y < max.y; ++y)
	for (int x = min.x; x < max.x; ++x)
	{
		double area2 = edge(v0, v1, v2);
		t_xyz weight = bary(vec2(x, y), face);
		if (weight.x <= 0 && weight.y <= 0 && weight.z <= 0)
		{
			weight = vec3_div(weight, area2);
			int r = 255 * weight.x;
			int g = 255 * weight.y;
			int b = 255 * weight.z;
			pixel[x + y * GAME_WIN_WIDTH] = (r << 16 | g << 8 | b);
		}
	}
}
