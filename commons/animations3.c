/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:07:53 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 19:14:31 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	animate_attack(t_enemy *enemy, t_doom *doom)
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

static void	propagate_portal_active_sprites(t_model *mdl)
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

void	animate_portals(t_doom *doom)
{
	static SDL_Surface	*frames[16] = {0};
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
