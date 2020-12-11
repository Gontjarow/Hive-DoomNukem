/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/12/11 19:18:22 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"
#include <stdint.h>

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

double deg_to_rad(int deg)
{
	return (deg * M_PI / 180);
}

void		game_render(t_doom *doom)
{
	// These will be the doom->game key handling, right now it only supports the minimap
	// but once the game can be tested with 3D rendering, these will work for both
	int		x;
	int		y;
	int		location_id;
	double	cur_x;
	double	cur_y;
	double	old_x;
	double	old_y;
	double	rad;

	cur_x = doom->mdl->player.x;
	cur_y = doom->mdl->player.y;
	old_x = cur_x;
	old_y = cur_y;
	if (doom->keystates[SDL_SCANCODE_ESCAPE])
	{
		// Open menu, quit game...
		printf("ESC key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_W])
	{
		// Walk forward
		rad = deg_to_rad(doom->mdl->player.rot);
		doom->mdl->player.x += ((double)doom->mdl->player.mov_speed) * -cos(rad);
 		doom->mdl->player.y += ((double)doom->mdl->player.mov_speed) * -sin(rad);
		x = doom->mdl->player.x + doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y + doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		// printf("W key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_S])
	{
		// Walk backward
		rad = deg_to_rad(doom->mdl->player.rot);
		doom->mdl->player.x -= ((double)doom->mdl->player.mov_speed) * -cos(rad);
 		doom->mdl->player.y -= ((double)doom->mdl->player.mov_speed) * -sin(rad);
		x = doom->mdl->player.x - doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y - doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		// printf("S key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_A])
	{
		// Rotate left
		doom->mdl->player.rot -= doom->mdl->player.rot_speed;
		if (doom->mdl->player.rot < 0)
			doom->mdl->player.rot = 359;
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x + doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y + doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		// printf("A key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_D])
	{
		// Rotate right
		doom->mdl->player.rot += doom->mdl->player.rot_speed;
		if (doom->mdl->player.rot >= 360)
			doom->mdl->player.rot = 0;
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x + doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y + doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		// printf("D key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_E])
	{
		// Use button, open doors, talk to NPC's...
		if (doom->mdl->player.shoot_cd == 0 && doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur > 0 && doom->mdl->player.reload_time == 0)
		{
			player_shoots(doom);
		}
		// printf("E key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_R])
	{
		if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur != doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max)
		{
			doom->mdl->player.reload_time = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].reload_time;
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max;
		}
	}
	if (doom->keystates[SDL_SCANCODE_1])
	{
		doom->mdl->player.weap_id = 0;
		printf("Switched to Weapon 1\n");
	}
	if (doom->keystates[SDL_SCANCODE_2])
	{
		doom->mdl->player.weap_id = 1;
		printf("Switched to Weapon 2\n");
	}
	if (doom->keystates[SDL_SCANCODE_3])
	{
		doom->mdl->player.weap_id = 2;
		printf("Switched to Weapon 3\n");
	}
	if (doom->keystates[SDL_SCANCODE_SPACE])
	{
		// Jump
		if (doom->mdl->player.is_jumping == 0 && doom->mdl->player.is_crouching == 0)
		{
			while (doom->mdl->player.is_jumping == 0)
			{
				doom->mdl->player.height += 10;
				if (doom->mdl->player.height == 200)
					doom->mdl->player.is_jumping = 1;
			}
			while (doom->mdl->player.is_jumping == 1)
			{
				doom->mdl->player.height -= 10;
				if (doom->mdl->player.height == 100)
					doom->mdl->player.is_jumping = 0;
			}
		}
		// printf("Spacebar key pressed!\n");
	}

	/*
	**	Player Run Feature Handling
	*/

	if (doom->keystates[SDL_SCANCODE_LSHIFT] && !doom->mdl->player.run_lock)
	{
		doom->mdl->player.run_lock = 1;
		doom->mdl->player.is_running = 1;
		printf("LSHIFT key pressed!\n");
	}
	if (!doom->keystates[SDL_SCANCODE_LSHIFT] && doom->mdl->player.run_lock)
	{
		doom->mdl->player.is_running = 0;
		doom->mdl->player.run_lock = 0;
		printf("LSHIFT key released!\n");
	}
	if (doom->mdl->player.is_running && doom->mdl->player.mov_speed != doom->mdl->player.max_speed)
		doom->mdl->player.mov_speed++;
	else if (!doom->mdl->player.is_running && doom->mdl->player.mov_speed != 10 && !doom->mdl->player.crouch_lock)
		doom->mdl->player.mov_speed--;

	if (doom->keystates[SDL_SCANCODE_LCTRL] && !doom->mdl->player.crouch_lock)
	{
		doom->mdl->player.crouch_lock = 1;
		doom->mdl->player.is_crouching = 1;
		doom->mdl->player.mov_speed = 5;
		printf("LCTRL key pressed!\n");
	}
	if (!doom->keystates[SDL_SCANCODE_LCTRL] && doom->mdl->player.crouch_lock)
	{
		doom->mdl->player.crouch_lock = 0;
		doom->mdl->player.is_crouching = 0;
		doom->mdl->player.mov_speed = 10;
		printf("LCTRL key released!\n");
	}
	if (doom->mdl->player.is_crouching && doom->mdl->player.height != 50)
	{
		doom->mdl->player.height -= 10;
	}
	else if (!doom->mdl->player.is_crouching && doom->mdl->player.height != 100)
	{
		doom->mdl->player.height += 10;
	}
	/*
	**	Player Collision Detection, checking if it's position is valid according to where he is currently located
	*/
	location_id = check_location(doom, doom->mdl->player.x, doom->mdl->player.y);
	if (location_id == -1 || location_id == UINT_ERROR_CONSTANT || player_collision_with_enemies(doom) == -1)
	{
		doom->mdl->player.x = old_x;
		doom->mdl->player.y = old_y;
	}

	if (doom->mdl->player.shoot_cd > 0)
	{
		doom->mdl->player.shoot_cd--;
	}
	if (doom->mdl->player.reload_time > 0)
	{
		printf("Reloading...\n");
		doom->mdl->player.reload_time--;
	}
	
	if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur > 0 && doom->mdl->player.reload_time == 0)
	{
		printf("Player ammo: %d/%d\n", doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur, doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max);
	}
	else if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur == 0)
	{
		printf("Out of ammo!\n");
	}
	enemy_update_cooldown(doom);
	rotate_enemy_towards_player(doom);
	move_enemy_towards_player(doom);
	update_minimap(doom);
	render_frame(doom);
	SDL_UpdateWindowSurface(doom->game->win);
}
