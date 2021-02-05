/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:15:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 17:19:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static int		equal_points(t_point a, t_point b)
{
	if (a.x == b.x && a.y == b.y)
		return (1);
	return (0);
}

static int		matching_walls(t_wall *a, t_wall *b)
{
	if (equal_points(a->start, b->start) && equal_points(a->end, b->end))
		return (1);
	if (equal_points(a->start, b->end) && equal_points(a->end, b->start))
		return (1);
	return (0);
}

int				portal_belongs_to_room(t_wall *portal, t_room *room)
{
	t_wall	*wall;
	int 	wc;

	wall = room->first_wall;
	wc = room->wall_count;
	while (wc--)
	{
		if (matching_walls(portal, wall))
			return (1);
		wall = wall->next;
	}
	return (0);
}