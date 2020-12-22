/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:30:16 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/22 15:42:38 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void			update_player_tail(t_doom *doom, double rad)
{
	t_point	p;

	p.x = doom->mdl->player.x + doom->mdl->player.mov_speed * -cos(rad);
	p.y = doom->mdl->player.y + doom->mdl->player.mov_speed * -sin(rad);
	doom->mdl->player.tail.x = p.x;
	doom->mdl->player.tail.y = p.y;
}

static void		moving_up_down(t_doom *doom, int signal, double rad)
{
	if (doom->mdl->player.is_crouching && doom->sounds->footstep_delay == 0)
		Mix_PlayChannel(-1, doom->sounds->mcCrouching, 0);
	else if (doom->mdl->player.is_running && doom->sounds->footstep_delay == 0)
		Mix_PlayChannel(-1, doom->sounds->mcRunning, 0);
	else if (!doom->mdl->player.is_running && !doom->mdl->player.is_crouching && doom->sounds->footstep_delay == 0)
		Mix_PlayChannel(-1, doom->sounds->mcWalking, 0);
	if (doom->sounds->footstep_delay == 0)
		doom->sounds->footstep_delay = 8;
	doom->mdl->player.x = doom->mdl->player.x + (signal *
	((double)doom->mdl->player.mov_speed)) * -cos(rad);
	doom->mdl->player.y = doom->mdl->player.y + (signal *
	((double)doom->mdl->player.mov_speed)) * -sin(rad);
	update_player_tail(doom, rad);
}

static void		rotating_left_right(t_doom *doom, int signal)
{
	doom->mdl->player.rot = doom->mdl->player.rot + (signal *
							doom->mdl->player.rot_speed);
	if (doom->mdl->player.rot < 0)
		doom->mdl->player.rot = 359;
	if (doom->mdl->player.rot >= 360)
		doom->mdl->player.rot = 0;
	update_player_tail(doom, deg_to_rad(doom->mdl->player.rot));
}

static void		validate_player_position(t_doom *doom, t_coord old)
{
	int		location_id;

	location_id = check_location(doom, doom->mdl->player.x,
					doom->mdl->player.y);
	if (location_id == -1 || location_id == UINT_ERROR_CONSTANT ||
		player_collision_with_enemies(doom) == -1)
	{
		doom->mdl->player.x = old.x;
		doom->mdl->player.y = old.y;
	}
}

void			handle_player_movement(t_doom *doom)
{
	double	rad;
	t_coord	old;

	old.x = doom->mdl->player.x;
	old.y = doom->mdl->player.y;
	rad = deg_to_rad(doom->mdl->player.rot);
	if (doom->sounds->footstep_delay > 0)
		doom->sounds->footstep_delay--;
	if (doom->keystates[SDL_SCANCODE_W])
		moving_up_down(doom, 1, rad);
	if (doom->keystates[SDL_SCANCODE_S])
		moving_up_down(doom, -1, rad);
	if (doom->keystates[SDL_SCANCODE_A])
		rotating_left_right(doom, -1);
	if (doom->keystates[SDL_SCANCODE_D])
		rotating_left_right(doom, 1);
	validate_player_position(doom, old);
}
