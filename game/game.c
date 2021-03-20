/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/03/20 20:59:34 by msuarez-         ###   ########.fr       */
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
	doom->game->hud_location = NULL;
	doom->game->cel_shade_hud = 0;
	doom->game->show_info = 0;
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
	static double yaw = 0.0;
	int x;
	int y;
	int delta;

	SDL_GetRelativeMouseState(&x,&y);
	yaw   = clamp(yaw - y*0.01f, -(M_PI / 8), (M_PI / 8));
	doom->mdl->player.yaw = yaw;
	delta = abs(x);
	if (x > 0)
		get_model()->player.rot += delta;
	if (x < 0)
		get_model()->player.rot -= delta;
	if (doom_ptr()->mdl->player.rot < 0)
		doom_ptr()->mdl->player.rot = 359 + (doom_ptr()->mdl->player.rot);
	if (doom_ptr()->mdl->player.rot >= 360)
		doom_ptr()->mdl->player.rot = 0 + (doom_ptr()->mdl->player.rot) - 360;
	update_player_tail(doom_ptr(), deg_to_rad(doom_ptr()->mdl->player.rot));
}

void		game_mouse_updown(t_doom *doom)
{
	if (doom->event.type == SDL_MOUSEBUTTONUP && doom->event.button.button == SDL_BUTTON_LEFT)
		doom->mdl->player.shooting = 0;
	if (doom->event.type == SDL_MOUSEBUTTONDOWN && doom->event.button.button == SDL_BUTTON_LEFT)
		doom->mdl->player.shooting = 1;
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
	static int mix_i = 0;
	static int lock_i = 0;

	if (doom->keystates[SDL_SCANCODE_I] && !lock_i)
	{
		doom->mdl->player.invis == 0 ? printf("Invisibility on %d!\n", (doom->mdl->player.invis = 1)) : printf(
				"Invisibility off %d!\n", (doom->mdl->player.invis = 0));
		lock_i = 1;
	}
	if (lock_i && !doom->keystates[SDL_SCANCODE_I])
		lock_i = 0;
	handle_player_movement(doom);
	handle_player_action(doom);
	player_update_weapons(doom);
	if (doom->mdl->player.shooting)
	{
		if (doom->mdl->player.shoot_cd == 0 && doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur > 0 && doom->mdl->player.reload_time == 0)
		{
			Mix_PlayChannel(mix_i, doom->mdl->player.weap_arr[doom->mdl->player.weap_id].fire_sound, 0);
			mix_i++;
			if (mix_i > 7)
				mix_i = 0;
			player_shoots(doom);
		}
	}
	handle_enemy_ai(doom);
	if (DEBUG == 1)
		update_minimap(doom);
	render_frame(doom);
	render_game_hud(doom);

	/*
	static SDL_Surface *flip = NULL;
	if (!flip)
		flip = flip_horizontal(doom->sprites->txt_ranged_side_idle);
	draw_surface(0,0,doom->sprites->txt_ranged_side_idle, doom->game->buff);
	draw_surface(doom->sprites->txt_ranged_side_idle->w * 2,0, flip, doom->game->buff);
	*/
	SDL_UpdateWindowSurface(doom->game->win);
}
