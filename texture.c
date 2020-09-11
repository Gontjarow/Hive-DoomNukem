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

Uint32			get_exact_pixel(t_texture *texture, int x, int y)
{
	if (texture->surface->w < x || texture->surface->h < y)
		return (0);
	return (((Uint32 *)texture->surface->pixels)[y * texture->surface->w + x]);
}

/*
** A loading texture we could use with any SDL2 supported image format
*/

SDL_Surface		*load_texture(t_doom *doom, char *path)
{
	t_texture	*texture;
	SDL_Surface	*free_s;

	if (!(texture = malloc(sizeof(t_texture))))
		ft_die("Failed texture allocation\n");
	free_s = IMG_Load(path);
	if (free_s)
	{
		texture->surface = SDL_ConvertSurfaceFormat(free_s,
			doom->buff->format->format, 0);
		SDL_FreeSurface(free_s);
		texture->tex_pixels = texture->surface->w * texture->surface->h;
		return (texture->surface);
	}
	SDL_FreeSurface(free_s);
	free(texture);
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
