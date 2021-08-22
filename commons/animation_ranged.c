/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_ranged.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 15:00:16 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 18:52:15 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	animate_ranged_back_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[4] = {0};
	static int			cd = 0;

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
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}

void	animate_ranged_front_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[4] = {0};
	static int			cd = 0;

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
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}

void	animate_ranged_death(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[7] = {0};

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
	if (enemy->anim_phase > 6)
	{
		enemy->anim_phase = 6;
		enemy->anim.dead = 1;
	}
	enemy->active_sprite = frames[enemy->anim_phase];
	if (enemy->anim.count > doom->delta_anim)
	{
		enemy->anim.count = 0;
		enemy->anim_phase++;
	}
	enemy->anim.count++;
}

void	animate_ranged_front_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface	*frames[2] = {0};
	static int			cd = 0;

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
	enemy->active_sprite = frames[enemy->anim_phase];
	if (cd > doom->delta_anim)
	{
		cd = 0;
		enemy->anim_phase++;
	}
	cd++;
}
