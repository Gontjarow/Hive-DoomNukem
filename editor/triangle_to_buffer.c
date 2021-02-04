#include "doom-nukem.h"

void			unpreserving_triangle_to_buffer(SDL_Surface *buff, t_point xy, int dir, uint32_t color)
{
	t_line 		horz_line;
	t_line 		left_line;
	t_line 		right_line;

	horz_line.buff = buff;
	left_line.buff = buff;
	right_line.buff = buff;
	horz_line.color = color;
	left_line.color = color;
	right_line.color = color;

	if (dir == DOWNWARD)
	{
		horz_line.x1 = xy.x - EDT_TRIANGLE_SIZE / 2;
		horz_line.y1 = xy.y - EDT_TRIANGLE_SIZE;
		horz_line.x2 = xy.x + EDT_TRIANGLE_SIZE / 2;
		horz_line.y2 = xy.y - EDT_TRIANGLE_SIZE;
	}
	else if (dir == UPWARD)
	{
		horz_line.x1 = xy.x - EDT_TRIANGLE_SIZE / 2;
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + EDT_TRIANGLE_SIZE / 2;
		horz_line.y2 = xy.y;
	}
	left_line.x1 = horz_line.x1;
	left_line.y1 = horz_line.y1;
	right_line.x1 = horz_line.x2;
	right_line.y1 = horz_line.y2;
	left_line.x2 = xy.x;
	right_line.x2 = xy.x;
	if (dir == DOWNWARD)
	{
		left_line.y2 = xy.y;
		right_line.y2 = xy.y;
	}
	else if (dir == UPWARD)
	{
		left_line.y2 = xy.y - EDT_TRIANGLE_SIZE;
		right_line.y2 = xy.y - EDT_TRIANGLE_SIZE;
	}
	unpreserve_render_line(&horz_line);
	unpreserve_render_line(&left_line);
	unpreserve_render_line(&right_line);
}

void			preserving_triangle_to_buffer(SDL_Surface *buff, t_point xy, int dir, uint32_t color)
{
	t_line 		horz_line;
	t_line 		left_line;
	t_line 		right_line;

	horz_line.buff = buff;
	left_line.buff = buff;
	right_line.buff = buff;
	horz_line.color = color;
	left_line.color = color;
	right_line.color = color;

	if (dir == DOWNWARD)
	{
		horz_line.x1 = xy.x - EDT_TRIANGLE_SIZE / 2;
		horz_line.y1 = xy.y - EDT_TRIANGLE_SIZE;
		horz_line.x2 = xy.x + EDT_TRIANGLE_SIZE / 2;
		horz_line.y2 = xy.y - EDT_TRIANGLE_SIZE;
	}
	else if (dir == UPWARD)
	{
		horz_line.x1 = xy.x - EDT_TRIANGLE_SIZE / 2;
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + EDT_TRIANGLE_SIZE / 2;
		horz_line.y2 = xy.y;
	}
	left_line.x1 = horz_line.x1;
	left_line.y1 = horz_line.y1;
	right_line.x1 = horz_line.x2;
	right_line.y1 = horz_line.y2;
	left_line.x2 = xy.x;
	right_line.x2 = xy.x;
	if (dir == DOWNWARD)
	{
		left_line.y2 = xy.y;
		right_line.y2 = xy.y;
	}
	else if (dir == UPWARD)
	{
		left_line.y2 = xy.y - EDT_TRIANGLE_SIZE;
		right_line.y2 = xy.y - EDT_TRIANGLE_SIZE;
	}
	preserve_render_line(&horz_line);
	preserve_render_line(&left_line);
	preserve_render_line(&right_line);
}
