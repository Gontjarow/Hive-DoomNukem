/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hud2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 18:40:01 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 18:45:25 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	handle_player_fuel_bar(t_doom *doom)
{
	float	percentage;
	int		pos;

	percentage = doom->mdl->player.weap_arr[JETPACK].ammo_cur / 100;
	pos = (int)percentage;
	doom->mdl->player.active_fuel_bar = doom->sprites->txt_fuel_bar[pos];
	draw_surface(0, WIN_HEIGHT - 75, doom->mdl->player.active_fuel_bar,
		doom->game->hud_location);
}

static int	handle_space_ammo_bar(t_doom *doom, int space)
{
	if (doom->mdl->player.weap_id == SHOTGUN)
		space -= 20;
	else
		space -= 10;
	return (space);
}

void	handle_player_ammo_bar(t_doom *doom)
{
	int		ammo;
	int		i;
	int		space;

	ammo = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur;
	i = 1;
	space = 0;
	while (i <= ammo)
	{
		if (doom->mdl->player.reload_time <= 0)
		{
			if (doom->mdl->player.weap_id == PISTOL)
				draw_ignore_alpha((WIN_WIDTH - 10) + space, WIN_HEIGHT - 40,
					doom->sprites->txt_pistol_bar, doom->game->hud_location);
			else if (doom->mdl->player.weap_id == SHOTGUN)
				draw_ignore_alpha(WIN_WIDTH - 10 + space - 10, WIN_HEIGHT - 45,
					doom->sprites->txt_shotgun_bar, doom->game->hud_location);
			else if (doom->mdl->player.weap_id == ASSAULT_RIFLE)
				draw_ignore_alpha((WIN_WIDTH - 10) + space, WIN_HEIGHT - 40,
					doom->sprites->txt_rifle_bar, doom->game->hud_location);
			space = handle_space_ammo_bar(doom, space);
		}
		i++;
	}
}

void	handle_clip_bar(t_doom *doom)
{
	int		clips;
	int		i;
	int		space;

	clips = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res;
	space = 0;
	i = 0;
	while (i < clips)
	{
		draw_ignore_alpha((WIN_WIDTH - 20) + space, WIN_HEIGHT - 80,
			doom->sprites->txt_clip_bar, doom->game->hud_location);
		space -= 20;
		i++;
	}
}

void	handle_weapon_bar(t_doom *doom)
{
	// if (doom->game->cel_shade_hud)
	// 	return (handle_weapon_bar_shaded(doom));
	if (doom->mdl->player.weap_id == PISTOL && !doom->mdl->player.is_running)
		draw_ignore_alpha(WIN_WIDTH - 150, 0,
			doom->sprites->txt_pistol_image, doom->game->hud_location);
	else if (doom->mdl->player.weap_id == SHOTGUN
		&& !doom->mdl->player.is_running)
		draw_ignore_alpha(WIN_WIDTH - 260, 0,
			doom->sprites->txt_shotgun_image, doom->game->hud_location);
	else if (doom->mdl->player.weap_id == ASSAULT_RIFLE
		&& !doom->mdl->player.is_running)
		draw_ignore_alpha(WIN_WIDTH - 260, 0,
			doom->sprites->txt_rifle_image, doom->game->hud_location);
}
