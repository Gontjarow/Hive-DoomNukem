/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_melee.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:01:21 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/27 15:31:14 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		animate_melee_front_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };

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
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_melee_side_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_side_walk[0];
		frames[1] = doom->sprites->txt_melee_side_walk[1];
		frames[2] = doom->sprites->txt_melee_side_walk[2];
		frames[3] = doom->sprites->txt_melee_side_walk[3];
	}
	if (enemy->anim_phase > 3)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_melee_death(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[6] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_death[0];
		frames[1] = doom->sprites->txt_melee_death[1];
		frames[2] = doom->sprites->txt_melee_death[2];
		frames[3] = doom->sprites->txt_melee_death[3];
		frames[4] = doom->sprites->txt_melee_death[4];
		frames[5] = doom->sprites->txt_melee_death[5];
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
	if (enemy->anim_phase > 5)
		enemy->anim.done = IDLE;
}

void		animate_melee_front_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };

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
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_melee_side_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_melee_side_idle;
		frames[1] = doom->sprites->txt_melee_side_attack[0];
		frames[2] = doom->sprites->txt_melee_side_attack[1];
		frames[3] = doom->sprites->txt_melee_side_attack[2];
	}
	if (enemy->anim_phase > 3)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = IDLE;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}
