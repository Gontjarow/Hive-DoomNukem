/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_action2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 19:25:15 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 19:35:21 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	player_jumps(t_doom *doom)
{
	if (doom->mdl->player.weap_arr[JETPACK].do_own
		&& doom->mdl->player.weap_arr[JETPACK].ammo_cur > 0)
	{
		doom->mdl->player.is_flying = 1;
		doom->mdl->player.weap_arr[JETPACK].ammo_cur--;
		doom->mdl->player.z_velocity += 0.4;
		if (doom->sounds->jetpack_delay <= 0)
		{
			Mix_PlayChannel(-1, doom->sounds->mcJetpack, 0);
			doom->sounds->jetpack_delay = 0.5;
		}
		return ;
	}
	if (doom->mdl->player.is_jumping == 0
		&& doom->mdl->player.is_crouching == 0)
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
		doom->mdl->player.mov_speed = doom->mdl->player.min_speed;
		doom->mdl->player.height = CROUCH_HEIGHT;
	}
	else
	{
		doom->mdl->player.crouch_lock = 0;
		doom->mdl->player.is_crouching = 0;
		doom->mdl->player.mov_speed = 300;
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

void	handle_player_action(t_doom *doom)
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
	if (!doom->keystates[SDL_SCANCODE_E] && doom->mdl->player.eff_pressed)
		doom->mdl->player.eff_pressed = 0;
	if (doom->keystates[SDL_SCANCODE_E] && !doom->mdl->player.eff_pressed)
		player_effector_interactions(doom, 0);
	// if (doom->keystates[SDL_SCANCODE_P])
	// 	debug_model_effects();
	// if (doom->keystates[SDL_SCANCODE_L])
	// 	debug_model_chain();
	// if (doom->keystates[SDL_SCANCODE_U])
	// 	experimental_elevator(1, 0);
	// if (doom->keystates[SDL_SCANCODE_J])
	// 	experimental_elevator(0, 0);
	// if (doom->keystates[SDL_SCANCODE_N])
	// 	experimental_elevator(0, 1);
}