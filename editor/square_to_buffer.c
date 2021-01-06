#include "doom-nukem.h"

void			preserving_square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t mask)
{
	unsigned int *pixels;
	unsigned int *com_pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	com_pixels = editor_back_buffer()->buff->pixels;
	xy.x -= radius / 2;
	xy.y -= radius / 2;
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
					{
						if (pixels[address] != mask - 1)
							if (com_pixels[address] == 0xff000000)
								pixels[address] = 0xff000000;
					}
					//else
					//ft_putendl("preserving_square_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
				}
			x++;
		}
		y++;
	}
}

void			unpreserving_square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	xy.x -= radius / 2;
	xy.y -= radius / 2;
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
						if (pixels[address] == 0xff000000)
							pixels[address] = color;
					//else
					//ft_putendl("Warning: unpreserving_square_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
				}
			x++;
		}
		y++;
	}
}

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
				//else
					//ft_putendl("Warning: square_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
	}
}

void 	unpreserving_cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
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
	unpreserve_render_line(&horz_line);
	unpreserve_render_line(&vert_line);
}

void 	preserving_cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
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
	preserve_render_line(&horz_line);
	preserve_render_line(&vert_line);
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
// Function pointerize as parameter

void	digit_to_buffer(SDL_Surface *buff, t_point xy, int digit, uint32_t color)
{
	t_line 		vert_line;
	t_line 		horz_line;
	int 		factor;

	vert_line.buff = buff;
	horz_line.buff = buff;
	vert_line.color = color;
	horz_line.color = color;
	factor = (digit / 10 > 0) ? 2 : 1;
	if (digit / 10 > 0)
		digit /= 10;
	if (digit == 1)
	{
		vert_line.x1 = xy.x;
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x;
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
	}
	else if (digit == 2)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y;
		render_line(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y;
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y;
		horz_line.y2 = xy.y;
		render_line(&horz_line);
	}
	else if (digit == 3)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y;
		render_line(&vert_line);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y;
		horz_line.y2 = xy.y;
		render_line(&horz_line);
	}
	else if (digit == 4)
	{
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_line(&vert_line);
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y;
		render_line(&vert_line);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_line(&horz_line);
	}
	else if (digit == 5)
	{
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_line(&vert_line);
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_line(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
	}
	else if (digit == 6)
	{
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_line(&vert_line);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_line(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
	}
	else if (digit == 7)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
	}
	else if (digit == 8)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_line(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
	}
	else if (digit == 9)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_line(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
	}
	else if (digit == 0 || digit == 10)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_line(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		render_line(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_line(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_line(&horz_line);
	}
	else
		return ;
}

void	digit_to_buffer_ptr(SDL_Surface *buff, t_point xy, int digit, uint32_t color, void (*render_fun)(t_line *line))
{
	t_line 		vert_line;
	t_line 		horz_line;
	int 		factor;

	vert_line.buff = buff;
	horz_line.buff = buff;
	vert_line.color = color;
	horz_line.color = color;
	factor = (digit / 10 > 0) ? 2 : 1;
	if (digit / 10 > 0)
		digit /= 10;
	if (digit == 1)
	{
		vert_line.x1 = xy.x;
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x;
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
	}
	else if (digit == 2)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y;
		render_fun(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y;
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y;
		horz_line.y2 = xy.y;
		render_fun(&horz_line);
	}
	else if (digit == 3)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y;
		render_fun(&vert_line);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y;
		horz_line.y2 = xy.y;
		render_fun(&horz_line);
	}
	else if (digit == 4)
	{
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_fun(&vert_line);
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y;
		render_fun(&vert_line);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_fun(&horz_line);
	}
	else if (digit == 5)
	{
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_fun(&vert_line);
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_fun(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
	}
	else if (digit == 6)
	{
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_fun(&vert_line);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y1 = xy.y;
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_fun(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
	}
	else if (digit == 7)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
	}
	else if (digit == 8)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_fun(&horz_line);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
	}
	else if (digit == 9)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		vert_line.y2 = xy.y;
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.y1 = xy.y;
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y2 = xy.y;
		render_fun(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
	}
	else if (digit == 0 || digit == 10)
	{
		vert_line.x1 = xy.x + (2 * factor);
		vert_line.y1 = xy.y - (2 * factor);
		vert_line.x2 = xy.x + (2 * factor);
		vert_line.y2 = xy.y + (2 * factor);
		render_fun(&vert_line);
		vert_line.x1 = xy.x - (2 * factor);
		vert_line.x2 = xy.x - (2 * factor);
		render_fun(&vert_line);
		horz_line.x1 = xy.x - (2 * factor);
		horz_line.x2 = xy.x + (2 * factor);
		horz_line.y1 = xy.y + (2 * factor);
		horz_line.y2 = xy.y + (2 * factor);
		render_fun(&horz_line);
		horz_line.y1 = xy.y - (2 * factor);
		horz_line.y2 = xy.y - (2 * factor);
		render_fun(&horz_line);
	}
	else
		return ;
}
