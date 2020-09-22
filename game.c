/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/22 15:57:32 by msuarez-         ###   ########.fr       */
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
	// Random placeholder demo
	set_pixel(doom->game->buff,doom->event.motion.x, doom->event.motion.y, 0xffffffff);
}

void 		game_mouse_down(t_doom *doom)
{
	// Random placeholder demo
	uint32_t  color;

	color = 0xff00ff00;
	set_pixel(doom->game->buff, doom->event.motion.x, doom->event.motion.y, color);
	set_pixel(doom->game->buff,doom->event.motion.x+1, doom->event.motion.y+1, color);
	set_pixel(doom->game->buff,doom->event.motion.x+1, doom->event.motion.y-1, color);
	set_pixel(doom->game->buff,doom->event.motion.x-1, doom->event.motion.y+1, color);
	set_pixel(doom->game->buff,doom->event.motion.x-1, doom->event.motion.y-1, color);
}

// void		game_key_up(t_doom *doom)
// {

// }

void		game_key_down(t_doom *doom)
{
	// These will be the doom->game key handling, right now it only supports the minimap
	// but once the game can be tested with 3D rendering, these will work for both
	if (doom->event.button.button == GAME_KEY_ESC)
	{
		// Open menu, quit game...
		printf("ESC key pressed!\n");
	}
	else if (doom->event.button.button == GAME_KEY_W)
	{
		// Walk forward
		doom->mdl->player.y--;
		doom->mdl->player.tail.y--;
		printf("W key pressed!\n");
	}
	else if (doom->event.button.button == GAME_KEY_S)
	{
		// Walk backward
		doom->mdl->player.y++;
		doom->mdl->player.tail.y++;
		printf("S key pressed!\n");
	}
	else if (doom->event.button.button == GAME_KEY_A)
	{
		// Rotate left or walk left (if free camera implemented)
		doom->mdl->player.x--;
		doom->mdl->player.tail.x--;
		printf("A key pressed!\n");
	}
	else if (doom->event.button.button == GAME_KEY_D)
	{
		// Rotate right or walk right (if free camera implemented)
		doom->mdl->player.x++;
		doom->mdl->player.tail.x++;
		printf("D key pressed!\n");
	}
	else if (doom->event.button.button == GAME_KEY_E_ACTION)
	{
		// Use button, open doors, talk to NPC's...
		printf("E key pressed!\n");
	}
	else if (doom->event.button.button == GAME_KEY_SPACEBAR)
	{
		// Jump
		printf("Spacebar key pressed!\n");
	}
	else if (doom->event.button.button == GAME_KEY_LSHIFT)
	{
		// Increase player's speed, sprint
		printf("Left Shift key pressed!\n");
	}
	update_minimap(doom);
}

void 		game_loop(t_doom *doom)
{
	// Random placeholder demo
	static int cooldown = 0;
	if (!cooldown)
	{
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
