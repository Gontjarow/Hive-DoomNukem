/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 15:08:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 19:14:14 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	animate_ranged_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = {0};

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_front_idle;
		frames[1] = doom->sprites->txt_ranged_death[0];
	}
	if (enemy->stun_time <= 0)
		enemy->anim.done = IDLE;
	enemy->active_sprite = frames[1];
}

void	animate_melee_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = {0};

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_front_idle;
		frames[1] = doom->sprites->txt_melee_death[0];
	}
	if (enemy->stun_time <= 0)
		enemy->anim.done = IDLE;
	enemy->active_sprite = frames[1];
}

void	animate_boss_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = {0};

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_front_idle;
		frames[1] = doom->sprites->txt_boss_death[0];
	}
	if (enemy->stun_time <= 0)
		enemy->anim.done = IDLE;
	enemy->active_sprite = frames[1];
}

void	animate_death(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == RANGED)
		animate_ranged_death(enemy, doom);
	else if (enemy->ai.type_id == MELEE)
		animate_melee_death(enemy, doom);
	else
		animate_boss_death(enemy, doom);
}

void	animation_switch(t_enemy *enemy, t_doom *doom)
{
	if (enemy->anim.dead)
		return ;
	if (enemy->anim.done == DEATH || enemy->hp.cur == 0)
		animate_death(enemy, doom);
	else if (enemy->anim.done == MOVE)
		animate_walk(enemy, doom);
	else if (enemy->anim.done == ATTACK)
		animate_attack(enemy, doom);
	else if (enemy->anim.done == HURT)
		animate_hurt(enemy, doom);
	else if (enemy->anim.done == IDLE)
		animate_idle(enemy, doom);
}
