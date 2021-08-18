/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_collision2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:01:29 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 15:59:31 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static int	enemy_collision_with_player(t_doom *doom, t_enemy *enemy)
{
	int			dx;
	int			dy;
	int			distance;

	dx = enemy->x - (int)doom->mdl->player.x;
	dy = enemy->y - (int)doom->mdl->player.y;
	distance = sqrt(dx * dx + dy * dy);
	if ((distance < 10 + 10) && enemy->hp.cur > 0)
		return (-1);
	return (0);
}

static int	enemy_collision_with_enemies(t_doom *doom, t_enemy *enemy)
{
	t_enemy		*next_enemy;
	int			dx;
	int			dy;
	int			ec2;
	int			distance;

	ec2 = doom->mdl->enemy_count;
	next_enemy = doom->mdl->enemy_first;
	if (ec2 > 1)
	{
		while (ec2 - 1)
		{
			if (next_enemy->id == enemy->id && next_enemy->next != NULL)
				next_enemy = next_enemy->next;
			dx = next_enemy->x - enemy->x;
			dy = next_enemy->y - enemy->y;
			distance = sqrt(dx * dx + dy * dy);
			if ((distance < 10 + 10) && next_enemy->hp.cur > 0)
				return (-1);
			ec2--;
			next_enemy = next_enemy->next;
		}
	}
	return (0);
}

static t_coord	check_segment(t_point start, t_point end, t_enemy *enemy)
{
	t_coord	dist;
	t_coord	closest;
	double	dot;
	double	len;

	dist.x = start.x - end.x;
	dist.y = start.y - end.y;
	len = sqrt((dist.x * dist.x) + (dist.y * dist.y));
	dot = (((enemy->x - start.x) * (end.x
					- start.x)) + ((enemy->y - start.y)
				* (end.y - start.y)))
		/ pow(len, 2);
	closest.x = start.x + (dot
			* (end.x - start.x));
	closest.y = start.y + (dot
			* (end.y - start.y));
	return (closest);
}

static int	enemy_collision_with_portals(t_doom *doom, t_enemy *enemy)
{
	int		pc;
	t_coord	closest;
	t_coord	dist;
	t_wall	*portals;

	pc = doom->mdl->portal_count;
	if (pc == 0)
		return (0);
	portals = doom->mdl->portal_first;
	while (pc--)
	{
		if (portals->open == 0)
		{
			if ((point_circle(portals->start.x, portals->start.y,
						enemy->x, enemy->y) || point_circle(portals->end.x,
					portals->end.y, enemy->x, enemy->y)))
				return (-1);
			closest = check_segment(portals->start, portals->end, enemy);
			if (!line_point_portal(doom, closest, portals->start, portals->end))
			{
				portals = portals->next;
				continue ;
			}
			dist.x = closest.x - enemy->x;
			dist.y = closest.y - enemy->y;
			if (sqrt((dist.x * dist.x) + (dist.y * dist.y)) <= 10)
				return (-1);
		}
		portals = portals->next;
	}
	return (0);
}

int	enemy_collision(t_doom *doom, t_enemy *enemy)
{
	if (enemy_collision_with_enemies(doom, enemy) == -1)
		return (-1);
	if (enemy_collision_with_player(doom, enemy) == -1)
		return (-1);
	if (enemy_collision_with_portals(doom, enemy) == -1)
		return (-1);
	return (0);
}
