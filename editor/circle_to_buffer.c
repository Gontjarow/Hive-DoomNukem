#include "doom-nukem.h"

// TODO	REFACTOR!! I WANT CIRCLES RADIUSES TO SCALE WITH ZOOM FACTOR, CREATE A CIRCLE STRUCT FOR PASSING THE DATA

// TODO Norminettize function sizes, if going to production with these still in use (YAGNI?)

void			preserving_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t mask)
{
	unsigned int *pixels;
	unsigned int *com_pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	com_pixels = editor_back_buffer()->buff->pixels;
	x = -radius;
	y = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
				{
					if (pixels[address] != mask - 1)
						if (com_pixels[address] == 0xff000000)
							pixels[address] = 0xff000000;
				}
				//else
					//ft_putendl("preserving_circle_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
		x = -radius;
	}
}

void			unpreserving_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	x = -radius;
	y = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
				{
					if (pixels[address] == 0xff000000)
						pixels[address] = color;
				}
				//else
					//ft_putendl("unpreserving_circle_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
		x = -radius;
	}
}

void             unmasked_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color, uint32_t mask)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	x = -radius;
	y = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
				{
					if (pixels[address] == mask - 1)
						pixels[address] = color;
					else
						pixels[address] = 0xff000000;
				}
				//else
					//ft_putendl("circle_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
		x = -radius;
	}
}

void             masked_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color, uint32_t avoid)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	x = -radius;
	y = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
				{
					if (pixels[address] == avoid)
						pixels[address] = color - 1;
					else
						pixels[address] = color;
				}
				//else
					//ft_putendl("circle_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
		x = -radius;
	}
}

void             circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	x = -radius;
	y = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
					pixels[address] = color;
				//else
					//ft_putendl("circle_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
		x = -radius;
	}
}