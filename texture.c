/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 16:23:13 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/11 14:07:19 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		validate_file(char *path)
{
    int     fd;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        ft_die("The file is invalid or does not exist!\n");
    close(fd);
}

/*
** This function makes it possible to get the exact pixel from a x,y coordinate on a surface
*/

Uint32			get_exact_pixel(SDL_Surface *surface, int x, int y)
{
	if (surface->w < x || surface->h < y)
		return (0);
	return (((Uint32 *)surface->pixels)[y * surface->w + x]);
}

/*
** A loading texture we could use with any SDL2 supported image format
*/

SDL_Surface		*load_texture(t_doom *doom, char *path)
{
	SDL_Surface	*free_s;
	SDL_Surface *ret_s;
	int img_flags;

	img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags))
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

/*
** A more simple loading function for PNG format only
*/

SDL_Surface		*load_png(char *path)
{
	SDL_Surface		*pic_surface;

	validate_file(path);
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		ft_putendl(IMG_GetError());
	pic_surface = IMG_Load(path);
	if (!pic_surface)
	{
		ft_putendl("Img_Load:");
		ft_putendl(IMG_GetError());
		return (NULL);
	}
	return (pic_surface);
}
