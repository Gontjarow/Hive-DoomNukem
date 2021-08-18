/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 19:06:21 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 19:12:48 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	init_hud(t_doom *doom)
{
	int	n;

	n = 0;
	while (n < 3)
	{
		doom->mdl->player.weap_arr[n].weap_img = (SDL_Surface *)
			malloc(sizeof(SDL_Surface));
		if (!(doom->mdl->player.weap_arr[n].weap_img))
			ft_die("Error allocating weap_img!\n");
		n++;
	}
}

static void	init_more_weap(t_doom *doom)
{
	doom->mdl->player.weap_arr[2].ammo_cur = 30;
	doom->mdl->player.weap_arr[2].ammo_res = 3;
	doom->mdl->player.weap_arr[2].ammo_max = 30;
	doom->mdl->player.weap_arr[2].cooldown = 60 / 600.0;
	doom->mdl->player.weap_arr[2].dmg = 30;
	doom->mdl->player.weap_arr[2].reload_time = 2;
	doom->mdl->player.weap_arr[2].fire_sound = doom->sounds->mcAssaultShot;
	doom->mdl->player.weap_arr[2].reload_sound = doom->sounds->mcAssaultRld;
	doom->mdl->player.weap_arr[2].weap_img = load_texture(doom,
			"img/weapons/aktest.png");
	doom->mdl->player.weap_arr[2].do_own = 0;
	doom->mdl->player.weap_arr[3].do_own = 0;
	doom->mdl->player.weap_arr[3].ammo_res = 0;
	doom->mdl->player.weap_arr[3].ammo_cur = 1000;
	doom->mdl->player.weap_arr[3].ammo_max = 1000;
	doom->mdl->player.weap_arr[3].weap_img = load_texture(doom,
			"img/weapons/jetpack.png");
}

void	init_player_weapon(t_doom *doom)
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
	doom->mdl->player.weap_arr[0].weap_img = load_texture(doom,
			"img/weapons/coltest.png");
	doom->mdl->player.weap_arr[0].do_own = 1;
	doom->mdl->player.weap_arr[1].ammo_cur = 5;
	doom->mdl->player.weap_arr[1].ammo_res = 5;
	doom->mdl->player.weap_arr[1].ammo_max = 5;
	doom->mdl->player.weap_arr[1].cooldown = 60 / 120.0;
	doom->mdl->player.weap_arr[1].dmg = 40;
	doom->mdl->player.weap_arr[1].reload_time = 3.5;
	doom->mdl->player.weap_arr[1].fire_sound = doom->sounds->mcShotgunShot;
	doom->mdl->player.weap_arr[1].reload_sound = doom->sounds->mcShotgunRld;
	doom->mdl->player.weap_arr[1].weap_img = load_texture(doom,
			"img/weapons/shotguntest.png");
	doom->mdl->player.weap_arr[1].do_own = 0;
	init_more_weap(doom);
}
