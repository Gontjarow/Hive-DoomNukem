#include "renderer.h"

void		draw(unsigned int *pixel, t_xy start, t_xy end, int color)
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

void	drawline(t_xy_line line, SDL_Surface *surface)
{
	t_xy	length;
	t_xy	ratio;
	t_xy	pos;
	signed	pixels;

	length = vec2(
		fabs(line.stop.x - line.start.x),
		fabs(line.stop.y - line.start.y));
	ratio.x = (line.start.y != line.stop.y) ? (length.x / length.y) : 1;
	ratio.y = (line.start.x != line.stop.x) ? (length.y / length.x) : 1;
	ratio.x = (ratio.x > ratio.y) ? 1 : ratio.x;
	ratio.y = (ratio.y > ratio.x) ? 1 : ratio.y;
	pos = line.start;
	pixels = (length.x > length.y) ? length.x : length.y;
	while (pixels-- > 0)
	{
		set_pixel(surface, pos.x, pos.y, line.color);
		pos.x += ratio.x * ((line.start.x < line.stop.x) ? 1 : -1);
		pos.y += ratio.y * ((line.start.y < line.stop.y) ? 1 : -1);
	}
}

void	draw_box(t_xy center, int radius, int color, SDL_Surface *surface)
{
	center.x = center.x - radius;
	center.y = center.y - radius;

	int size = radius * 2;
	for (int x = 0; x < size; ++x)
	for (int y = 0; y < size; ++y)
		set_pixel(surface, center.x + x, center.y + y, color);
}

double		*get_zbuffer()
{
	static double *zbuffer = NULL;

	if (zbuffer == NULL)
		zbuffer = malloc(GAME_WIN_WIDTH * GAME_WIN_HEIGHT * sizeof(*zbuffer));
	return (zbuffer);
}

int			zbuffer_ok(int index, double depth)
{
	double *zbuffer;

	zbuffer = get_zbuffer();
	if (zbuffer[index] > depth)
	{
		zbuffer[index] = depth;
		return (1);
	}
	// printf("%f  %f\n", zbuffer[index], depth);
	return (0);
}

double		face_depth(t_xyz weight, t_face face)
{
	return (weight.x * face.vert[0].z
		+ weight.y * face.vert[1].z
		+ weight.z * face.vert[2].z);
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
			double depth = face_depth(bary(vec2(x, y), face), face);
			if (zbuffer_ok(x + y * GAME_WIN_WIDTH, depth))
				pixel[x + y * GAME_WIN_WIDTH] = color;
		}
		else
		{
			// pixel[x + y * GAME_WIN_WIDTH] = 0xFF0000;
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
