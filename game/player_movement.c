/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:30:16 by msuarez-          #+#    #+#             */
/*   Updated: 2021/04/03 19:31:17 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void			update_player_tail(t_doom *doom, double rad)
{
	t_point	p;

	p.x = doom->mdl->player.x + 10 * -cos(rad);
	p.y = doom->mdl->player.y + 10 * -sin(rad);
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
	((double)doom->mdl->player.mov_speed * doom->delta_time)) * -cos(rad);
	doom->mdl->player.y = doom->mdl->player.y + (signal *
	((double)doom->mdl->player.mov_speed * doom->delta_time)) * -sin(rad);
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
	(((double)doom->mdl->player.mov_speed * doom->delta_time)) * -cos(strafe_rad);
	doom->mdl->player.y = doom->mdl->player.y +
	(((double)doom->mdl->player.mov_speed * doom->delta_time)) * -sin(strafe_rad);
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

static void no_go(t_model *mdl, t_coord old)
{
	puts("No go!");
	mdl->player.x = old.x;
	mdl->player.y = old.y;
}

// TODO FINISH CROSSING_HEIGHTS CHECKS
// 		CAN ALWAYS FALL DOWN TO LOWER HEIGHT ROOM
// 		CAN NOT ALWAYS ENTER ROOM IF HEAD IS HITTING THE TOP CEILING OF THE OPENING
static int check_crossing_heights(t_model *mdl, t_coord old, int room_test)
{
	t_room *room;
	int i;

	i = 0;
	room = mdl->room_first;
	while (i++ < room_test)
		room = room->next;
	if (room->id != room_test)
	{
		ft_putendl("Room->id != room_test !");
	}
	if (mdl->player.z - mdl->player.height + KNEE_HEIGHT > room->floor_height)
	{
		//puts("Knee test OK!");
		return (1);
	}
	else
	{
		//puts("FAIL Knee test!");
		//printf("player.z - player.height = %d | room->floor_height = %d\n", (int)(mdl->player.z) - mdl->player.height, room->floor_height);
		return (0);
	}
}

static void cross_boundaries(t_model *mdl, t_coord old)
{
	static double 	prev_x, prev_y;
	static int 		room_prev = -1;
	int 			room_test;
	int				crossing_ok;

		//puts("Crossing boundaries");
	if (room_prev == -1)
		room_prev = room_id_from_polymap(get_model()->poly_map, (int)get_model()->player.x, (int)get_model()->player.y);
	room_test = room_id_from_polymap(get_model()->poly_map, (int)get_model()->player.x, (int)get_model()->player.y);
	if (room_test != room_prev)
		crossing_ok = check_crossing_heights(mdl, old, room_test);
	else
	{
			//puts("All clear!");
		room_prev = room_test;
		prev_x = mdl->player.x;
		prev_y = mdl->player.y;
		return ;
	}
	if (crossing_ok)
	{
		room_prev = room_test;
		prev_x = mdl->player.x;
		prev_y = mdl->player.y;
	}
	else
		no_go(mdl, old);
}

static void		apply_gravity(t_doom *doom)
{
	doom->mdl->player.z += doom->mdl->player.z_velocity;
	if ((int)doom->mdl->player.z > doom->mdl->player.room->floor_height + doom->mdl->player.height)
	{
		puts("reducing height!");
		doom->mdl->player.z_velocity -= 0.25;
	}
	else if ((int)doom->mdl->player.z < doom->mdl->player.room->floor_height + doom->mdl->player.height
		&& doom->mdl->player.z_velocity == 0.0)
	{
		puts("bouncing up from the crouch!");
		//doom->mdl->player.z += 1.0;
		doom->mdl->player.z += 0.5;
	}
	else if ((int)doom->mdl->player.z < doom->mdl->player.room->floor_height + doom->mdl->player.height)
	{
		puts("hit the floor with the feet!");
		//printf("player.z %d | room.floor_height %d | player.height %d\n", (int)doom->mdl->player.z, doom->mdl->player.room->floor_height, doom->mdl->player.height);
		doom->mdl->player.z = doom->mdl->player.room->floor_height + doom->mdl->player.height;
		doom->mdl->player.z_velocity = 0.0;
		doom->mdl->player.is_jumping = 0;
	}
	else if ((int)doom->mdl->player.z > doom->mdl->player.room->roof_height)
	{
		puts("hit the roof with the head");
		doom->mdl->player.z = doom->mdl->player.room->roof_height;
		doom->mdl->player.z_velocity = 0.0;
	}
	// printf("player.z %d | room.floor_height %d | player.height %d\n", (int)doom->mdl->player.z, doom->mdl->player.room->floor_height, doom->mdl->player.height);
}

static void		apply_velocity(t_doom *doom)
{
	if (doom->mdl->player.is_running && doom->mdl->player.mov_speed != doom->mdl->player.max_speed)
		doom->mdl->player.mov_speed += 100;
	else if (!doom->mdl->player.is_running && doom->mdl->player.mov_speed > 800)
		doom->mdl->player.mov_speed -= 100;
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
	if (doom->keystates[SDL_SCANCODE_LSHIFT] && (doom->keystates[SDL_SCANCODE_UP] || doom->keystates[SDL_SCANCODE_DOWN]))
	{
		doom->mdl->player.z = doom->keystates[SDL_SCANCODE_UP] ? doom->mdl->player.z + 5 : doom->mdl->player.z - 5;
		ft_putnbr(doom->mdl->player.z);
		ft_putendl(" player Z position");
	}
	else if (doom->keystates[SDL_SCANCODE_UP] || doom->keystates[SDL_SCANCODE_DOWN])
	{
		doom->mdl->player.height = doom->keystates[SDL_SCANCODE_UP] ? doom->mdl->player.height + 1 : doom->mdl->player.height - 1;
		ft_putnbr(doom->mdl->player.height);
		ft_putendl(" player height");
	}
	cross_boundaries(doom->mdl, old);
	validate_player_position(doom, old);
	apply_gravity(doom);
	apply_velocity(doom);
}
