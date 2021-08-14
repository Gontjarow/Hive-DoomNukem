/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_collision2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:01:29 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/14 16:12:53 by msuarez-         ###   ########.fr       */
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

int	enemy_collision(t_doom *doom, t_enemy *enemy)
{
	if (enemy_collision_with_enemies(doom, enemy) == -1)
		return (-1);
	if (enemy_collision_with_player(doom, enemy) == -1)
		return (-1);
	return (0);
}
