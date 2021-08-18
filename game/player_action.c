/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_action.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 16:43:51 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 19:44:37 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

// static void	update_world(t_world *world)
// {
// 	t_model		*mdl;
// 	t_room		*room;
// 	t_sector	*sector;
// 	int			room_index;
// 	int			rooms;

// 	mdl = get_model();
// 	room = mdl->room_first;
// 	room_index = 0;
// 	rooms = mdl->room_count;
// 	while (~ --rooms)
// 	{
// 		sector = &world->sectors[room_index];
// 		sector->floor = room->floor_height / WORLD_SCALE;
// 		sector->ceil = room->roof_height / WORLD_SCALE;
// 		++room_index;
// 		room = room->next;
// 	}
// }

// static void	experimental_elevator(int active, int hard_reset)
// {
// 	static int	original_floor = -1;
// 	static int	original_roof = -1;

// 	if (active && original_floor == -1 && original_roof == -1)
// 	{
// 		original_floor = get_model()->room_first->floor_height;
// 		original_roof = get_model()->room_first->roof_height;
// 	}
// 	else if (active)
// 	{
// 		get_model()->room_first->floor_height++;
// 		get_model()->room_first->roof_height++;
// 		get_model()->room_first->floor_height = get_model()->room_first->floor_height > 750 ? 750 : get_model()->room_first->floor_height;
// 		get_model()->room_first->roof_height = get_model()->room_first->roof_height > 800 ? 800 : get_model()->room_first->roof_height;
// 		update_world(get_world());
// 		if (get_model()->player.room_id == get_model()->room_first->id)
// 		{
// 			get_model()->player.z++;
// 			get_model()->player.z = get_model()->player.z > get_model()->player.room->floor_height + get_model()->player.height ?
// 									get_model()->player.room->floor_height + get_model()->player.height : get_model()->player.z;
// 		}
// 	}
// 	else if (hard_reset)
// 	{
// 		get_model()->room_first->floor_height = original_floor;
// 		get_model()->room_first->roof_height = original_roof;
// 		update_world(get_world());
// 		puts("EXPERIMENTAL ELEVATOR RESET TO ORIGINAL VALUES");
// 	}
// 	else if (!active)
// 	{
// 		get_model()->room_first->floor_height--;
// 		get_model()->room_first->roof_height--;
// 		get_model()->room_first->floor_height = get_model()->room_first->floor_height < 0 ? 0 : get_model()->room_first->floor_height;
// 		get_model()->room_first->roof_height = get_model()->room_first->roof_height < 50 ? 50 : get_model()->room_first->roof_height;
// 		update_world(get_world());
// 		if (get_model()->player.room_id == get_model()->room_first->id)
// 		{
// 			get_model()->player.z--;
// 			get_model()->player.z = get_model()->player.z < get_model()->player.room->floor_height + get_model()->player.height ?
// 									get_model()->player.room->floor_height + get_model()->player.height : get_model()->player.z;
// 		}
// 	}
// }

static void	player_switch_light(t_doom *doom, t_effect *effect)
{
	t_room	*curr_room;

	curr_room = room_by_wall_id(effect->target_id, doom->mdl);
	if (!doom->mdl->player.eff_pressed)
	{
		doom->mdl->player.eff_pressed = 1;
		effect->activated = !effect->activated;
		curr_room->lit = !curr_room->lit;
		if (effect->activated)
			effect->active_sprite = doom->sprites->txt_switch_on;
		else
			effect->active_sprite = doom->sprites->txt_switch_off;
	}
}

static void	player_switch_lever(t_doom *doom, t_effect *effect)
{
	if (!doom->mdl->player.eff_pressed)
	{
		doom->mdl->player.eff_pressed = 1;
		effect->activated = !effect->activated;
		if (effect->activated)
			effect->active_sprite = doom->sprites->txt_lever_on;
		else
			effect->active_sprite = doom->sprites->txt_lever_off;
		open_doors(doom, effect);
	}
}

void	player_effector_interactions(t_doom *doom, int ec)
{
	t_effect	*effect;

	ec = doom->mdl->effect_count;
	if (ec == 0)
		return ;
	effect = doom->mdl->effect_first;
	while (ec--)
	{
		if (effect->type_id == EFFECT_LIGHTKNOB
			&& player_collision_with_effects(doom, effect) == -1)
			player_switch_light(doom, effect);
		if (effect->type_id == EFFECT_KEYPANEL
			&& player_collision_with_effects(doom, effect) == -1
			&& doom->mdl->player.has_key && !effect->activated)
		{
			doom->mdl->player.has_key = 0;
			effect->activated = 1;
			effect->active_sprite = doom->sprites->txt_panel_on;
			open_doors(doom, effect);
		}
		if (effect->type_id == EFFECT_LEVER
			&& player_collision_with_effects(doom, effect) == -1)
			player_switch_lever(doom, effect);
		effect = effect->next;
	}
}
