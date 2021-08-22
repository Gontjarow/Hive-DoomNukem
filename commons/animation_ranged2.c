/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_ranged2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 18:50:35 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 18:53:30 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	assign_frame(SDL_Surface *frames[2], t_enemy *enemy, t_doom *doom)
{
	if (enemy->anim.orient == LEFT)
	{
		frames[0] = doom->sprites->txt_ranged_left_idle;
		frames[1] = doom->sprites->txt_ranged_left_attack;
	}
	else
	{
		frames[0] = doom->sprites->txt_ranged_right_idle;
		frames[1] = doom->sprites->txt_ranged_right_attack;
	}
}

void	animate_ranged_side_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = {0};
	static int			last_orient = -1;
	static int			cd = 0;

	if (enemy->anim.orient != last_orient)
	{
		assign_frame(frames, enemy, doom);
		last_orient = enemy->anim.orient;
	}
	if (enemy->anim_phase > 1)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}

static void	assign_frames(SDL_Surface *frames[4], t_enemy *enemy, t_doom *doom)
{
	if (enemy->anim.orient == LEFT)
	{
		frames[0] = doom->sprites->txt_ranged_left_walk[0];
		frames[1] = doom->sprites->txt_ranged_left_walk[1];
		frames[2] = doom->sprites->txt_ranged_left_walk[2];
		frames[3] = doom->sprites->txt_ranged_left_walk[3];
	}
	else
	{
		frames[0] = doom->sprites->txt_ranged_right_walk[0];
		frames[1] = doom->sprites->txt_ranged_right_walk[1];
		frames[2] = doom->sprites->txt_ranged_right_walk[2];
		frames[3] = doom->sprites->txt_ranged_right_walk[3];
	}
}

void	animate_ranged_side_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[4] = {0};
	static int			last_orient = -1;
	static int			cd = 0;

	if (enemy->anim.orient != last_orient)
	{
		assign_frames(frames, enemy, doom);
		last_orient = enemy->anim.orient;
	}
	if (enemy->anim_phase > 3)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}
