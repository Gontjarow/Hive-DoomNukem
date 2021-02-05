/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:20:42 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/05 20:41:47 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		ai_ranged(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.min_dis = 200;
	enemy->ai.max_dis = 400;
	enemy->ai.mov_speed = 10;
	enemy->hp.cur = 100;
	enemy->ai.aggro = INACTIVE;
	enemy->ai.dmg = 10;
}

void		ai_melee(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.min_dis = 40;
	enemy->ai.max_dis = 300;
	enemy->ai.mov_speed = 15;
	enemy->hp.cur = 50;
	enemy->ai.aggro = INACTIVE;
	enemy->ai.dmg = 20;
}

void		ai_boss(t_doom *doom, t_enemy *enemy)
{
	enemy->ai.min_dis = 70;
	enemy->ai.max_dis = 200;
	enemy->ai.mov_speed = 5;
	enemy->hp.cur = 1000;
	enemy->ai.aggro = INACTIVE;
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
		if (enemy->ai.type_id == RANGED)
			ai_ranged(doom, enemy);
		else if (enemy->ai.type_id == MELEE)
			ai_melee(doom, enemy);
		else if (enemy->ai.type_id == BOSS)
			ai_boss(doom, enemy);
		enemy = enemy->next;
	}
}

void		check_aggro(t_doom *doom, t_enemy *enemy)
{
	int dist;

	if (check_location(doom, doom->mdl->player.x, doom->mdl->player.y)
		== check_location(doom, enemy->x, enemy->y))
	{
		dist = calc_distance(enemy, doom);
		if (dist <= enemy->ai.max_dis)
			enemy->ai.aggro = ACTIVE;
	}
}

static void		check_sprite_orient(t_doom *doom, t_enemy *enemy)
{
	t_xy	delta;
	float	angle;

	if (enemy->hp.cur > 0)
	{
		delta.y = enemy->y - doom->mdl->player.y;
		delta.x = enemy->x - doom->mdl->player.x;
		delta = vec2_norm(delta);
		angle = (atan2(delta.y, delta.x) * 180 / M_PI) + doom->mdl->player.rot;
		printf("Angle between: %0.2f\n", angle);
		if (angle >= -45 && angle <= 45)
		{
			enemy->anim.orient = FRONT;
		}
		else if (angle > 30 && angle <= 135)
		{
			enemy->anim.orient = RIGHT;
		}
		else if (angle < -45 && angle >= -135)
		{
			enemy->anim.orient = LEFT;
		}
		else
		{
			enemy->anim.orient = BACK;
		}
	}
}

void		handle_enemy_ai(t_doom *doom)
{
	int		ec;
	t_enemy *enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		enemy_update_cooldown(doom, enemy);
		// check_sprite_orient(doom, enemy);		// WIP
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
