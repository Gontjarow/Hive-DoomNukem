/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/11/01 17:05:24 by msuarez-         ###   ########.fr       */
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

void		render(t_doom *doom);

static double	deg_to_rad(int deg)
{
	return (deg * M_PI / 180);
}

void		game_render(t_doom *doom)
{
	int		x;
	int		y;
	double	rad;
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
		doom->mdl->player.x = doom->mdl->player.tail.x;
		doom->mdl->player.y = doom->mdl->player.tail.y;
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x + 10 * -cos(rad);
		y = doom->mdl->player.y + 10 * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		printf("W key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_S])
	{
		// Walk backward
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x - 10 * -cos(rad);
		y = doom->mdl->player.y - 10 * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		doom->mdl->player.x = doom->mdl->player.tail.x;
		doom->mdl->player.y = doom->mdl->player.tail.y;
		printf("S key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_A])
	{
		// Rotate left
		doom->mdl->player.rot--;
		if (doom->mdl->player.rot < 0)
		{
			doom->mdl->player.rot = 359;
		}
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x + 10 * -cos(rad);
		y = doom->mdl->player.y + 10 * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		printf("Player rotation in degree: %d\n", doom->mdl->player.rot);
		printf("A key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_D])
	{
		// Rotate right
		doom->mdl->player.rot++;
		if (doom->mdl->player.rot >= 360)
		{
			doom->mdl->player.rot = 0;
		}
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x + 10 * -cos(rad);
		y = doom->mdl->player.y + 10 * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		printf("Player rotation in degree: %d\n", doom->mdl->player.rot);
		printf("D key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_E])
	{
		// Use button, open doors, talk to NPC's...
		printf("E key pressed!\n");
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
	render(doom);
	SDL_UpdateWindowSurface(doom->game->win);
}

// !!! NEW CODE, TO BE CLEANED UP

void render(t_doom *doom)
{
	// SDL_memset(doom->game->buff->pixels, 0, GAME_WIN_WIDTH * doom->game->buff->pitch);
	t_mesh test = load_mesh_obj("tiny-donut.obj");

	int i = 0;
	while (i < test.faces)
	{
		t_vert *v = test.face[i++].vert;

		// Face-normal (counter-clockwise vertex order)
		t_xyz normal = vec3_norm(vec4_cross(
			vec4_sub(v[1], v[0]),
			vec4_sub(v[2], v[0])));

		// How much the face aligns with the camera (backface culling)
		// Note: The face must have the opposite direction as the camera to be seen.
		// 📷-->   <-|
		double facing = -vec3_dot(vec3(0,0,-1), normal);
		if (facing > 0)
		{
			// How much the face aligns with the light
			// Note: Normal must face in the OPPOSITE direction as the light-source to be lit.
			// 💡-->   <-|
			double light = -vec3_dot(vec3(0,0,-1), normal);
			if (light > 0)
			{
				// Greyscale brightness; Same value used for R, G, and B
				int color = 255 * light;
				color = color | color << 8 | color << 16;

				// Transformed face (moved and scaled to window size)
				// return;
				double s = 1.2;
				t_face tf = init_face(3,
					vec4((v[0].x + s) * GAME_MIDWIDTH / s, (v[0].y + s) * GAME_MIDHEIGHT / s, 0, 1),
					vec4((v[1].x + s) * GAME_MIDWIDTH / s, (v[1].y + s) * GAME_MIDHEIGHT / s, 0, 1),
					vec4((v[2].x + s) * GAME_MIDWIDTH / s, (v[2].y + s) * GAME_MIDHEIGHT / s, 0, 1));

				draw_tri(doom->game->buff->pixels, tf, color);
				free_verts(&tf);
			}
		}
	}

	free_faces(&test);
}
