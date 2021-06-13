/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_action.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 16:43:51 by msuarez-          #+#    #+#             */
/*   Updated: 2021/06/13 19:52:48 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	player_jumps(t_doom *doom)
{
	if (doom->mdl->player.weap_arr[JETPACK].do_own &&
		doom->mdl->player.weap_arr[JETPACK].ammo_cur > 0)
	{
		doom->mdl->player.is_flying = 1;
		doom->mdl->player.weap_arr[JETPACK].ammo_cur--;
		doom->mdl->player.z_velocity += 0.4;
		if (doom->sounds->jetpack_delay <= 0)
		{
			Mix_PlayChannel(-1, doom->sounds->mcJetpack, 0);
			doom->sounds->jetpack_delay = 0.5;
		}
		return ;
	}
	if (doom->mdl->player.is_jumping == 0 &&
		doom->mdl->player.is_crouching == 0)
	{
		doom->mdl->player.z_velocity += 3.0;
		doom->mdl->player.is_jumping = 1;
	}
}

static void	player_crouch(t_doom *doom)
{
	if (!doom->mdl->player.is_crouching)
	{
		doom->mdl->player.crouch_lock = 1;
		doom->mdl->player.is_crouching = 1;
		doom->mdl->player.mov_speed = doom->mdl->player.min_speed;
		doom->mdl->player.height = CROUCH_HEIGHT;
	}
	else
	{
		doom->mdl->player.crouch_lock = 0;
		doom->mdl->player.is_crouching = 0;
		doom->mdl->player.mov_speed = 300;
		doom->mdl->player.height = STAND_HEIGHT;
	}
}

static void	player_run(t_doom *doom)
{
	if (!doom->mdl->player.run_lock)
	{
		doom->mdl->player.run_lock = 1;
		doom->mdl->player.is_running = 1;
	}
	else
	{
		doom->mdl->player.is_running = 0;
		doom->mdl->player.run_lock = 0;
	}
}

static void	update_world(t_world *world)
{
	t_model		*mdl;
	t_room		*room;
	t_sector	*sector;
	int			room_index;
	int			rooms;

	mdl = get_model();
	room = mdl->room_first;
	room_index = 0;
	rooms = mdl->room_count;
	while (~--rooms)
	{
		sector = &world->sectors[room_index];
		sector->floor = room->floor_height / WORLD_SCALE;
		sector->ceil = room->roof_height / WORLD_SCALE;
		++room_index;
		room = room->next;
	}
}

static void	experimental_elevator(int active, int hard_reset)
{
	static int	original_floor = -1;
	static int	original_roof = -1;

	if (active && original_floor == -1 && original_roof == -1)
	{
		original_floor = get_model()->room_first->floor_height;
		original_roof = get_model()->room_first->roof_height;
	}
	else if (active)
	{
		get_model()->room_first->floor_height++;
		get_model()->room_first->roof_height++;
		get_model()->room_first->floor_height = get_model()->room_first->floor_height > 750 ? 750 : get_model()->room_first->floor_height;
		get_model()->room_first->roof_height = get_model()->room_first->roof_height > 800 ? 800 : get_model()->room_first->roof_height;
		update_world(get_world());
		if (get_model()->player.room_id == get_model()->room_first->id)
		{
			get_model()->player.z++;
			get_model()->player.z = get_model()->player.z > get_model()->player.room->floor_height + get_model()->player.height ?
									get_model()->player.room->floor_height + get_model()->player.height : get_model()->player.z;
		}
	}
	else if (hard_reset)
	{
		get_model()->room_first->floor_height = original_floor;
		get_model()->room_first->roof_height = original_roof;
		update_world(get_world());
		puts("EXPERIMENTAL ELEVATOR RESET TO ORIGINAL VALUES");
	}
	else if (!active)
	{
		get_model()->room_first->floor_height--;
		get_model()->room_first->roof_height--;
		get_model()->room_first->floor_height = get_model()->room_first->floor_height < 0 ? 0 : get_model()->room_first->floor_height;
		get_model()->room_first->roof_height = get_model()->room_first->roof_height < 50 ? 50 : get_model()->room_first->roof_height;
		update_world(get_world());
		if (get_model()->player.room_id == get_model()->room_first->id)
		{
			get_model()->player.z--;
			get_model()->player.z = get_model()->player.z < get_model()->player.room->floor_height + get_model()->player.height ?
									get_model()->player.room->floor_height + get_model()->player.height : get_model()->player.z;
		}
	}
}

static void	player_effector_interactions(t_doom *doom)		// WIP - 1
{
	int			ec;
	t_effect	*effect;

	ec = doom->mdl->effect_count;
	if (ec == 0)
		return ;
	effect = doom->mdl->effect_first;
	while (ec--)
	{
		if (effect->type_id == EFFECT_LIGHTKNOB &&
			player_collision_with_effects(doom, effect) == -1)
		{
			effect->activated = !effect->activated;
			if (effect->activated)
				effect->active_sprite = doom->sprites->txt_switch_off;
			else
				effect->active_sprite = doom->sprites->txt_switch_on;
			// switch_light();
		}
		if (effect->type_id == EFFECT_KEYPANEL &&
			player_collision_with_effects(doom, effect) == -1 &&
			doom->mdl->player.has_key && effect->activated == 0)
		{
			doom->mdl->player.has_key = 0;
			effect->activated = 1;
			effect->active_sprite = doom->sprites->txt_panel_on;
			// open_door();
		}
		effect = effect->next;
	}
}

void		handle_player_action(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_SPACE])
		player_jumps(doom);
	if (doom->keystates[SDL_SCANCODE_LCTRL] && !doom->mdl->player.crouch_lock
		&& !doom->mdl->player.is_jumping)
		player_crouch(doom);
	if (!doom->keystates[SDL_SCANCODE_LCTRL] && doom->mdl->player.crouch_lock)
		player_crouch(doom);
	if (doom->keystates[SDL_SCANCODE_LSHIFT] && !doom->mdl->player.run_lock
		&& !doom->mdl->player.crouch_lock && !doom->mdl->player.is_jumping)
		player_run(doom);
	if (!doom->keystates[SDL_SCANCODE_LSHIFT] && doom->mdl->player.run_lock)
		player_run(doom);
	if (doom->keystates[SDL_SCANCODE_E])
		player_effector_interactions(doom);
	if (doom->keystates[SDL_SCANCODE_P])
		debug_model_effects();
	if (doom->keystates[SDL_SCANCODE_L])
		debug_model_chain();
	if (doom->keystates[SDL_SCANCODE_U])
		experimental_elevator(1, 0);
	if (doom->keystates[SDL_SCANCODE_J])
		experimental_elevator(0, 0);
	if (doom->keystates[SDL_SCANCODE_N])
		experimental_elevator(0, 1);
}
