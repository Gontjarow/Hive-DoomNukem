/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_melee.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:01:21 by msuarez-          #+#    #+#             */
/*   Updated: 2021/05/08 20:51:32 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		animate_melee_back_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[6] = { 0 };
	static int			cd = 0;

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_back_walk[0];
		frames[1] = doom->sprites->txt_melee_back_walk[1];
		frames[2] = doom->sprites->txt_melee_back_walk[2];
		frames[3] = doom->sprites->txt_melee_back_walk[3];
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

void		animate_melee_front_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };
	static int			cd = 0;

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_front_walk[0];
		frames[1] = doom->sprites->txt_melee_front_walk[1];
		frames[2] = doom->sprites->txt_melee_front_walk[2];
		frames[3] = doom->sprites->txt_melee_front_walk[3];
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

void		animate_melee_side_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };
	static int			cd = 0;

	if (frames[0] == 0)
	{
		if (enemy->anim.orient == LEFT)
		{
			frames[0] = doom->sprites->txt_melee_left_walk[0];
			frames[1] = doom->sprites->txt_melee_left_walk[1];
			frames[2] = doom->sprites->txt_melee_left_walk[2];
			frames[3] = doom->sprites->txt_melee_left_walk[3];
		}
		else
		{
			frames[0] = doom->sprites->txt_melee_right_walk[0];
			frames[1] = doom->sprites->txt_melee_right_walk[1];
			frames[2] = doom->sprites->txt_melee_right_walk[2];
			frames[3] = doom->sprites->txt_melee_right_walk[3];
		}
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

void		animate_melee_death(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[6] = { 0 };
	static int			cd = 0;

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_death[0];
		frames[1] = doom->sprites->txt_melee_death[1];
		frames[2] = doom->sprites->txt_melee_death[2];
		frames[3] = doom->sprites->txt_melee_death[3];
		frames[4] = doom->sprites->txt_melee_death[4];
		frames[5] = doom->sprites->txt_melee_death[5];
	}
	if (enemy->anim_phase > 5)
	{
		enemy->anim.dead = 1;
		enemy->anim_phase = 5;
	}
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}

void		animate_melee_front_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };
	static int			cd = 0;

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_front_idle;
		frames[1] = doom->sprites->txt_melee_front_attack[0];
		frames[2] = doom->sprites->txt_melee_front_attack[1];
		frames[3] = doom->sprites->txt_melee_front_attack[2];
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

void		animate_melee_side_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };
	static int			last_orient = -1;
	static int			cd = 0;

	if (enemy->anim.orient != last_orient)
	{
		if (enemy->anim.orient == LEFT)
		{
			frames[0] = doom->sprites->txt_melee_left_idle;
			frames[1] = doom->sprites->txt_melee_left_attack[0];
			frames[2] = doom->sprites->txt_melee_left_attack[1];
			frames[3] = doom->sprites->txt_melee_left_attack[2];
		}
		else
		{
			frames[0] = doom->sprites->txt_melee_right_idle;
			frames[1] = doom->sprites->txt_melee_right_attack[0];
			frames[2] = doom->sprites->txt_melee_right_attack[1];
			frames[3] = doom->sprites->txt_melee_right_attack[2];
		}
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
