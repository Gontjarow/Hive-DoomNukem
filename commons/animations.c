/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 15:08:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/25 18:38:18 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		animation_switch(t_enemy *enemy, t_doom *doom, int flag)
{
	// -1 - Idle state / 0 - Death State / 1 - Move state / 2 - Attack state
	if (enemy->ai.type_id == 0 && flag == 0)
		animate_ranged_death(enemy, doom);
	else if (enemy->ai.type_id == 1 && flag == 0)
		animate_melee_death(enemy, doom);
	else if (enemy->ai.type_id == 2 && flag == 0)
		animate_boss_death(enemy, doom);
	else if (enemy->ai.type_id == 0 && flag == 1)
		animate_ranged_front_walk(enemy, doom);
	else if (enemy->ai.type_id == 1 && flag == 1)
		animate_melee_front_walk(enemy, doom);
	else if (enemy->ai.type_id == 2 && flag == 1)
		animate_boss_front_walk(enemy, doom);
	else if (enemy->ai.type_id == 0 && flag == 2)
		animate_ranged_front_attack(enemy, doom);
	else if (enemy->ai.type_id == 1 && flag == 2)
		animate_melee_front_attack(enemy, doom);
	else if (enemy->ai.type_id == 2 && flag == 2)
		animate_boss_front_attack(enemy, doom);
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
		enemy->anim.done = -1;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_ranged_side_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[4] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_side_walk[0];
		frames[1] = doom->sprites->txt_ranged_side_walk[1];
		frames[2] = doom->sprites->txt_ranged_side_walk[2];
		frames[3] = doom->sprites->txt_ranged_side_walk[3];
	}
	if (enemy->anim_phase > 3)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = -1;
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
		enemy->anim.done = -1;
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
		enemy->anim.done = -1;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_ranged_side_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[2] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_ranged_side_idle;
		frames[1] = doom->sprites->txt_ranged_side_attack;
	}
	if (enemy->anim_phase > 1)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = -1;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

/*
**	Melee Animations
*/

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
		enemy->anim.done = -1;
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
		enemy->anim.done = -1;
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
		enemy->anim.done = -1;
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
		enemy->anim.done = -1;
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
		enemy->anim.done = -1;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

/*
**	Boss Animations
*/

void		animate_boss_front_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[6] = { 0 };

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
		enemy->anim.done = -1;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_boss_side_walk(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[6] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_side_walk[0];
		frames[1] = doom->sprites->txt_boss_side_walk[1];
		frames[2] = doom->sprites->txt_boss_side_walk[2];
		frames[3] = doom->sprites->txt_boss_side_walk[3];
		frames[4] = doom->sprites->txt_boss_side_walk[4];
		frames[5] = doom->sprites->txt_boss_side_walk[5];
	}
	if (enemy->anim_phase > 5)
	{
		enemy->anim.done = -1;
		enemy->anim_phase = 0;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_boss_death(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[9] = { 0 };

	if (frames[0] == 0)
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
	enemy->active_sprite = frames[enemy->anim_phase++];
	if (enemy->anim_phase > 8)
		enemy->anim.done = -1;
}

void		animate_boss_front_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[3] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_front_idle;
		frames[1] = doom->sprites->txt_boss_front_attack[0];
		frames[2] = doom->sprites->txt_boss_front_attack[1];
	}
	if (enemy->anim_phase > 2)
	{
		enemy->anim_phase = 0;
		enemy->anim.done = -1;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}

void		animate_boss_side_attack(t_enemy *enemy, t_doom *doom)
{
	static SDL_Surface *frames[3] = { 0 };

	if (frames[0] == 0)
	{
		frames[0] = doom->sprites->txt_boss_side_idle;
		frames[1] = doom->sprites->txt_boss_side_attack[0];
		frames[2] = doom->sprites->txt_boss_side_attack[1];
	}
	if (enemy->anim_phase > 2)
	{
		enemy->anim.done = -1;
		enemy->anim_phase = 0;
	}
	enemy->active_sprite = frames[enemy->anim_phase++];
}
