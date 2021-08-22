/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_boss2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 18:23:31 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 18:40:27 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	assign_frames(SDL_Surface *frames[6], t_doom *doom)
{
	frames[0] = doom->sprites->txt_boss_death[0];
	frames[1] = doom->sprites->txt_boss_death[1];
	frames[2] = doom->sprites->txt_boss_death[2];
	frames[3] = doom->sprites->txt_boss_death[3];
	frames[4] = doom->sprites->txt_boss_death[4];
	frames[5] = doom->sprites->txt_boss_death[5];
	frames[6] = doom->sprites->txt_boss_death[6];
	frames[7] = doom->sprites->txt_boss_death[7];
	frames[8] = doom->sprites->txt_boss_death[8];
}

void	animate_boss_death(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[10] = {0};
	static int			cd = 0;

	if (frames[0] == 0)
		assign_frames(frames, doom);
	if (enemy->anim_phase > 8)
	{
		enemy->anim.dead = 1;
		enemy->anim_phase = 8;
	}
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}

static void	assign_frame(SDL_Surface *frames[6], t_enemy *enemy, t_doom *doom)
{
	if (enemy->anim.orient == LEFT)
	{
		frames[0] = doom->sprites->txt_boss_left_walk[0];
		frames[1] = doom->sprites->txt_boss_left_walk[1];
		frames[2] = doom->sprites->txt_boss_left_walk[2];
		frames[3] = doom->sprites->txt_boss_left_walk[3];
		frames[4] = doom->sprites->txt_boss_left_walk[4];
		frames[5] = doom->sprites->txt_boss_left_walk[5];
	}
	else
	{
		frames[0] = doom->sprites->txt_boss_right_walk[0];
		frames[1] = doom->sprites->txt_boss_right_walk[1];
		frames[2] = doom->sprites->txt_boss_right_walk[2];
		frames[3] = doom->sprites->txt_boss_right_walk[3];
		frames[4] = doom->sprites->txt_boss_right_walk[4];
		frames[5] = doom->sprites->txt_boss_right_walk[5];
	}
}

void	animate_boss_side_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[6] = {0};
	static int			last_orient = -1;
	static int			cd = 0;

	if (enemy->anim.orient != last_orient)
	{
		assign_frame(frames, enemy, doom);
		last_orient = enemy->anim.orient;
	}
	if (enemy->anim_phase > 5)
	{
		enemy->anim.done = IDLE;
		enemy->anim_phase = 0;
	}
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}
