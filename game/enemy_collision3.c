/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_collision3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 17:58:30 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 18:06:37 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int	validate_portal_collision(t_wall *portals, t_enemy *enemy)
{
	if ((point_circle(portals->start.x, portals->start.y,
				enemy->x, enemy->y) || point_circle(portals->end.x,
				portals->end.y, enemy->x, enemy->y)))
		return (-1);
	return (0);
}

int	validate_portal_dist(t_coord closest, t_enemy *enemy)
{
	t_coord	dist;

	dist.x = closest.x - enemy->x;
	dist.y = closest.y - enemy->y;
	if (sqrt((dist.x * dist.x) + (dist.y * dist.y)) <= 10)
		return (-1);
	return (0);
}
