/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_ranged.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:00:16 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/05 18:34:46 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		animate_ranged_back_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_back_walk[0];
		frames[1] = doom->sprites->txt_ranged_back_walk[1];
		frames[2] = doom->sprites->txt_ranged_back_walk[2];
		frames[3] = doom->sprites->txt_ranged_back_walk[3];
	}
	if (enemy->anim_phase > 3)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_ranged_front_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_front_walk[0];
		frames[1] = doom->sprites->txt_ranged_front_walk[1];
		frames[2] = doom->sprites->txt_ranged_front_walk[2];
		frames[3] = doom->sprites->txt_ranged_front_walk[3];
	}
	if (enemy->anim_phase > 3)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_ranged_side_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };
	static int			last_orient = -1;

	if (enemy->anim.orient != last_orient)
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
		last_orient = enemy->anim.orient;
	}
	if (enemy->anim_phase > 3)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_ranged_death(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[7] = { 0 };
	
	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_death[0];
		frames[1] = doom->sprites->txt_ranged_death[1];
		frames[2] = doom->sprites->txt_ranged_death[2];
		frames[3] = doom->sprites->txt_ranged_death[3];
		frames[4] = doom->sprites->txt_ranged_death[4];
		frames[5] = doom->sprites->txt_ranged_death[5];
		frames[6] = doom->sprites->txt_ranged_death[6];
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
	if (enemy->anim_phase > 6)
		enemy->anim.done = IDLE;
}

void		animate_ranged_front_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_front_idle;
		frames[1] = doom->sprites->txt_ranged_front_attack;
	}
	if (enemy->anim_phase > 1)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_ranged_side_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[2] = { 0 };
	static int			last_orient = -1;

	if (enemy->anim.orient != last_orient)
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
		last_orient = enemy->anim.orient;
	}
	if (enemy->anim_phase > 1)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}
