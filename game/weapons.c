/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 20:21:46 by msuarez-          #+#    #+#             */
/*   Updated: 2021/06/05 19:29:20 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		init_player_weapon(t_doom *doom)
{
	init_hud(doom);
	doom->mdl->player.weap_arr[0].ammo_cur = 12;
	doom->mdl->player.weap_arr[0].ammo_res = 1;
	doom->mdl->player.weap_arr[0].ammo_max = 12;
	doom->mdl->player.weap_arr[0].cooldown = 60 / 600.0;
	doom->mdl->player.weap_arr[0].dmg = 15;
	doom->mdl->player.weap_arr[0].reload_time = 1.5;
	doom->mdl->player.weap_arr[0].fire_sound = doom->sounds->mcPistolShot;
	doom->mdl->player.weap_arr[0].reload_sound = doom->sounds->mcPistolRld;
	doom->mdl->player.weap_arr[0].weap_img = load_texture(doom, "img/weapons/colt.png");
	doom->mdl->player.weap_arr[0].do_own = 1;
	doom->mdl->player.weap_arr[1].ammo_cur = 5;
	doom->mdl->player.weap_arr[1].ammo_res = 5;
	doom->mdl->player.weap_arr[1].ammo_max = 5;
	doom->mdl->player.weap_arr[1].cooldown = 60 / 120.0;
	doom->mdl->player.weap_arr[1].dmg = 40;
	doom->mdl->player.weap_arr[1].reload_time = 3.5;
	doom->mdl->player.weap_arr[1].fire_sound = doom->sounds->mcShotgunShot;
	doom->mdl->player.weap_arr[1].reload_sound = doom->sounds->mcShotgunRld;
	doom->mdl->player.weap_arr[1].weap_img = load_texture(doom, "img/weapons/shotgun.png");
	doom->mdl->player.weap_arr[1].do_own = 0;
	doom->mdl->player.weap_arr[2].ammo_cur = 30;
	doom->mdl->player.weap_arr[2].ammo_res = 3;
	doom->mdl->player.weap_arr[2].ammo_max = 30;
	doom->mdl->player.weap_arr[2].cooldown = 60 / 600.0;
	doom->mdl->player.weap_arr[2].dmg = 30;
	doom->mdl->player.weap_arr[2].reload_time = 2;
	doom->mdl->player.weap_arr[2].fire_sound = doom->sounds->mcAssaultShot;
	doom->mdl->player.weap_arr[2].reload_sound = doom->sounds->mcAssaultRld;
	doom->mdl->player.weap_arr[2].weap_img = load_texture(doom, "img/weapons/ak47.png");
	doom->mdl->player.weap_arr[2].do_own = 0;
	doom->mdl->player.weap_arr[3].do_own = 0;
	doom->mdl->player.weap_arr[3].ammo_res = 0;
	doom->mdl->player.weap_arr[3].ammo_cur = 1000;
	doom->mdl->player.weap_arr[3].ammo_max = 1000;
	doom->mdl->player.weap_arr[3].weap_img = load_texture(doom, "img/weapons/jetpack.png");
}

static void	player_swap_weapons(t_doom *doom)
{
	if (doom->mdl->player.reload_time <= 0)
	{
		if (doom->keystates[SDL_SCANCODE_1] && doom->mdl->player.weap_arr[PISTOL].do_own && doom->mdl->player.weap_id != PISTOL)
		{
			doom->mdl->player.weap_id = PISTOL;
			Mix_PlayChannel(-1, doom->sounds->mcWeaponPickup, 0);
		}
		if (doom->keystates[SDL_SCANCODE_2] && doom->mdl->player.weap_arr[SHOTGUN].do_own && doom->mdl->player.weap_id != SHOTGUN)
		{
			doom->mdl->player.weap_id = SHOTGUN;
			Mix_PlayChannel(-1, doom->sounds->mcWeaponPickup, 0);
		}
		if (doom->keystates[SDL_SCANCODE_3] && doom->mdl->player.weap_arr[ASSAULT_RIFLE].do_own && doom->mdl->player.weap_id != ASSAULT_RIFLE)
		{
			doom->mdl->player.weap_id = ASSAULT_RIFLE;
			Mix_PlayChannel(-1, doom->sounds->mcWeaponPickup, 0);
		}
	}
}

static void	handle_ammo_calc(t_doom *doom)
{
	int dif;
	int rest;

	if (doom->mdl->player.weap_id == PISTOL)
	{
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur = 12;
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res = 1;
	}
	else
	{
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur =
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max;
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res--;
	}
}

static void	player_shoot_reload(t_doom *doom)
{
	// Disabled Shooting with E key -> Place holder for player interaction with doors
	// if (doom->keystates[SDL_SCANCODE_E] && doom->mdl->player.has_key && player_collision_with_effector(doom, effector) == -1)
	// {
	// 	doom->mdl->player.has_key = 0;
	// 	open_door();
	// }
	if (doom->keystates[SDL_SCANCODE_R])
	{
		if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur !=
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max &&
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res > 0)
		{
			Mix_PlayChannel(-1, doom->mdl->player.weap_arr[doom->mdl->player.weap_id].reload_sound, 0);
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
		doom->mdl->player.shoot_cd -= 1 * doom->delta_time;
	if (doom->mdl->player.reload_time > 0)
		doom->mdl->player.reload_time -= 1 * doom->delta_time;
	if (doom->sounds->jetpack_delay > 0)
		doom->sounds->jetpack_delay -= 1 * doom->delta_time;
}
