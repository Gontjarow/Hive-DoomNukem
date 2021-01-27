/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 15:08:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/27 15:50:09 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void			animate_ranged_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_front_idle;
		frames[1] = doom->sprites->txt_ranged_death[0];
	}
	if (enemy->anim_phase > 1)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void			animate_melee_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_front_idle;
		frames[1] = doom->sprites->txt_melee_death[0];
	}
	if (enemy->anim_phase > 1)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void			animate_boss_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_front_idle;
		frames[1] = doom->sprites->txt_boss_death[0];
	}
	if (enemy->anim_phase > 1)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

static void		animation_switch2(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == 0 && enemy->anim.done == ATTACK)
		animate_ranged_front_attack(enemy, doom);
	else if (enemy->ai.type_id == 1 && enemy->anim.done == ATTACK)
		animate_melee_front_attack(enemy, doom);
	else if (enemy->ai.type_id == 2 && enemy->anim.done == ATTACK)
		animate_boss_front_attack(enemy, doom);
	else if (enemy->ai.type_id == 0 && enemy->anim.done == HURT)
		animate_ranged_hurt(enemy, doom);
	else if (enemy->ai.type_id == 1 && enemy->anim.done == HURT)
		animate_melee_hurt(enemy, doom);
	else if (enemy->ai.type_id == 2 && enemy->anim.done == HURT)
		animate_boss_hurt(enemy, doom);
}

void			animation_switch(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == 0 && enemy->anim.done == DEATH)
		animate_ranged_death(enemy, doom);
	else if (enemy->ai.type_id == 1 && enemy->anim.done == DEATH)
		animate_melee_death(enemy, doom);
	else if (enemy->ai.type_id == 2 && enemy->anim.done == DEATH)
		animate_boss_death(enemy, doom);
	else if (enemy->ai.type_id == 0 && enemy->anim.done == MOVE)
		animate_ranged_front_walk(enemy, doom);
	else if (enemy->ai.type_id == 1 && enemy->anim.done == MOVE)
		animate_melee_front_walk(enemy, doom);
	else if (enemy->ai.type_id == 2 && enemy->anim.done == MOVE)
		animate_boss_front_walk(enemy, doom);
	animation_switch2(enemy, doom);
}
