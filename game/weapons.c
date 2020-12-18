/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 20:21:46 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/18 17:31:01 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		init_player_weapon(t_doom *doom)
{
	doom->mdl->player.weap_arr[0].ammo_cur = 12;
	doom->mdl->player.weap_arr[0].ammo_max = 12;
	doom->mdl->player.weap_arr[0].cooldown = 10;
	doom->mdl->player.weap_arr[0].dmg = 20;
	doom->mdl->player.weap_arr[0].reload_time = 15;
	doom->mdl->player.weap_arr[1].ammo_cur = 30;
	doom->mdl->player.weap_arr[1].ammo_max = 30;
	doom->mdl->player.weap_arr[1].cooldown = 5;
	doom->mdl->player.weap_arr[1].dmg = 25;
	doom->mdl->player.weap_arr[1].reload_time = 25;
	doom->mdl->player.weap_arr[2].ammo_cur = 40;
	doom->mdl->player.weap_arr[2].ammo_max = 40;
	doom->mdl->player.weap_arr[2].cooldown = 2;
	doom->mdl->player.weap_arr[2].dmg = 30;
	doom->mdl->player.weap_arr[2].reload_time = 35;
}

static void	player_swap_weapons(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_1])
		doom->mdl->player.weap_id = 0;
	if (doom->keystates[SDL_SCANCODE_2])
		doom->mdl->player.weap_id = 1;
	if (doom->keystates[SDL_SCANCODE_3])
		doom->mdl->player.weap_id = 2;
}

static void	player_shoot_reload(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_E])
	{
		if (doom->mdl->player.shoot_cd == 0 && doom->mdl->player.weap_arr
			[doom->mdl->player.weap_id].ammo_cur > 0 &&
				doom->mdl->player.reload_time == 0)
			player_shoots(doom);
	}
	if (doom->keystates[SDL_SCANCODE_R])
	{
		if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur !=
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max)
		{
			doom->mdl->player.reload_time = doom->mdl->player.weap_arr
				[doom->mdl->player.weap_id].reload_time;
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur =
				doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max;
		}
	}
}

void		player_update_weapons(t_doom *doom)
{
	player_swap_weapons(doom);
	player_shoot_reload(doom);
	if (doom->mdl->player.shoot_cd > 0)
		doom->mdl->player.shoot_cd--;
	if (doom->mdl->player.reload_time > 0)
	{
		printf("Reloading...\n");
		doom->mdl->player.reload_time--;
	}
	if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur > 0
		&& doom->mdl->player.reload_time == 0)
		printf("Player ammo: %d/%d\n", doom->mdl->player.weap_arr
		[doom->mdl->player.weap_id].ammo_cur, doom->mdl->player.
		weap_arr[doom->mdl->player.weap_id].ammo_max);
	else if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].
			ammo_cur == 0)
		printf("Out of ammo!\n");
}
