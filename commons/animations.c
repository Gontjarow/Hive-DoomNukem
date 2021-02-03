/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 15:08:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/03 20:50:43 by msuarez-         ###   ########.fr       */
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
	if (enemy->stun_time == 0)
		enemy->anim.done = IDLE;
	enemy->active_sprite = frames[1];
}

void			animate_melee_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_front_idle;
		frames[1] = doom->sprites->txt_melee_death[0];
	}
	if (enemy->stun_time == 0)
		enemy->anim.done = IDLE;
	enemy->active_sprite = frames[1];
}

void			animate_boss_hurt(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_front_idle;
		frames[1] = doom->sprites->txt_boss_death[0];
	}
    if (enemy->stun_time == 0)
        enemy->anim.done = IDLE;
    enemy->active_sprite = frames[1];
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
	// else if (enemy->ai.type_id == 0 && enemy->anim.done == IDLE && enemy->anim.orient == LEFT)
	// 	enemy->active_sprite = doom->sprites->txt_ranged_side_idle;
	// else if (enemy->ai.type_id == 1 && enemy->anim.done == IDLE && enemy->anim.orient == LEFT)
	// 	enemy->active_sprite = doom->sprites->txt_melee_side_idle;
	// else if (enemy->ai.type_id == 2 && enemy->anim.done == IDLE && enemy->anim.orient == LEFT)
	// 	enemy->active_sprite = doom->sprites->txt_boss_side_idle;
}

void			animation_switch(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == 0 && enemy->anim.done == DEATH)
		animate_ranged_death(enemy, doom);
	else if (enemy->ai.type_id == 1 && enemy->anim.done == DEATH)
		animate_melee_death(enemy, doom);
	else if (enemy->ai.type_id == 2 && enemy->anim.done == DEATH)
		animate_boss_death(enemy, doom);
	else if (enemy->ai.type_id == 0 && enemy->anim.done == MOVE && enemy->anim.orient == FRONT)
		animate_ranged_front_walk(enemy, doom);
	else if (enemy->ai.type_id == 1 && enemy->anim.done == MOVE && enemy->anim.orient == FRONT)
		animate_melee_front_walk(enemy, doom);
	else if (enemy->ai.type_id == 2 && enemy->anim.done == MOVE && enemy->anim.orient == FRONT)
		animate_boss_front_walk(enemy, doom);
	// else if (enemy->ai.type_id == 0 && enemy->anim.done == MOVE && enemy->anim.orient == LEFT)
	// 	animate_ranged_side_walk(enemy, doom);
	// else if (enemy->ai.type_id == 1 && enemy->anim.done == MOVE && enemy->anim.orient == LEFT)
	// 	animate_melee_side_walk(enemy, doom);
	// else if (enemy->ai.type_id == 2 && enemy->anim.done == MOVE && enemy->anim.orient == LEFT)
	// 	animate_boss_side_walk(enemy, doom);
	// else if (enemy->ai.type_id == 0 && enemy->anim.done == MOVE && enemy->anim.orient == RIGHT)
	// 	animate_ranged_side_walk(enemy, doom);
	// else if (enemy->ai.type_id == 1 && enemy->anim.done == MOVE && enemy->anim.orient == RIGHT)
	// 	animate_melee_side_walk(enemy, doom);
	// else if (enemy->ai.type_id == 2 && enemy->anim.done == MOVE && enemy->anim.orient == RIGHT)
	// 	animate_boss_side_walk(enemy, doom);
	// else if (enemy->ai.type_id == 0 && enemy->anim.done == MOVE && enemy->anim.orient == BACK)
	// 	animate_ranged_back_walk(enemy, doom);
	// else if (enemy->ai.type_id == 1 && enemy->anim.done == MOVE && enemy->anim.orient == BACK)
	// 	animate_melee_back_walk(enemy, doom);
	// else if (enemy->ai.type_id == 2 && enemy->anim.done == MOVE && enemy->anim.orient == BACK)
	// 	animate_boss_back_walk(enemy, doom);
	animation_switch2(enemy, doom);
}
