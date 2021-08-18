/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_action3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 19:44:48 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 19:47:03 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static t_effect	*find_target_effect(t_doom *doom, int keypanel_id)
{
	int				ec;
	t_effect		*effect;

	ec = doom->mdl->effect_count;
	if (ec == 0)
		return (NULL);
	effect = doom->mdl->effect_first;
	while (ec--)
	{
		if (effect->id == keypanel_id + 1)
			return (effect);
		effect = effect->next;
	}
	ft_die("WARNING!! RETURN VALUE IN FIND_TARGET_EFFECT IS NULL");
	return (NULL);
}

static void	handle_lever(t_doom *doom, t_effect *effect, t_wall *portals)
{
	if (effect->target_id == portals->id && !effect->activated)
	{
		portals->open = 1;
		portals->active_sprite = doom->sprites->txt_door_frame;
	}
	else if (effect->target_id == portals->id && effect->activated)
	{
		portals->open = 0;
		portals->active_sprite = doom->sprites->txt_door;
	}
}

static void	handle_keypanel(t_doom *doom, t_effect *effect, t_wall *portals)
{
	t_effect	*target_effect;

	target_effect = find_target_effect(doom, effect->id);
	if (target_effect->target_id == portals->id)
	{
		portals->open = 1;
		portals->active_sprite = doom->sprites->txt_transparent;
	}
}

void	open_doors(t_doom *doom, t_effect *effect)
{
	int				pc;
	t_wall			*portals;

	pc = doom->mdl->portal_count;
	if (pc == 0)
		return ;
	portals = doom->mdl->portal_first;
	while (pc--)
	{
		if (portals->portal_type == DOOR_PORTAL)
		{
			if (effect->type_id == EFFECT_LEVER)
				handle_lever(doom, effect, portals);
			else
				handle_keypanel(doom, effect, portals);
		}
		portals = portals->next;
	}
}
