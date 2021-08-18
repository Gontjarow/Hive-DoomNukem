/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 19:07:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 20:30:02 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static int	check_eight_pos_points(t_doom *doom, t_coord loc_id[8])
{
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
	i = -1;
	while (++i < 8)
	{
		if (check_location(doom, loc_id[i].x, loc_id[i].y) == -1)
			return (-1);
	}
	return (0);
}

static t_coord	check_segment(t_doom *doom, t_point start, t_point end)
{
	t_coord	dist;
	t_coord	closest;
	double	dot;
	double	len;

	dist.x = start.x - end.x;
	dist.y = start.y - end.y;
	len = sqrt((dist.x * dist.x) + (dist.y * dist.y));
	dot = (((doom->mdl->player.x - start.x) * (end.x
					- start.x)) + ((doom->mdl->player.y - start.y)
				* (end.y - start.y)))
		/ pow(len, 2);
	closest.x = start.x + (dot
			* (end.x - start.x));
	closest.y = start.y + (dot
			* (end.y - start.y));
	return (closest);
}

int	line_point_portal(t_doom *doom, t_coord p, t_point s, t_point end)
{
	double	d1;
	double	d2;
	double	len;
	double	buffer;

	d1 = dist(p.x, p.y, s.x, s.y);
	d2 = dist(p.x, p.y, end.x, end.y);
	len = dist(s.x, s.y, end.x, end.y);
	buffer = 0.001;
	if (d1 + d2 >= len - buffer && d1 + d2 <= len + buffer)
		return (1);
	return (0);
}

static int	validate_point_portal(t_doom *doom, t_wall *portals)
{
	if ((point_circle(portals->start.x, portals->start.y,
				doom->mdl->player.x, doom->mdl->player.y)
			|| point_circle(portals->end.x,
				portals->end.y, doom->mdl->player.x, doom->mdl->player.y)))
		return (-1);
	return (0);
}

static int	validate_distance_portal(t_doom *doom, t_coord dist, t_coord c)
{
	dist.x = c.x - doom->mdl->player.x;
	dist.y = c.y - doom->mdl->player.y;
	if (sqrt((dist.x * dist.x) + (dist.y * dist.y)) <= 10)
		return (-1);
	return (0);
}

static int	player_collision_portals(t_doom *doom, t_coord c, t_coord dist)
{
	int		pc;
	t_wall	*portals;

	pc = doom->mdl->portal_count;
	if (pc == 0)
		return (0);
	portals = doom->mdl->portal_first;
	while (pc--)
	{
		if (portals->open == 0)
		{
			if (validate_point_portal(doom, portals) == -1)
				return (-1);
			c = check_segment(doom, portals->start, portals->end);
			if (!line_point_portal(doom, c, portals->start, portals->end))
			{
				portals = portals->next;
				continue ;
			}
			if (validate_distance_portal(doom, dist, c) == -1)
				return (-1);
		}
		portals = portals->next;
	}
	return (0);
}

void	validate_player_position(t_doom *doom, t_coord old)
{
	static int	last_room_id = -1;
	int			current_room;
	int			location_id;
	t_coord		loc_id[8];

	location_id = check_location(doom, doom->mdl->player.x,
			doom->mdl->player.y);
	if (location_id == -1 || location_id == UINT_ERROR_CONSTANT
		|| player_collision_with_enemies(doom)
		== -1 || check_eight_pos_points(doom, loc_id) == -1
		|| player_collision_portals(doom, (t_coord){0, 0},
		(t_coord){0, 0}) == -1)
	{
		doom->mdl->player.x = old.x;
		doom->mdl->player.y = old.y;
	}
	current_room = check_location(doom, doom->mdl->player.x,
			doom->mdl->player.y);
	if (last_room_id != current_room)
	{
		doom->mdl->player.room_id = current_room;
		doom->mdl->player.room = room_by_id(current_room);
		last_room_id = current_room;
	}
}
