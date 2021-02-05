/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_room.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:15:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 17:21:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void		reduce_remaining_fwids(t_room *after, int x, t_model *mdl)
{
	t_room		*room;
	int			rc;

	room = mdl->room_first;
	rc = mdl->room_count;
	while (rc-- && room != after)
		room = room->next;
	room = room->next;
	while (rc--)
	{
		room->first_wall_id -= x;
		room = room->next;
	}
}

static void		link_two_rooms(t_room *a, t_room *b)
{
	t_wall		*wall;
	int			wc;

	a->next = b;
	wc = a->wall_count;
	wall = a->first_wall;
	while (--wc)
		wall = wall->next;
	wall->next = b->first_wall;
}

static void		relink_after_last_room_deleted(t_room *room, t_model *mdl)
{
	t_wall		*wall;
	int			wc;

	room->next = mdl->rooms;
	wc = room->wall_count;
	wall = room->first_wall;
	while (--wc)
		wall = wall->next;
	wall->next = mdl->walls;
}

static void		room_deletion_recalcs(t_model *mdl)
{
	mdl->room_count--;
	recalc_wall_ids(mdl);
	recalc_room_ids(mdl);
	repaint_polymap(mdl);
}

void			delete_room(t_room *room, int del_count, t_model *mdl)
{
	t_wall		*wall;
	t_wall		*tmp;
	int			wc;

	wc = room->wall_count;
	wall = room->first_wall;
	while (wc--)
	{
		tmp = wall;
		wall = wall->next;
		free(tmp);
	}
	reduce_remaining_fwids(room, del_count, mdl);
	mdl->wall_count -= del_count;
	if (mdl->room_first == room)
	{
		mdl->room_first = room->next;
		mdl->wall_first = room->next->first_wall;
	}
	else if (room->id != mdl->room_count - 1)
		link_two_rooms(room_by_id(room->id - 1), room->next);
	else
		relink_after_last_room_deleted(room_by_id(room->id - 1), mdl);
	room_deletion_recalcs(mdl);
	free(room);
}
