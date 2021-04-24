/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_movement2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:34:32 by msuarez-          #+#    #+#             */
/*   Updated: 2021/04/03 19:47:06 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int			calc_distance(t_enemy *enemy, t_doom *doom)
{
	t_coord	dist;
	int		distance;

	dist.x = enemy->x - doom->mdl->player.x;
	dist.y = enemy->y - doom->mdl->player.y;
	distance = sqrt(dist.x * dist.x + dist.y * dist.y);
	return (distance);
}

void		handle_enemy_movement(t_enemy *enemy, t_doom *doom, t_point old)
{
	t_coord p;

	enemy->x += (enemy->ai.mov_speed * doom->delta_time) * -cos(deg_to_rad(enemy->rot));
	enemy->y += (enemy->ai.mov_speed * doom->delta_time) * -sin(deg_to_rad(enemy->rot));
	p.x = enemy->x + enemy->ai.mov_speed * doom->delta_time * -cos(deg_to_rad(enemy->rot));
	p.y = enemy->y + enemy->ai.mov_speed * doom->delta_time * -sin(deg_to_rad(enemy->rot));
	enemy->tail.x = p.x;
	enemy->tail.y = p.y;
	if (check_location(doom, enemy->x, enemy->y) == -1 ||
		check_location(doom, enemy->x, enemy->y) == UINT_ERROR_CONSTANT ||
		enemy_collision(doom, enemy) == -1)
	{
		enemy->x = old.x;
		enemy->y = old.y;
	}
	enemy->anim.done = MOVE;
}
