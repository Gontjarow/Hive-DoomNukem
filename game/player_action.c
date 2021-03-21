/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_action.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 16:43:51 by msuarez-          #+#    #+#             */
/*   Updated: 2021/03/21 01:47:06 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	player_jumps(t_doom *doom)
{
	if (doom->mdl->player.is_jumping == 0 &&
		doom->mdl->player.is_crouching == 0)
	{
		doom->mdl->player.z_velocity += 3.0;
		doom->mdl->player.is_jumping = 1;
	}
}

static void	player_crouch(t_doom *doom)
{
	if (!doom->mdl->player.is_crouching)
	{
		doom->mdl->player.crouch_lock = 1;
		doom->mdl->player.is_crouching = 1;
		doom->mdl->player.mov_speed = 5;
		doom->mdl->player.height = CROUCH_HEIGHT;
	}
	else
	{
		doom->mdl->player.crouch_lock = 0;
		doom->mdl->player.is_crouching = 0;
		doom->mdl->player.mov_speed = 10;
		doom->mdl->player.height = STAND_HEIGHT;
	}
}

static void	player_run(t_doom *doom)
{
	if (!doom->mdl->player.run_lock)
	{
		doom->mdl->player.run_lock = 1;
		doom->mdl->player.is_running = 1;
	}
	else
	{
		doom->mdl->player.is_running = 0;
		doom->mdl->player.run_lock = 0;
	}
}

static void	experimental_elevator(int active)
{
	static int 	original_height = -1;

	if (active && original_height == -1)
	{
		original_height = get_model()->room_first->floor_height;
	}
	else if (active)
	{
		get_model()->room_first->floor_height = get_model()->room_first->floor_height + 8;
		get_model()->room_first->floor_height = get_model()->room_first->floor_height > 750 ? 750 : get_model()->room_first->floor_height;
	}
	else
		get_model()->room_first->floor_height = original_height;
}

void		handle_player_action(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_SPACE])
		player_jumps(doom);
	if (doom->keystates[SDL_SCANCODE_LCTRL] && !doom->mdl->player.crouch_lock
		&& !doom->mdl->player.is_jumping)
		player_crouch(doom);
	if (!doom->keystates[SDL_SCANCODE_LCTRL] && doom->mdl->player.crouch_lock)
		player_crouch(doom);
	if (doom->keystates[SDL_SCANCODE_LSHIFT] && !doom->mdl->player.run_lock
		&& !doom->mdl->player.crouch_lock && !doom->mdl->player.is_jumping)
		player_run(doom);
	if (!doom->keystates[SDL_SCANCODE_LSHIFT] && doom->mdl->player.run_lock)
		player_run(doom);
	if (doom->keystates[SDL_SCANCODE_P])
		debug_model_effects();
	if (doom->keystates[SDL_SCANCODE_L])
		debug_model_chain();
	if (doom->keystates[SDL_SCANCODE_E])
		experimental_elevator(1);
	if (doom->keystates[SDL_SCANCODE_R])
		experimental_elevator(0);

}
