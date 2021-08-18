/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 20:21:46 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 19:07:25 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	player_swap_weapons(t_doom *doom)
{
	if (doom->mdl->player.reload_time <= 0)
	{
		if (doom->keystates[SDL_SCANCODE_1] && doom->mdl->player.weap_arr
			[PISTOL].do_own && doom->mdl->player.weap_id != PISTOL)
		{
			doom->mdl->player.weap_id = PISTOL;
			Mix_PlayChannel(-1, doom->sounds->mcWeaponPickup, 0);
		}
		if (doom->keystates[SDL_SCANCODE_2] && doom->mdl->player.weap_arr
			[SHOTGUN].do_own && doom->mdl->player.weap_id != SHOTGUN)
		{
			doom->mdl->player.weap_id = SHOTGUN;
			Mix_PlayChannel(-1, doom->sounds->mcWeaponPickup, 0);
		}
		if (doom->keystates[SDL_SCANCODE_3] && doom->mdl->player.weap_arr
			[ASSAULT_RIFLE].do_own && doom->mdl->player.weap_id
			!= ASSAULT_RIFLE)
		{
			doom->mdl->player.weap_id = ASSAULT_RIFLE;
			Mix_PlayChannel(-1, doom->sounds->mcWeaponPickup, 0);
		}
	}
}

static void	handle_ammo_calc(t_doom *doom)
{
	int	dif;
	int	rest;

	if (doom->mdl->player.weap_id == PISTOL)
	{
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur = 12;
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res = 1;
	}
	else
	{
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur
			= doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max;
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res--;
	}
}

static void	player_shoot_reload(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_R])
	{
		if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur
			!= doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max
			&& doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res
			> 0)
		{
			Mix_PlayChannel(-1, doom->mdl->player.weap_arr
			[doom->mdl->player.weap_id].reload_sound, 0);
			doom->mdl->player.reload_time = doom->mdl->player.weap_arr
			[doom->mdl->player.weap_id].reload_time;
			handle_ammo_calc(doom);
		}
	}
}

void	player_update_weapons(t_doom *doom)
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

void	destroy_weapon_img(t_doom *doom)
{
	int	n;

	n = 0;
	while (n < 3)
	{
		SDL_FreeSurface(doom->mdl->player.weap_arr[n].weap_img);
		doom->mdl->player.weap_arr[n].weap_img = NULL;
		n++;
	}
}
