/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:11:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 17:13:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

t_room				*room_by_wall_id(int id, t_model *mdl)
{
	t_room *room;
	int     rc;

	// NOT saved to map data

	room->visual.x;
	room->visual.y;

	// Can be recalculated on map data load

	room = mdl->room_first;
	rc = mdl->room_count;
	while (rc--)
	{
		if (room->first_wall_id + room->wall_count > id)
			return (room);
		room = room->next;
	}
	ft_die("Fatal error: Could not find room_by_wall_id");
	return (NULL);
}

static void		flip_wall(t_wall *wall)
{
	t_point		tmp;

	tmp.x = wall->start.x;
	tmp.y = wall->start.y;
	wall->start.x = wall->end.x;
	wall->start.y = wall->end.y;
	wall->end.x = tmp.x;
	wall->end.y = tmp.y;
}

void			flip_room(t_room *room, t_model *mdl)
{
	t_wall		**walls;
	t_wall		*iter;
	int			i;

	flip_wall(room->first_wall);
	walls = (t_wall**)malloc(sizeof(t_wall*) * room->wall_count);
	if (walls == NULL)
		ft_die("Fatal error: Could not malloc walls pointers at flip_room.");
	walls[0] = room->first_wall;
	//printf("flip_wall on walls[%d]\n", 0);
	iter = room->first_wall;
	i = room->wall_count;
	while (--i > 0)
	{
		walls[i] = iter->next;
		walls[i]->id = room->first_wall_id + i;
		//printf("set walls[%d] to be wall_id[%d] | adjusted id to be [%d]\n", i, iter->next->id, walls[i]->id);
		//printf("flip_wall on walls[%d]\n", i);
		flip_wall(walls[i]);
		iter = iter->next;
	}
	room->first_wall = walls[0];
	iter = room->first_wall;
	while (i < room->wall_count - 1)
	{
		iter->next = walls[++i];
		iter = iter->next;
	}
	//printf("walls[%d]->next set mdl->walls\n", i);
	iter->next = mdl->walls;
	free(walls);

	/*
	int debug = 0;
	t_wall *debug_wall;
	debug_wall = room->first_wall;

	while (debug < room->wall_count)
	{
		printf("debug_wall id %d\n", debug_wall->id);
		debug_wall = debug_wall->next;
		debug++;
	}
	*/
}