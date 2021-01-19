/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:20:42 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/19 17:40:37 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		ai_ranged(t_doom *doom, t_enemy *enemy)
{
	/*
		Set enemy shoot cd
		If aggro or distance >= 70 && distance <= 200
		- Move forward player at normal speed
		- Once distance < 70, start shooting player, dealing 5dmg per sec?
		- HP normal
	*/
	enemy->ai.min_dis = 70;
	enemy->ai.max_dis = 200;
	enemy->ai.mov_speed = 10;
	enemy->hp.cur = 100;
	enemy->ai.aggro = 0;
	enemy->ai.dmg = 20;
}

void		ai_melee(t_doom *doom, t_enemy *enemy)
{
	/*
		Set enemy shoot cd
		If aggro or distance >= 100 && distance <= 300 (or 200)
		- Move forward player at a higher speed than other enemies
		- Once collided, start dealing 5-10 dmg per sec? Prob more dmg than ranged
		- HP lower than ranged?
		- Players should definitely avoid letting them get near to survive
	*/
	enemy->ai.min_dis = 40;
	enemy->ai.max_dis = 300;
	enemy->ai.mov_speed = 15;
	enemy->hp.cur = 50;
	enemy->ai.aggro = 0;
	enemy->ai.dmg = 30;
}

void		ai_boss(t_doom *doom, t_enemy *enemy)
{
	/*
		Set enemy shoot cd
		If aggro or distance (*)
		- Move forward player at slower speed?
		- Once distance (*) or collided, start dealing dmg, lower than most but depending
		of what is its kind, we could decide a balanced dmg rate
		- HP Very big pool - gotta make the fight last longer than usual
	*/
	enemy->ai.min_dis = 70;
	enemy->ai.max_dis = 200;
	enemy->ai.mov_speed = 5;
	enemy->hp.cur = 1000;
	enemy->ai.aggro = 0;
	enemy->ai.dmg = 10;
}

void		ai_assignment(t_doom *doom)			// this must be done before or after load model?
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
}
