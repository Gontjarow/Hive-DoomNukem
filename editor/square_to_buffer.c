#include "doom-nukem.h"

void	square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	xy.x -= radius / 2;
	xy.y -= radius / 2;
	//printf("square_to_buffering point %d, %d\n", xy.x, xy.y);
	y = 0;
	while (y <= radius)
	{
		x = 0;
		while (x <= radius)
		{
			if (y == 0 || y == radius || x == 0 || x == radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
					pixels[address] = color;
				else
					ft_putendl("Warning: square_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
	}
}

void	cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	t_line 		horz_line;
	t_line 		vert_line;

	horz_line.buff = buff;
	vert_line.buff = buff;
	horz_line.color = color;
	vert_line.color = color;
	horz_line.x1 = xy.x - radius / 2;
	horz_line.y1 = xy.y;
	horz_line.x2 = xy.x + radius / 2;
	horz_line.y2 = xy.y;
	vert_line.x1 = xy.x;
	vert_line.y1 = xy.y - radius / 2;
	vert_line.x2 = xy.x;
	vert_line.y2 = xy.y + radius / 2;
	render_line(&horz_line);
	render_line(&vert_line);
}
