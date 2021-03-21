/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_pickup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:20:47 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/01 19:16:08 by msuarez-         ###   ########.fr       */
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

	if (pickup->weapon_type_id - 1 == 0)
		doom->mdl->player.weap_arr[pickup->weapon_type_id - 1].ammo_res = 1;
	else
		doom->mdl->player.weap_arr[pickup->weapon_type_id - 1].ammo_res += 1;
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

	doom->mdl->player.weap_arr[pickup->weapon_type_id - 1].do_own = 1;
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
	doom->game->level_exit_reached = 1;
	doom->menu->update_argc_argv = 1;
	if (doom->menu->added_arg)
		free(doom->menu->added_arg);
	doom->menu->added_arg = ft_strdup(doom->mdl->chain);
	printf("Set added_arg to [%s]\n", doom->mdl->chain);
}

void			handle_pickup(t_doom *doom)
{
	int			pc;
	t_pickup	*pickup;

	// TODO SEPARATE TO PROPER FUNCTION
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
		doom->mdl->player.weap_arr[pickup->weapon_type_id - 1].ammo_res < 10)
			handle_ammo_pickup(doom, pickup);
		else if (pickup->flavor == PICKUP_WEAPON &&
		player_collision_with_pickup(doom, pickup) == -1 &&
		doom->mdl->player.weap_arr[pickup->weapon_type_id - 1].do_own == 0)
			handle_weapon_pickup(doom, pickup);
		pickup = pickup->next;
	}
}

