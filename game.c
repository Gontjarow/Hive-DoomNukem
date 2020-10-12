/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/10/07 09:02:59 by ngontjar         ###   ########.fr       */
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
	flood_window(doom->game->buff, 0xff000000);
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
	doom->game->mdl = NULL;
	doom->game->parent = NULL;
	doom->game->map_path = NULL;
	free(doom->game);
	doom->game = NULL;
}

void 		game_mouse_motion(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

void 		game_mouse_down(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

void		game_key_down(t_doom *doom)
{
	return ;
}

void 		game_loop(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

void		game_render(t_doom *doom)
{
		// These will be the doom->game key handling, right now it only supports the minimap
	// but once the game can be tested with 3D rendering, these will work for both
	if (doom->keystates[SDL_SCANCODE_ESCAPE])
	{
		// Open menu, quit game...
		printf("ESC key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_W])
	{
		// Walk forward
		doom->mdl->player.y += -5;
		doom->mdl->player.tail.y += -5;
		//printf("W key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_S])
	{
		// Walk backward
		doom->mdl->player.y += 5;
		doom->mdl->player.tail.y += 5;
		//printf("S key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_A])
	{
		// Rotate left or walk left (if free camera implemented)
		doom->mdl->player.x += -5;
		doom->mdl->player.tail.x += -5;
		//printf("A key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_D])
	{
		// Rotate right or walk right (if free camera implemented)
		doom->mdl->player.x += 5;
		doom->mdl->player.tail.x += 5;
		//printf("D key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_Q])
	{
		doom->mdl->player.rot_horizontal += 5 * 0.01745329252;
		printf("Q turn left (%.2f)\n", doom->mdl->player.rot_horizontal);
	}
	if (doom->keystates[SDL_SCANCODE_E])
	{
		doom->mdl->player.rot_horizontal += -5 * 0.01745329252;
		printf("E turn right (%.2f)\n", doom->mdl->player.rot_horizontal);
		// Use button, open doors, talk to NPC's...
		// printf("E key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_SPACE])
	{
		// Jump
		printf("Spacebar key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_LSHIFT])
	{
		// Increase player's speed, sprint
		printf("Left Shift key pressed!\n");
	}
	update_minimap(doom);
	render_frame(doom);
	SDL_UpdateWindowSurface(doom->game->win);
}
