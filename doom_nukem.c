/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/08 20:00:01 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ft_die(const char *error_message)
{
	ft_putendl(error_message);
	exit(0);
}

int		main(int argc, char **argv)
{
	t_doom		doom;

	(void)argc;
	(void)argv;
	doom.quit = 0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2.");
	doom.win = SDL_CreateWindow("Hive-DoomNukem", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	doom.ren = SDL_CreateRenderer(doom.win, -1, 1);
	if (doom.win == NULL || doom.ren == NULL)
		ft_die("Fatal error: Failed initialization of SDL2 window with renderer.");
	SDL_RenderClear(doom.ren);
	while (!doom.quit)
	{
		SDL_RenderPresent(doom.ren);
		while (SDL_PollEvent(&doom.event) != 0)
		{
			if (doom.event.type == SDL_QUIT)
				doom.quit = 1;
		}
	}
	SDL_DestroyRenderer(doom.ren);
	SDL_DestroyWindow(doom.win);
	SDL_Quit();
	return (0);
}
