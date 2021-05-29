/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 15:08:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/05/29 18:03:36 by msuarez-         ###   ########.fr       */
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
	// printf("Animate ranged [%d] hurt %f\n", enemy->id, (float)SDL_GetTicks());
	if (enemy->stun_time <= 0)
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
	if (enemy->stun_time <= 0)
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
    if (enemy->stun_time <= 0)
        enemy->anim.done = IDLE;
    enemy->active_sprite = frames[1];
}

void			animate_walk(t_enemy *enemy, t_doom *doom)
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
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			animate_boss_front_walk(enemy, doom);
		else if (enemy->anim.orient == BACK)
			animate_boss_back_walk(enemy, doom);
		else
			animate_boss_side_walk(enemy, doom);
	}
}

void			animate_death(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == RANGED)
		animate_ranged_death(enemy, doom);
	else if (enemy->ai.type_id == MELEE)
		animate_melee_death(enemy, doom);
	else
		animate_boss_death(enemy, doom);
}

void			animate_attack(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == RANGED)
	{
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			animate_ranged_front_attack(enemy, doom);
		else if (enemy->anim.orient == LEFT || enemy->anim.orient == RIGHT)
			animate_ranged_side_attack(enemy, doom);
		else
			enemy->active_sprite = doom->sprites->txt_ranged_back_idle;
	}
	else if (enemy->ai.type_id == MELEE)
	{
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			animate_melee_front_attack(enemy, doom);
		else
			animate_melee_side_attack(enemy, doom);	
	}
	else
	{
		if (enemy->anim.orient == FRONT || enemy->anim.orient == -1)
			animate_boss_front_attack(enemy, doom);
		else
			animate_boss_side_attack(enemy, doom);
	}
}

void			animate_hurt(t_enemy *enemy, t_doom *doom)
{
	if (enemy->ai.type_id == RANGED)
		animate_ranged_hurt(enemy, doom);
	else if (enemy->ai.type_id == MELEE)
		animate_melee_hurt(enemy, doom);
	else
		animate_boss_hurt(enemy, doom);
}

void			animate_idle(t_enemy *enemy, t_doom *doom)
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
	else
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

static void		propagate_portal_active_sprites(t_model *mdl)
{
	t_effect	*effect;
	int			ec;

	if (mdl->effect_count < 1)
		return ;
	effect = mdl->effect_first;
	ec = mdl->effect_count;
	while (ec--)
	{
		if (effect->type_id == EFFECT_EXIT)
		{
			effect->active_sprite = doom_ptr()->sprites->active_portal;
			return ;
		}
		effect = effect->next;
	}	
}

void			animate_portals(t_doom *doom)
{
	static SDL_Surface *frames[16] = { 0 };
	int					i;

	if (frames[0] == 0)
	{
		i = 0;
		while (i < 16)
		{
			frames[i] = doom->sprites->txt_portal[i];
			i++;
		}
	}
	if (doom->sprites->portal_phase > 15)
		doom->sprites->portal_phase = 0;
	doom->sprites->active_portal = frames[doom->sprites->portal_phase++];
	propagate_portal_active_sprites(doom->mdl);
}

void			animation_switch(t_enemy *enemy, t_doom *doom)
{
	if (enemy->anim.dead)
		return ;
	if (enemy->anim.done == DEATH || enemy->hp.cur == 0)
		animate_death(enemy, doom);
	else if (enemy->anim.done == MOVE)
		animate_walk(enemy, doom);
	else if (enemy->anim.done == ATTACK)
		animate_attack(enemy, doom);
	else if (enemy->anim.done == HURT)
		animate_hurt(enemy, doom);
	else if (enemy->anim.done == IDLE)
		animate_idle(enemy, doom);
}
