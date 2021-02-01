/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:20:42 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/01 18:36:40 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		ai_ranged(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.min_dis = 200;
	enemy->ai.max_dis = 400;
	enemy->ai.mov_speed = 10;
	enemy->hp.cur = 100;
	enemy->ai.aggro = 0;
	enemy->ai.dmg = 10;
}

void		ai_melee(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.min_dis = 40;
	enemy->ai.max_dis = 300;
	enemy->ai.mov_speed = 15;
	enemy->hp.cur = 50;
	enemy->ai.aggro = 0;
	enemy->ai.dmg = 20;
}

void		ai_boss(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.min_dis = 70;
	enemy->ai.max_dis = 200;
	enemy->ai.mov_speed = 5;
	enemy->hp.cur = 1000;
	enemy->ai.aggro = 0;
	enemy->ai.dmg = 5;
}

void		ai_assignment(t_doom *doom)
{
	int		ec;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (enemy->ai.type_id == 0)
			ai_ranged(doom, enemy);
		else if (enemy->ai.type_id == 1)
			ai_melee(doom, enemy);
		else if (enemy->ai.type_id == 2)
			ai_boss(doom, enemy);
		enemy = enemy->next;
	}
}

void		handle_enemy_ai(t_doom *doom)
{
	enemy_update_cooldown(doom);
	rotate_enemy_towards_player(doom);
	move_enemy_towards_player(doom);
	handle_enemy_shooting(doom);
	handle_enemy_animation(doom);
}
