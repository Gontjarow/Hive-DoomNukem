/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/12/22 19:36:01 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"
#include <stdint.h>

void		init_game(t_doom *doom, int argc, char **argv)
{
	doom->game = (t_game*)malloc(sizeof(t_game));
	if (!doom->game)
		ft_die("Fatal error: Mallocing game struct failed at init_game.");
	doom->game->parent = doom;
	doom->game->win = SDL_CreateWindow("DoomNukem Play Level",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIN_WIDTH,
		GAME_WIN_HEIGHT, 0);
	if (!doom->game->win)
		ft_die("Fatal error: SDL_CreateWindow failed at init_game.");
	doom->game->buff = SDL_GetWindowSurface(doom->game->win);
	flood_buffer(doom->game->buff, 0xff000000);
	if (!doom->game->win)
		ft_die("Fatal error: SDL_GetWindowSurface failed at init_game.");
	doom->game->map_supplied = 0;
	doom->game->map_path = NULL;
	if (argc == 2)
	{
		doom->game->map_path = argv[1];
		doom->game->map_supplied = 1;
	}
	else
		ft_die("Fatal error: No map specified as argument to load Play Level.");
}

void		destroy_game(t_doom *doom)
{
	SDL_FreeSurface(doom->game->buff);
	SDL_DestroyWindow(doom->game->win);
	doom->game->win = NULL;
	doom->game->buff = NULL;
	doom->game->mdl = NULL;
	doom->game->parent = NULL;
	doom->game->map_path = NULL;
	free(doom->game);
	doom->game = NULL;
}

void		game_mouse_motion(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

void		game_mouse_down(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

void		game_key_down(t_doom *doom)
{
	return ;
}

double		deg_to_rad(int deg)
{
	return (deg * M_PI / 180);
}

static void			debug_show_game_polymap(SDL_Surface *polymap, uint32_t *colors)
{
	unsigned int	*dst;
	unsigned int	*pixels;
	unsigned int 	col;
	int				x;
	int				y;

	y = 0;
	dst = (unsigned int*)doom_ptr()->game->buff->pixels;
	pixels = (unsigned int*)polymap->pixels;
	while (y < EDT_WIN_HEIGHT)
	{
		x = 0;
		while (x < EDT_WIN_WIDTH)
		{
			col = pixels[x + (y * GAME_POLYMAP_WIDTH)];
			if (colors != NULL)
				col = colors[(int)col];
			if (col != 0xffffffff)
				dst[x + (y * GAME_WIN_WIDTH)] = col;
			x++;
		}
		y++;
	}
	puts("Drew polymap onto game screen!");
	SDL_UpdateWindowSurface(doom_ptr()->game->win);
	SDL_Delay(1000);
}

void		game_render(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_ESCAPE])
		printf("ESC key pressed!\n");
	printf("Player Health: %d\n", doom->mdl->player.hp.cur);
	handle_player_movement(doom);
	handle_player_action(doom);
	player_update_weapons(doom);
	enemy_update_cooldown(doom);
	rotate_enemy_towards_player(doom);
	move_enemy_towards_player(doom);
	if (DEBUG == 1)
		update_minimap(doom);
	render_frame(doom);
	SDL_UpdateWindowSurface(doom->game->win);
}
