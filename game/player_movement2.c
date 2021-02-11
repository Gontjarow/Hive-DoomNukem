/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 19:07:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/11 18:18:14 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void			init_player_z(t_doom *doom)
{
	int			room_id;

	room_id = room_id_from_polymap(doom->mdl->poly_map, doom->mdl->player.x, doom->mdl->player.y);
	doom->mdl->player.z = room_by_id(room_id)->floor_height + doom->mdl->player.height;
}

static int		check_eight_pos_points(t_doom *doom)
{
	t_coord		loc_id[8];
	int			i;

	loc_id[0].x = doom->mdl->player.x + 10;
	loc_id[0].y = doom->mdl->player.y;
	loc_id[1].x = doom->mdl->player.x - 10;
	loc_id[1].y = doom->mdl->player.y;
	loc_id[2].x = doom->mdl->player.x;
	loc_id[2].y = doom->mdl->player.y - 10;
	loc_id[3].x = doom->mdl->player.x;
	loc_id[3].y = doom->mdl->player.y + 10;

	loc_id[4].x = doom->mdl->player.x + 7;
	loc_id[4].y = doom->mdl->player.y - 7;
	loc_id[5].x = doom->mdl->player.x - 7;
	loc_id[5].y = doom->mdl->player.y - 7;
	loc_id[6].x = doom->mdl->player.x + 7;
	loc_id[6].y = doom->mdl->player.y + 7;
	loc_id[7].x = doom->mdl->player.x - 7;
	loc_id[7].y = doom->mdl->player.y + 7;
	i = 0;
	while (i < 8)
	{
		if (check_location(doom, loc_id[i].x, loc_id[i].y) == -1)
			return (-1);
		i++;
	}
	return (0);
}	

void			validate_player_position(t_doom *doom, t_coord old)
{
	static	int	last_room_id = -1;
	int			current_room;
	int			location_id;

	location_id = check_location(doom, doom->mdl->player.x,
								 doom->mdl->player.y);
	if (location_id == -1 || location_id == UINT_ERROR_CONSTANT ||
		player_collision_with_enemies(doom) == -1 || check_eight_pos_points(doom) == -1)
	{
		doom->mdl->player.x = old.x;
		doom->mdl->player.y = old.y;
	}
	current_room = check_location(doom, doom->mdl->player.x, doom->mdl->player.y);
	if (last_room_id != current_room)
	{
		doom->mdl->player.room_id = current_room;
		doom->mdl->player.room = room_by_id(current_room);
		last_room_id = current_room;
	}
}
