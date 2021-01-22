/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_movement.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:18:34 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/19 17:43:15 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void		rotate_left(t_doom *doom, t_enemy *enemy)
{
	int		x;
	int		y;
	double	rad;

	enemy->rot -= 5;
	if (enemy->rot < 0)
		enemy->rot = 359;
	rad = deg_to_rad(enemy->rot);
	x = enemy->x + 10 * -cos(rad);
	y = enemy->y + 10 * -sin(rad);
	enemy->tail.x = x;
	enemy->tail.y = y;
}

static void		rotate_right(t_doom *doom, t_enemy *enemy)
{
	int		x;
	int		y;
	double	rad;

	enemy->rot += 5;
	if (enemy->rot >= 360)
		enemy->rot = 0;
	rad = deg_to_rad(enemy->rot);
	x = enemy->x + 10 * -cos(rad);
	y = enemy->y + 10 * -sin(rad);
	enemy->tail.x = x;
	enemy->tail.y = y;
}

static int		orientation(t_doom *doom, t_enemy *enemy, int x, int y)
{
	int		val;

	val = (y - enemy->y) * (doom->mdl->player.x - x) -
		(x - enemy->x) * (doom->mdl->player.y - y);
	if (val == 0)
		return (0);
	return ((val > 0) ? 1 : 2);
}

void			rotate_enemy_towards_player(t_doom *doom)
{
	t_enemy	*enemy;
	t_point	p;
	int		ec;
	int		orient;
	double	rad;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (enemy->hp.cur > 0)
		{
			rad = deg_to_rad(enemy->rot);
			p.x = enemy->x + 10 * -cos(rad);
			p.y = enemy->y + 10 * -sin(rad);
			orient = orientation(doom, enemy, p.x, p.y);
			if (orient == 1)
				rotate_left(doom, enemy);
			else if (orient == 2)
				rotate_right(doom, enemy);
		}
		enemy = enemy->next;
	}
}

void			move_enemy_towards_player(t_doom *doom)
{
	t_enemy	*enemy;
	int		ec;
	t_point	old;
	t_coord	dist;
	int		distance;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		old.x = enemy->x;
		old.y = enemy->y;
		if (check_location(doom, enemy->x, enemy->y) != -1
			&& enemy->hp.cur > 0)
			handle_enemy_movement(enemy, doom, old);
		enemy = enemy->next;
	}
}
