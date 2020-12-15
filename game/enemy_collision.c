/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_collision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:22:00 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/15 17:42:07 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int			enemy_line_point(t_doom *doom, t_enemy *enemy, t_coord p)
{
	double	d1;
	double	d2;
	double	len;
	double	buffer;

	d1 = dist(p.x, p.y, enemy->x, enemy->y);
	d2 = dist(p.x, p.y, enemy->bullet_pos.x, enemy->bullet_pos.y);
	len = dist(enemy->x, enemy->y, enemy->bullet_pos.x, enemy->bullet_pos.y);
	buffer = 0.001;
	if (d1 + d2 >= len - buffer && d1 + d2 <= len + buffer)
		return (1);
	return (0);
}

t_coord		enemy_check_segment(t_doom *doom, t_enemy *enemy)
{
	double		len;
	double		dot;
	t_coord		dist;
	t_coord		closest;

	dist.x = enemy->x - enemy->bullet_pos.x;
	dist.y = enemy->y - enemy->bullet_pos.y;
	len = sqrt((dist.x * dist.x) + (dist.y * dist.y));
	dot = (((doom->mdl->player.x - enemy->x) *
		(enemy->bullet_pos.x - enemy->x)) +
			((doom->mdl->player.y - enemy->y) *
				(enemy->bullet_pos.y - enemy->y))) / pow(len, 2);
	closest.x = enemy->x + (dot * (enemy->bullet_pos.x - enemy->x));
	closest.y = enemy->y + (dot * (enemy->bullet_pos.y - enemy->y));
	return (closest);
}

int			enemy_check_distance(t_doom *doom, t_coord closest)
{
	t_coord dist;

	dist.x = closest.x - doom->mdl->player.x;
	dist.y = closest.y - doom->mdl->player.y;
	if (sqrt((dist.x * dist.x) + (dist.y * dist.y)) <= 10)
		return (1);
	return (0);
}

int			check_hit_on_player(t_doom *doom, t_enemy *enemy)
{
	int		inside1;
	int		inside2;
	t_coord	closest;

	inside1 = point_circle(enemy->x, enemy->y, doom->mdl->player.x,
		doom->mdl->player.y);
	inside2 = point_circle(enemy->bullet_pos.x,
			enemy->bullet_pos.y, doom->mdl->player.x, doom->mdl->player.y);
	if ((inside1 || inside2)) 
		return (1);
	closest = enemy_check_segment(doom, enemy);
	if (!enemy_line_point(doom, enemy, closest))
		return (0);
	if (enemy_check_distance(doom, closest) <= 10)
		return (1);
	return (0);
}
