/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_pickup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:20:47 by msuarez-          #+#    #+#             */
/*   Updated: 2021/05/08 16:26:07 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void		handle_health_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	doom->mdl->player.hp.cur += 50;
	if (doom->mdl->player.hp.cur > 100)
		doom->mdl->player.hp.cur = 100;
	pc = doom->mdl->pickup_count;
	current = doom->mdl->pickup_first;
	if (current->id == pickup->id)
		pickup = delete_first(doom);
	else
		pickup = delete_node(current, prev, pickup);
	Mix_PlayChannel(-1, doom->sounds->mcHealthPickup, 0);
	doom->mdl->pickup_count--;
}

static void		handle_ammo_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	if (pickup->weapon_type_id == PISTOL)
		doom->mdl->player.weap_arr[pickup->weapon_type_id].ammo_res = 1;
	else if (pickup->weapon_type_id == JETPACK)
		doom->mdl->player.weap_arr[pickup->weapon_type_id].ammo_cur = 1000;
	else
		doom->mdl->player.weap_arr[pickup->weapon_type_id].ammo_res += 1;
	pc = doom->mdl->pickup_count;
	current = doom->mdl->pickup_first;
	if (current->id == pickup->id)
		pickup = delete_first(doom);
	else
		pickup = delete_node(current, prev, pickup);
	Mix_PlayChannel(-1, doom->sounds->mcAmmoPickup, 0);
	doom->mdl->pickup_count--;
}

static void		handle_weapon_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	doom->mdl->player.weap_arr[pickup->weapon_type_id].do_own = 1;
	pc = doom->mdl->pickup_count;
	current = doom->mdl->pickup_first;
	if (current->id == pickup->id)
		pickup = delete_first(doom);
	else
		pickup = delete_node(current, prev, pickup);
	Mix_PlayChannel(-1, doom->sounds->mcWeaponPickup, 0);
	doom->mdl->pickup_count--;
}

static void		handle_level_exit(t_doom *doom)
{
	puts("Reached level exit!");
	if (Mix_Playing(7))
		Mix_HaltChannel(7);
	doom->game->level_exit_reached = 1;
	if (doom->mdl->chain != NULL)
	{
		doom->menu->update_argc_argv = 1;
		doom->menu->added_arg = ft_strdup(doom->mdl->chain);
	}
	else
	{
		doom->game->level_exit_reached = 0;
		doom->game->show_loading = 1;
		doom->game->won_the_game = 1;
		render_winning_screen(doom, 1);
	}
}

void			handle_pickup(t_doom *doom)
{
	int			pc;
	t_pickup	*pickup;

	// TODO handle_effects
	if (get_model()->effect_count > 0 && player_collision_with_exit(doom, get_model()->effect_first))
		handle_level_exit(doom);
	pc = doom->mdl->pickup_count;
	if (pc == 0)
		return ;
	pickup = doom->mdl->pickup_first;
	while (pc--)
	{
		if (pickup->flavor == PICKUP_HEALTH &&
		player_collision_with_pickup(doom, pickup) == -1 &&
		doom->mdl->player.hp.cur < 100)
			handle_health_pickup(doom, pickup);
		else if (pickup->flavor == PICKUP_AMMO &&
		player_collision_with_pickup(doom, pickup) == -1 &&
		doom->mdl->player.weap_arr[pickup->weapon_type_id].ammo_res < 10)
			handle_ammo_pickup(doom, pickup);
		else if (pickup->flavor == PICKUP_WEAPON &&
		player_collision_with_pickup(doom, pickup) == -1 &&
		doom->mdl->player.weap_arr[pickup->weapon_type_id].do_own == 0)
			handle_weapon_pickup(doom, pickup);
		pickup = pickup->next;
	}
}
