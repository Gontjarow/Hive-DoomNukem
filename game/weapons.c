/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 20:21:46 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/11 20:29:53 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		init_player_weapon(t_doom *doom)
{
	init_hud(doom);
	load_numbers(doom);
	doom->mdl->player.weap_arr[0].ammo_cur = 12;
	doom->mdl->player.weap_arr[0].ammo_res = 9999;
	doom->mdl->player.weap_arr[0].ammo_max = 12;
	doom->mdl->player.weap_arr[0].cooldown = 10;
	doom->mdl->player.weap_arr[0].dmg = 20;
	doom->mdl->player.weap_arr[0].reload_time = 20;
	doom->mdl->player.weap_arr[0].fire_sound = doom->sounds->mcPistolShot;
	doom->mdl->player.weap_arr[0].reload_sound = doom->sounds->mcPistolRld;
	doom->mdl->player.weap_arr[0].weap_img = load_png("img/weapons/colt.png");
	doom->mdl->player.weap_arr[0].do_own = 1;
	doom->mdl->player.weap_arr[1].ammo_cur = 30;
	doom->mdl->player.weap_arr[1].ammo_res = 250;
	doom->mdl->player.weap_arr[1].ammo_max = 30;
	doom->mdl->player.weap_arr[1].cooldown = 2;
	doom->mdl->player.weap_arr[1].dmg = 25;
	doom->mdl->player.weap_arr[1].reload_time = 25;
	doom->mdl->player.weap_arr[1].fire_sound = doom->sounds->mcSmgShot;
	doom->mdl->player.weap_arr[1].reload_sound = doom->sounds->mcSmgRld;
	doom->mdl->player.weap_arr[1].weap_img = load_png("img/weapons/mp5.png");
	doom->mdl->player.weap_arr[1].do_own = 0;
	doom->mdl->player.weap_arr[2].ammo_cur = 40;
	doom->mdl->player.weap_arr[2].ammo_res = 200;
	doom->mdl->player.weap_arr[2].ammo_max = 40;
	doom->mdl->player.weap_arr[2].cooldown = 2;
	doom->mdl->player.weap_arr[2].dmg = 30;
	doom->mdl->player.weap_arr[2].reload_time = 35;
	doom->mdl->player.weap_arr[2].fire_sound = doom->sounds->mcAssaultShot;
	doom->mdl->player.weap_arr[2].reload_sound = doom->sounds->mcAssaultRld;
	doom->mdl->player.weap_arr[2].weap_img = load_png("img/weapons/ak47.png");
	doom->mdl->player.weap_arr[2].do_own = 0;
}

static void	player_swap_weapons(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_1] && doom->mdl->player.weap_arr
		[0].do_own == 1)
		doom->mdl->player.weap_id = 0;
	if (doom->keystates[SDL_SCANCODE_2] && doom->mdl->player.weap_arr
		[1].do_own == 1)
		doom->mdl->player.weap_id = 1;
	if (doom->keystates[SDL_SCANCODE_3] && doom->mdl->player.weap_arr
		[2].do_own == 1)
		doom->mdl->player.weap_id = 2;
}

static void	handle_ammo_calc(t_doom *doom)
{
	int dif;
	int rest;

	if (doom->mdl->player.weap_id == 0)	// pistol has infinite ammo
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur = 12;
	else
	{
		dif = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max -
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur;
		rest = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res - dif;
		if (rest <= 0)
		{
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur +=
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res;
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res = 0;
		}
		else
		{
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur =
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max;
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res = rest;
		}
	}
}

static void	player_shoot_reload(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_E])
	{
		if (doom->mdl->player.shoot_cd == 0 && doom->mdl->player.weap_arr
			[doom->mdl->player.weap_id].ammo_cur > 0 &&
				doom->mdl->player.reload_time == 0)
		{
			Mix_PlayChannel(0, doom->mdl->player.weap_arr[doom->mdl->player.weap_id].fire_sound, 0);
			player_shoots(doom);
		}
	}
	if (doom->keystates[SDL_SCANCODE_R])
	{
		if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur !=
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max &&
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res > 0)
		{
			Mix_PlayChannel(0, doom->mdl->player.weap_arr[doom->mdl->player.weap_id].reload_sound, 0);
			doom->mdl->player.reload_time = doom->mdl->player.weap_arr
				[doom->mdl->player.weap_id].reload_time;
			handle_ammo_calc(doom);
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
		// printf("Reloading...\n");
		doom->mdl->player.reload_time--;
	}
}
