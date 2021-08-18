/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 15:30:16 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 20:12:37 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	no_go(t_model *mdl, t_coord old)
{
	mdl->player.x = old.x;
	mdl->player.y = old.y;
}

// TODO FINISH CROSSING_HEIGHTS CHECKS
// 		CAN ALWAYS FALL DOWN TO LOWER HEIGHT ROOM
// 		CAN NOT ALWAYS ENTER ROOM IF HEAD IS HITTING THE TOP CEILING OF THE OPENING
static int	check_crossing_heights(t_model *mdl, t_coord old, int room_test)
{
	t_room	*room;
	int		i;

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

void	cross_boundaries(t_model *mdl, t_coord old)
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

void	apply_gravity(t_doom *doom)
{
	doom->mdl->player.z += doom->mdl->player.z_velocity;
	if ((int)doom->mdl->player.z > doom->mdl->player.room->roof_height)
	{
		doom->mdl->player.z = (double)doom->mdl->player.room->roof_height;
		doom->mdl->player.z_velocity = -0.25;
		return ;
	}
	if ((int)doom->mdl->player.z > doom->mdl->player.room->floor_height + doom->mdl->player.height)
	{
		doom->mdl->player.z_velocity -= 0.25;
	}
	else if ((int)doom->mdl->player.z < doom->mdl->player.room->floor_height + doom->mdl->player.height
		&& doom->mdl->player.z_velocity == 0.0)
	{
		doom->mdl->player.z += 0.5;
	}
	else if ((int)doom->mdl->player.z < doom->mdl->player.room->floor_height + doom->mdl->player.height)
	{
		doom->mdl->player.z = doom->mdl->player.room->floor_height + doom->mdl->player.height;
		doom->mdl->player.z_velocity = 0.0;
		doom->mdl->player.is_jumping = 0;
		doom->mdl->player.is_flying = 0;
	}
}

void	apply_velocity(t_doom *doom)
{
	if (doom->mdl->player.is_running && doom->mdl->player.mov_speed
		!= doom->mdl->player.max_speed)
		doom->mdl->player.mov_speed += 100;
	else if (!doom->mdl->player.is_running && doom->mdl->player.mov_speed > 300)
		doom->mdl->player.mov_speed -= 100;
}
