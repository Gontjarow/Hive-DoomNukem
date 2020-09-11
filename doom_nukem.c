/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/11 14:02:23 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ft_die(const char *error_message)
{
	ft_putendl(error_message);
	exit(0);
}

static void		init_doom(t_doom *doom)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2.");
	doom->win = SDL_CreateWindow("Hive-DoomNukem", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	doom->buff = SDL_GetWindowSurface(doom->win);
	if (doom->win == NULL || doom->buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window or SDL_Surface with SDL_CreateWindow and SDL_GetWindowSurface.");
	doom->quit = 0;
}

int		main(int argc, char **argv)
{
	t_doom			*doom;
	const Uint8* 	keystates;
	unsigned int	*pixels;
	int 			i;
	int				dir_horz;
	int 			dir_vert;

	if (!(doom = (t_doom *)malloc(sizeof(t_doom))))
		return (-1);
	init_doom(doom);
	i = 0;
	dir_horz = 1;
	dir_vert = 1;
	pixels = doom->buff->pixels;
	keystates = SDL_GetKeyboardState(NULL);
	if (argc == 2)
	{
		//doom->buff->pixels = load_png(argv[1]);
		doom->buff->pixels = load_texture(doom, argv[1]);
	}
	while (!doom->quit)
	{
		if (keystates[SDL_SCANCODE_RETURN])
		{
			i += dir_horz;
			if (i >= (WIN_HEIGHT * WIN_WIDTH) || i <= 0)
				dir_horz *= -1;
			else
				pixels[i] = 0xffffffff;
		}
		if (keystates[SDL_SCANCODE_SPACE])
		{
			i += dir_vert * WIN_WIDTH;
			if (i >= (WIN_HEIGHT * WIN_WIDTH) || i <= 0)
				dir_vert *= -1;
			else
				pixels[i] = 0xffffffff;
		}
		while (SDL_PollEvent(&doom->event) != 0)
		{
			if (doom->event.type == SDL_QUIT || doom->event.button.button == SDL_SCANCODE_ESCAPE)
				doom->quit = 1;
		}
		SDL_UpdateWindowSurface(doom->win);
	}
	SDL_DestroyWindow(doom->win);
	SDL_Quit();
	return (0);
}
