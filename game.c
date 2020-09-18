/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/18 21:20:23 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void 		init_game(t_doom *doom, int argc, char **argv)
{
	doom->game = (t_game*)malloc(sizeof(t_game));
	if (!doom->game)
		ft_die("Fatal error: Mallocing game struct failed at init_game.");
	doom->game->parent = doom;
	doom->game->win = SDL_CreateWindow("DoomNukem Play Level", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, GAME_WIN_WIDTH, GAME_WIN_HEIGHT, 0);
	if (!doom->game->win)
		ft_die("Fatal error: SDL_CreateWindow failed at init_game.");
	doom->game->buff = SDL_GetWindowSurface(doom->game->win);
	if (!doom->game->buff)
		ft_die("Fatal error: SDL_GetWindowSurface failed at init_game.");
	if (argc == 2)
		doom->game->map_path = argv[1];
	else
		ft_die("Fatal error: No map specified as argument to load with Play Level.");
}

void 		destroy_game(t_doom *doom)
{
	SDL_FreeSurface(doom->game->buff);
	SDL_DestroyWindow(doom->game->win);
	doom->game->win = NULL;
	doom->game->buff = NULL;
	free(doom->game);
	doom->game = NULL;
}

void 		game_mouse_motion(t_doom *doom)
{
	set_pixel(doom->game->buff,
		GAME_MID_X + doom->event.motion.xrel,
		GAME_MID_Y + doom->event.motion.yrel, 0xffffffff);
}

void 		game_mouse_down(t_doom *doom)
{
	uint32_t  color;

	color = 0xff00ff00;
	set_pixel(doom->game->buff, doom->event.motion.x, doom->event.motion.y, color);
	set_pixel(doom->game->buff,doom->event.motion.x+1, doom->event.motion.y+1, color);
	set_pixel(doom->game->buff,doom->event.motion.x+1, doom->event.motion.y-1, color);
	set_pixel(doom->game->buff,doom->event.motion.x-1, doom->event.motion.y+1, color);
	set_pixel(doom->game->buff,doom->event.motion.x-1, doom->event.motion.y-1, color);
}

void 		game_loop(t_doom *doom)
{
	// Random placeholder demo
	static int cooldown = 0;
	if (!cooldown)
	{
		ft_putendl("Game looping!");
		cooldown = 100;
	} else
		cooldown--;
	set_pixel(doom->game->buff,cooldown+1, cooldown+1, 0xffffffff);
	set_pixel(doom->game->buff,cooldown, cooldown, 0xff000000);
}

void		game_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->game->win);
}
