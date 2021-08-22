/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_boss.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:02:25 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 18:38:29 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	animate_boss_back_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[6] = {0};
	static int			cd = 0;

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_back_walk[0];
		frames[1] = doom->sprites->txt_boss_back_walk[1];
		frames[2] = doom->sprites->txt_boss_back_walk[2];
		frames[3] = doom->sprites->txt_boss_back_walk[3];
		frames[4] = doom->sprites->txt_boss_back_walk[4];
		frames[5] = doom->sprites->txt_boss_back_walk[5];
	}
	if (enemy->anim_phase > 5)
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

void	animate_boss_front_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[6] = {0};
	static int			cd = 0;

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_front_walk[0];
		frames[1] = doom->sprites->txt_boss_front_walk[1];
		frames[2] = doom->sprites->txt_boss_front_walk[2];
		frames[3] = doom->sprites->txt_boss_front_walk[3];
		frames[4] = doom->sprites->txt_boss_front_walk[4];
		frames[5] = doom->sprites->txt_boss_front_walk[5];
	}
	if (enemy->anim_phase > 5)
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

void	animate_boss_front_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[3] = {0};
	static int			cd = 0;

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_front_idle;
		frames[1] = doom->sprites->txt_boss_front_attack[0];
		frames[2] = doom->sprites->txt_boss_front_attack[1];
	}
	if (enemy->anim_phase > 2)
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

static void	assign_frames(SDL_Surface *frames[6], t_enemy *enemy, t_doom *doom)
{
	if (enemy->anim.orient == LEFT)
	{
		frames[0] = doom->sprites->txt_boss_left_idle;
		frames[1] = doom->sprites->txt_boss_left_attack[0];
		frames[2] = doom->sprites->txt_boss_left_attack[1];
	}
	else
	{
		frames[0] = doom->sprites->txt_boss_right_idle;
		frames[1] = doom->sprites->txt_boss_right_attack[0];
		frames[2] = doom->sprites->txt_boss_right_attack[1];
	}
}

void	animate_boss_side_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[3] = {0};
	static int			last_orient = -1;
	static int			cd = 0;

	if (enemy->anim.orient != last_orient)
	{
		assign_frames(frames, enemy, doom);
		last_orient = enemy->anim.orient;
	}
	if (enemy->anim_phase > 2)
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
