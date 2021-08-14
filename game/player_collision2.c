/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_collision2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 15:46:17 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/14 18:12:05 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int	player_collision_with_enemies(t_doom *doom)
{
	t_enemy	*enemy;
	int		dx;
	int		dy;
	int		ec;
	int		distance;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return (0);
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		dx = (int)doom->mdl->player.x - enemy->x;
		dy = (int)doom->mdl->player.y - enemy->y;
		distance = sqrt(dx * dx + dy * dy);
		if ((distance < 10 + 10) && enemy->hp.cur > 0)
			return (-1);
		enemy = enemy->next;
	}
	return (0);
}

int	player_collision_with_pickup(t_doom *doom, t_pickup *pickup)
{
	int		dx;
	int		dy;
	int		distance;

	dx = (int)doom->mdl->player.x - pickup->loc.x;
	dy = (int)doom->mdl->player.y - pickup->loc.y;
	distance = sqrt(dx * dx + dy * dy);
	if ((distance < 10 + PICKING_RADIUS))
		return (-1);
	return (0);
}

t_coord	player_check_segment(t_doom *doom, t_enemy *enemy)
{
	t_coord	dist;
	t_coord	closest;
	double	dot;
	double	len;

	dist.x = doom->mdl->player.x - doom->mdl->player.bullet_pos.x;
	dist.y = doom->mdl->player.y - doom->mdl->player.bullet_pos.y;
	len = sqrt((dist.x * dist.x) + (dist.y * dist.y));
	dot = (((enemy->x - doom->mdl->player.x) * (doom->mdl->player.bullet_pos.x
					- doom->mdl->player.x)) + ((enemy->y - doom->mdl->player.y)
				* (doom->mdl->player.bullet_pos.y - doom->mdl->player.y)))
		/ pow(len, 2);
	closest.x = doom->mdl->player.x + (dot
			* (doom->mdl->player.bullet_pos.x - doom->mdl->player.x));
	closest.y = doom->mdl->player.y + (dot
			* (doom->mdl->player.bullet_pos.y - doom->mdl->player.y));
	return (closest);
}

int	is_point_circle(t_enemy *enemy, t_doom *doom)
{
	if ((point_circle(doom->mdl->player.x, doom->mdl->player.y,
				enemy->x, enemy->y) || point_circle(
				doom->mdl->player.bullet_pos.x, doom->mdl->player.bullet_pos.y,
				enemy->x, enemy->y)))
		return (1);
	return (0);
}

int	player_check_distance(t_enemy *enemy, t_coord closest)
{
	t_coord	dist;

	dist.x = closest.x - enemy->x;
	dist.y = closest.y - enemy->y;
	if (sqrt((dist.x * dist.x) + (dist.y * dist.y)) <= 10)
		return (1);
	return (0);
}
