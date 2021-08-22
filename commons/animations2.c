/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 18:59:13 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 19:18:22 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	animate_idle_boss(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == BOSS)
	{
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			enemy->active_sprite = doom->sprites->txt_boss_front_idle;
		else if (enemy->anim.orient == BACK)
			enemy->active_sprite = doom->sprites->txt_boss_back_idle;
		else if (enemy->anim.orient == LEFT)
			enemy->active_sprite = doom->sprites->txt_boss_left_idle;
		else
			enemy->active_sprite = doom->sprites->txt_boss_right_idle;
	}
}

void	animate_idle(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == RANGED)
	{
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			enemy->active_sprite = doom->sprites->txt_ranged_front_idle;
		else if (enemy->anim.orient == BACK)
			enemy->active_sprite = doom->sprites->txt_ranged_back_idle;
		else if (enemy->anim.orient == LEFT)
			enemy->active_sprite = doom->sprites->txt_ranged_left_idle;
		else
			enemy->active_sprite = doom->sprites->txt_ranged_right_idle;
	}
	else if (enemy->ai.type_id == MELEE)
	{
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			enemy->active_sprite = doom->sprites->txt_melee_front_idle;
		else if (enemy->anim.orient == BACK)
			enemy->active_sprite = doom->sprites->txt_melee_back_idle;
		else if (enemy->anim.orient == LEFT)
			enemy->active_sprite = doom->sprites->txt_melee_left_idle;
		else
			enemy->active_sprite = doom->sprites->txt_melee_right_idle;
	}
	animate_idle_boss(enemy, doom);
}

static void	animate_walk_boss(t_enemy *enemy, t_doom *doom)
{
	if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
		animate_boss_front_walk(enemy, doom);
	else if (enemy->anim.orient == BACK)
		animate_boss_back_walk(enemy, doom);
	else
		animate_boss_side_walk(enemy, doom);
}

void	animate_walk(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == RANGED)
	{
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			animate_ranged_front_walk(enemy, doom);
		else if (enemy->anim.orient == BACK)
			animate_ranged_back_walk(enemy, doom);
		else
			animate_ranged_side_walk(enemy, doom);
	}
	else if (enemy->ai.type_id == MELEE)
	{
		if (enemy->anim.orient == FRONT)
			animate_melee_front_walk(enemy, doom);
		else if (enemy->anim.orient == BACK)
			animate_melee_back_walk(enemy, doom);
		else
			animate_melee_side_walk(enemy, doom);
	}
	else
	{
		animate_walk_boss(enemy, doom);
	}
}

void	animate_hurt(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == RANGED)
		animate_ranged_hurt(enemy, doom);
	else if (enemy->ai.type_id == MELEE)
		animate_melee_hurt(enemy, doom);
	else
		animate_boss_hurt(enemy, doom);
}
