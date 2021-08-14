/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/14 17:58:46 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int	point_circle(double px, double py, double cx, double cy)
{
	double	dist_x;
	double	dist_y;
	double	dist;

	dist_x = px - cx;
	dist_y = py - cy;
	dist = sqrt((dist_x * dist_x) + (dist_y * dist_y));
	if (dist <= 10)
		return (1);
	return (0);
}

double	dist(double px, double py, double x1, double y1)
{
	double	distance;

	distance = sqrt(((px - x1) * (px - x1)) + ((py - y1) * (py - y1)));
	return (distance);
}

int	line_point(t_doom *doom, t_coord p)
{
	double	d1;
	double	d2;
	double	len;
	double	buffer;

	d1 = dist(p.x, p.y, doom->mdl->player.x, doom->mdl->player.y);
	d2 = dist(p.x, p.y, doom->mdl->player.bullet_pos.x,
			doom->mdl->player.bullet_pos.y);
	len = dist(doom->mdl->player.x, doom->mdl->player.y,
			doom->mdl->player.bullet_pos.x, doom->mdl->player.bullet_pos.y);
	buffer = 0.001;
	if (d1 + d2 >= len - buffer && d1 + d2 <= len + buffer)
		return (1);
	return (0);
}

int	check_hit(t_doom *doom)
{
	int		ec;
	t_coord	closest;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return (-1);
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (is_point_circle(enemy, doom) && enemy->hp.cur > 0)
			return (enemy->id);
		closest = player_check_segment(doom, enemy);
		if (!line_point(doom, closest))
		{
			enemy = enemy->next;
			continue ;
		}
		if (player_check_distance(enemy, closest) && enemy->hp.cur > 0)
			return (enemy->id);
		enemy = enemy->next;
	}
	return (-1);
}

int	player_collision_with_effects(t_doom *doom, t_effect *effect)
{
	int		dx;
	int		dy;
	int		distance;

	dx = (int)doom->mdl->player.x - effect->loc.x;
	dy = (int)doom->mdl->player.y - effect->loc.y;
	distance = sqrt(dx * dx + dy * dy);
	if ((distance < 20) && effect->type_id == EFFECT_EXIT)
		return (-1);
	else if ((distance < 80) && effect->type_id != EFFECT_EXIT)
		return (-1);
	return (0);
}
