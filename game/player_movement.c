/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:30:16 by msuarez-          #+#    #+#             */
/*   Updated: 2021/03/21 01:46:36 by msuarez-         ###   ########.fr       */
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
	signal = (-1) * signal;
	if (doom->mdl->player.is_crouching && doom->sounds->footstep_delay == 0)
		Mix_PlayChannel(-1, doom->sounds->mcCrouching, 0);
	else if (doom->mdl->player.is_running && doom->sounds->footstep_delay == 0)
		Mix_PlayChannel(-1, doom->sounds->mcRunning, 0);
	else if (!doom->mdl->player.is_running && !doom->mdl->player.is_crouching
			&& doom->sounds->footstep_delay == 0)
		Mix_PlayChannel(-1, doom->sounds->mcWalking, 0);
	if (doom->sounds->footstep_delay == 0)
		doom->sounds->footstep_delay = 8;
	doom->mdl->player.x = doom->mdl->player.x + (signal *
	((double)doom->mdl->player.mov_speed)) * -cos(rad);
	doom->mdl->player.y = doom->mdl->player.y + (signal *
	((double)doom->mdl->player.mov_speed)) * -sin(rad);
	update_player_tail(doom, rad);
	handle_pickup(doom);
}

static void		strafe(t_doom *doom, int signal)
{
	int			orig_rot;
	double		strafe_rad;

	orig_rot = doom->mdl->player.rot;
	doom->mdl->player.rot = doom->mdl->player.rot + (90 * signal);
	if (doom->mdl->player.rot < 0)
		doom->mdl->player.rot = 359 + (doom->mdl->player.rot);
	if (doom->mdl->player.rot >= 360)
		doom->mdl->player.rot = 0 + (doom->mdl->player.rot) - 360;
	strafe_rad = deg_to_rad(doom->mdl->player.rot);
	doom->mdl->player.x = doom->mdl->player.x +
	(((double)doom->mdl->player.mov_speed)) * -cos(strafe_rad);
	doom->mdl->player.y = doom->mdl->player.y +
	(((double)doom->mdl->player.mov_speed)) * -sin(strafe_rad);
	doom->mdl->player.rot = orig_rot;
	update_player_tail(doom, deg_to_rad(doom->mdl->player.rot));
	handle_pickup(doom);
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

static void		apply_gravity(t_doom *doom)
{
	doom->mdl->player.z += doom->mdl->player.z_velocity;
	if ((int)doom->mdl->player.z > doom->mdl->player.room->floor_height + doom->mdl->player.height)
	{
		//puts("reducing height!");
		doom->mdl->player.z_velocity -= 0.25;
	}
	else if ((int)doom->mdl->player.z < doom->mdl->player.room->floor_height + doom->mdl->player.height
		&& doom->mdl->player.z_velocity == 0.0)
	{
			//puts("bouncing up from the crouch!");
		doom->mdl->player.z += 1.0;
	}
	else if ((int)doom->mdl->player.z < doom->mdl->player.room->floor_height + doom->mdl->player.height)
	{
			//puts("hit the floor with the feet!");
		doom->mdl->player.z = doom->mdl->player.room->floor_height + doom->mdl->player.height;
		doom->mdl->player.z_velocity = 0.0;
		doom->mdl->player.is_jumping = 0;
	}
	else if ((int)doom->mdl->player.z > doom->mdl->player.room->roof_height)
	{
			//puts("hit the roof with the head");
		doom->mdl->player.z = doom->mdl->player.room->roof_height;
		doom->mdl->player.z_velocity = 0.0;
	}
	// printf("player.z %d | room.floor_height %d | player.height %d\n", (int)doom->mdl->player.z, doom->mdl->player.room->floor_height, doom->mdl->player.height);
}

static void		apply_velocity(t_doom *doom)
{
	if (doom->mdl->player.is_running && doom->mdl->player.mov_speed != doom->mdl->player.max_speed)
		doom->mdl->player.mov_speed += 1;
	else if (!doom->mdl->player.is_running && doom->mdl->player.mov_speed > 10)
		doom->mdl->player.mov_speed -= 1;
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
		strafe(doom, 1);
	if (doom->keystates[SDL_SCANCODE_D])
		strafe(doom, -1);
	if (doom->keystates[SDL_SCANCODE_LEFT])
		rotating_left_right(doom, -1);
	if (doom->keystates[SDL_SCANCODE_RIGHT])
		rotating_left_right(doom, 1);
	if (doom->keystates[SDL_SCANCODE_UP] || doom->keystates[SDL_SCANCODE_DOWN])
	{
		doom->mdl->player.height = doom->keystates[SDL_SCANCODE_UP] ? doom->mdl->player.height + 1 : doom->mdl->player.height - 1;
		ft_putnbr(doom->mdl->player.height);
		ft_putendl(" player height");
	}
	static int lock_c = 0;
	if (doom->keystates[SDL_SCANCODE_C] && !lock_c)
	{
		doom->game->cel_shade_hud = !(doom->game->cel_shade_hud);
		lock_c = 1;
	}
	if (!doom->keystates[SDL_SCANCODE_C] && lock_c)
		lock_c = 0;
	static int lock_m = 0;
	if (doom->keystates[SDL_SCANCODE_M] && !lock_m)
	{
		doom->game->show_info = !(doom->game->show_info);
		lock_m = 1;
	}
	if (!doom->keystates[SDL_SCANCODE_M] && lock_m)
		lock_m = 0;
	validate_player_position(doom, old);
	apply_gravity(doom);
	apply_velocity(doom);
}
