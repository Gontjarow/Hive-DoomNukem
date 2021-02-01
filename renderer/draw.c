#include "renderer.h"

void		drawline(t_xy_line line, SDL_Surface *surface)
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

void		draw_box(t_xy center, int radius, int color, SDL_Surface *surface)
{
	center.x = center.x - radius;
	center.y = center.y - radius;

	int size = radius * 2;
	for (int x = 0; x < size; ++x)
	for (int y = 0; y < size; ++y)
		set_pixel(surface, center.x + x, center.y + y, color);
}

void		vertical_line(int column, int start, int end, int color)
{
	int *pixel;
	int draw;

	pixel = (int*)doom_ptr()->game->buff->pixels;
	start = clamp(start, 0, GAME_WIN_HEIGHT - 1);
	end = clamp(end, 0, GAME_WIN_HEIGHT - 1);

	// Always draw at least one pixel.
	draw = 1 + (end - start);
	while (draw-- > 0)
	{
		pixel[GAME_WIN_WIDTH * (start + draw) + column] = color;
	}
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
