/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:20:42 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/14 16:03:23 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	ai_ranged(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.mcEnemyDeath = doom->sounds->mcEnemyDeath;
	enemy->ai.mcEnemyShoot = doom->sounds->mcLaserShot;
	enemy->ai.min_dis = 200;
	enemy->ai.max_dis = 400;
	enemy->ai.mov_speed = 300;
	enemy->ai.aggro = INACTIVE;
	enemy->ai.dmg = 10;
}

void	ai_melee(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.mcEnemyDeath = doom->sounds->mcDogDying;
	enemy->ai.mcEnemyShoot = doom->sounds->mcDogShot;
	enemy->ai.min_dis = 45;
	enemy->ai.max_dis = 300;
	enemy->ai.mov_speed = 500;
	enemy->ai.aggro = INACTIVE;
	enemy->ai.dmg = 20;
}

void	ai_boss(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.mcEnemyDeath = doom->sounds->mcEnemyDeath;
	enemy->ai.mcEnemyShoot = doom->sounds->mcLaserShot;
	enemy->ai.min_dis = 200;
	enemy->ai.max_dis = 400;
	enemy->ai.mov_speed = 400;
	enemy->ai.aggro = INACTIVE;
	enemy->ai.dmg = 15;
}

void	ai_assignment(t_doom *doom)
{
	int		ec;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (enemy->ai.type_id == RANGED)
			ai_ranged(doom, enemy);
		else if (enemy->ai.type_id == MELEE)
			ai_melee(doom, enemy);
		else if (enemy->ai.type_id == BOSS)
			ai_boss(doom, enemy);
		enemy = enemy->next;
	}
}

void	handle_enemy_ai(t_doom *doom)
{
	int		ec;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		enemy_update_cooldown(doom, enemy);
		check_sprite_orient(doom, enemy);
		animation_switch(enemy, doom);
		check_aggro(doom, enemy);
		if (enemy->ai.aggro == ACTIVE)
		{
			rotate_enemy_towards_player(doom, enemy);
			move_enemy_towards_player(doom, enemy);
			handle_enemy_shooting(doom, enemy);
		}
		enemy = enemy->next;
	}
}
