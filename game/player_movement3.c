/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 20:07:43 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 20:30:20 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	moving_up_down(t_doom *doom, int signal, double rad)
{
	signal = (-1) * signal;
	if (doom->mdl->player.is_crouching && doom->sounds->footstep_delay <= 0)
		Mix_PlayChannel(-1, doom->sounds->mcCrouching, 0);
	else if (doom->mdl->player.is_running && doom->sounds->footstep_delay <= 0)
		Mix_PlayChannel(-1, doom->sounds->mcRunning, 0);
	else if (!doom->mdl->player.is_running && !doom->mdl->player.is_crouching
		&& doom->sounds->footstep_delay <= 0 && !doom->mdl->player.is_flying)
		Mix_PlayChannel(-1, doom->sounds->mcWalking, 0);
	if (doom->sounds->footstep_delay <= 0)
		doom->sounds->footstep_delay = 0.5;
	doom->mdl->player.x = doom->mdl->player.x + ((float)signal
			* ((float)doom->mdl->player.mov_speed * doom->delta_time))
		* -cos(rad);
	doom->mdl->player.y = doom->mdl->player.y + ((float)signal
			* ((float)doom->mdl->player.mov_speed * doom->delta_time))
		* -sin(rad);
	update_player_tail(doom, rad);
	handle_pickup(doom);
}

static void	strafe(t_doom *doom, int signal)
{
	int			orig_rot;
	double		strafe_rad;

	orig_rot = doom->mdl->player.rot;
	doom->mdl->player.rot = doom->mdl->player.rot + (90 * signal);
	if (!doom->mdl->player.is_running && !doom->mdl->player.is_crouching
		&& doom->sounds->footstep_delay <= 0 && !doom->mdl->player.is_flying)
		Mix_PlayChannel(-1, doom->sounds->mcWalking, 0);
	if (doom->sounds->footstep_delay <= 0)
		doom->sounds->footstep_delay = 0.5;
	if (doom->mdl->player.rot < 0)
		doom->mdl->player.rot = 359 + (doom->mdl->player.rot);
	if (doom->mdl->player.rot >= 360)
		doom->mdl->player.rot = 0 + (doom->mdl->player.rot) - 360;
	strafe_rad = deg_to_rad(doom->mdl->player.rot);
	doom->mdl->player.x = doom->mdl->player.x + ((((double)doom->mdl->player
					.mov_speed - 50) * doom->delta_time)) * -cos(strafe_rad);
	doom->mdl->player.y = doom->mdl->player.y + ((((double)doom->mdl->player
					.mov_speed - 50) * doom->delta_time)) * -sin(strafe_rad);
	doom->mdl->player.rot = orig_rot;
	update_player_tail(doom, deg_to_rad(doom->mdl->player.rot));
	handle_pickup(doom);
}

static void	rotating_left_right(t_doom *doom, int signal)
{
	doom->mdl->player.rot = doom->mdl->player.rot + (signal
			* doom->mdl->player.rot_speed);
	if (doom->mdl->player.rot < 0)
		doom->mdl->player.rot = 359;
	if (doom->mdl->player.rot >= 360)
		doom->mdl->player.rot = 0;
	update_player_tail(doom, deg_to_rad(doom->mdl->player.rot));
}

static void	handle_player_controls(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_A])
		strafe(doom, 1);
	if (doom->keystates[SDL_SCANCODE_D])
		strafe(doom, -1);
	if (doom->keystates[SDL_SCANCODE_LEFT])
		rotating_left_right(doom, -1);
	if (doom->keystates[SDL_SCANCODE_RIGHT])
		rotating_left_right(doom, 1);
	if (doom->keystates[SDL_SCANCODE_LSHIFT]
		&& (doom->keystates[SDL_SCANCODE_UP]
			|| doom->keystates[SDL_SCANCODE_DOWN]))
	{
		if (doom->keystates[SDL_SCANCODE_UP])
			doom->mdl->player.z = doom->mdl->player.z + 5;
		else
			doom->mdl->player.z = doom->mdl->player.z - 5;
	}
	else if (doom->keystates[SDL_SCANCODE_UP]
		|| doom->keystates[SDL_SCANCODE_DOWN])
	{
		if (doom->keystates[SDL_SCANCODE_UP])
			doom->mdl->player.height = doom->mdl->player.height + 1;
		else
			doom->mdl->player.height = doom->mdl->player.height - 1;
	}
}

void	handle_player_movement(t_doom *doom)
{
	double	rad;
	t_coord	old;

	old.x = doom->mdl->player.x;
	old.y = doom->mdl->player.y;
	rad = deg_to_rad(doom->mdl->player.rot);
	if (doom->sounds->footstep_delay > 0)
		doom->sounds->footstep_delay -= 1 * doom->delta_time;
	if (doom->keystates[SDL_SCANCODE_W] || doom->keystates[SDL_SCANCODE_UP])
		moving_up_down(doom, 1, rad);
	if (doom->keystates[SDL_SCANCODE_S] || doom->keystates[SDL_SCANCODE_DOWN])
		moving_up_down(doom, -1, rad);
	handle_player_controls(doom);
	cross_boundaries(doom->mdl, old);
	validate_player_position(doom, old);
	apply_gravity(doom);
	apply_velocity(doom);
}
