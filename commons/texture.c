/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 16:23:13 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/05 18:05:30 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

SDL_Surface			*flip_horizontal(SDL_Surface *surf)
{
	SDL_Surface		*flipped;
	uint32_t		color;
	int				x;
	int				y;

	flipped = SDL_CreateRGBSurface(0, surf->w, surf->h,
		surf->format->BitsPerPixel, surf->format->Rmask, surf->format->Gmask,
		surf->format->Bmask, surf->format->Amask);
	if (flipped == NULL)
		ft_die("Fatal error:"
		"Could not SDL_CreateRGBSurface at flip_horizontal!");
	y = -1;
	while (y++ < surf->h)
	{
		x = -1;
		while (x++ < surf->w)
		{
			color = get_pixel(surf, surf->w - x, y);
			set_pixel(flipped, x, y, color);
		}
	}
	return (flipped);
}

static void			validate_file(char *path)
{
	int				fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		ft_die("The file is invalid or does not exist!\n");
	close(fd);
}

SDL_Surface			*xpm2surface(char *path)
{
	SDL_Surface		*surface;

	surface = IMG_Load(path);
	if (!surface)
		ft_die("Fatal error: xpm2surface failure.");
	return (surface);
}

SDL_Surface			*load_xpm(char *path)
{
	SDL_Surface		*surf;
	SDL_Surface		*conv;

	if (!(IMG_Init(0)))
		ft_die("Fatal error: IMG_Init() failure.");
	surf = IMG_Load(path);
	if (surf)
	{
		if (!(conv = SDL_ConvertSurfaceFormat(surf,
			SDL_PIXELFORMAT_ARGB8888, 0)))
		{
			ft_die("Fatal error:"
			"Could not convert XPM surface into ARGB8888 format.");
		}
		SDL_FreeSurface(surf);
		return (conv);
	}
	ft_putstr(path);
	ft_putstr(" could not be loaded!\n");
	ft_die("Fatal error: Error loading XPM file.");
	return (NULL);
}

/*
** A loading texture we could use with any SDL2 supported image format
*/

SDL_Surface			*load_texture(t_doom *doom, char *path)
{
	SDL_Surface		*free_s;
	SDL_Surface		*ret_s;

	if (!(IMG_Init(IMG_INIT_PNG)))
		ft_die("Fatal error: IMG_INIT_PNG failure.");
	free_s = IMG_Load(path);
	if (free_s)
	{
		ret_s = SDL_ConvertSurfaceFormat(free_s, doom->buff->format->format, 0);
		SDL_FreeSurface(free_s);
		return (ret_s);
	}
	ft_putstr("Warning: Failed to load_texture for \"");
	ft_putstr(path);
	ft_putendl("\" Continuing execution.");
	SDL_FreeSurface(free_s);
	return (NULL);
}
