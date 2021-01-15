/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_movement2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:34:32 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/15 15:48:53 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int			dist_please(t_enemy *enemy, t_doom *doom)
{
	t_coord	dist;
	int		distance;

	dist.x = enemy->x - doom->mdl->player.x;
	dist.y = enemy->y - doom->mdl->player.y;
	distance = sqrt(dist.x * dist.x + dist.y * dist.y);
	return (distance);
}

static void	move_enemy(t_doom *doom, t_enemy *enemy, t_point old)
{
	t_coord p;

	enemy->x += 5 * -cos(deg_to_rad(enemy->rot));
	enemy->y += 5 * -sin(deg_to_rad(enemy->rot));
	p.x = enemy->x + 5 * -cos(deg_to_rad(enemy->rot));
	p.y = enemy->y + 5 * -sin(deg_to_rad(enemy->rot));
	enemy->tail.x = p.x;
	enemy->tail.y = p.y;
	if (check_location(doom, enemy->x, enemy->y) == -1 ||
		check_location(doom, enemy->x, enemy->y) == UINT_ERROR_CONSTANT ||
		enemy_collision(doom, enemy) == -1)
	{
		enemy->x = old.x;
		enemy->y = old.y;
	}
}

int			calc_dist(t_enemy *enemy, t_doom *doom, t_point old)
{
	int		distance;

	distance = dist_please(enemy, doom);
	if (distance > 70 && enemy->aggro == 1)
		move_enemy(doom, enemy, old);
	else if ((distance >= 70 && distance <= 200))
	{
		enemy->aggro = 1;
		move_enemy(doom, enemy, old);
	}
	return (distance);
}
