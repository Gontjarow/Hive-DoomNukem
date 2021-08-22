/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:59:50 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 20:01:56 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	set_single_sprite_null(t_doom *doom)
{
	doom->sprites->txt_ranged_front_attack = NULL;
	doom->sprites->txt_ranged_left_attack = NULL;
	doom->sprites->txt_ranged_right_attack = NULL;
	doom->sprites->txt_ranged_front_idle = NULL;
	doom->sprites->txt_ranged_left_idle = NULL;
	doom->sprites->txt_ranged_right_idle = NULL;
	doom->sprites->txt_ranged_back_idle = NULL;
	doom->sprites->txt_melee_front_idle = NULL;
	doom->sprites->txt_melee_left_idle = NULL;
	doom->sprites->txt_melee_right_idle = NULL;
	doom->sprites->txt_melee_back_idle = NULL;
	doom->sprites->txt_boss_front_idle = NULL;
	doom->sprites->txt_boss_left_idle = NULL;
	doom->sprites->txt_boss_right_idle = NULL;
	doom->sprites->txt_boss_back_idle = NULL;
	doom->sprites->txt_health_pickup = NULL;
	doom->sprites->txt_shotgun_ammo_pickup = NULL;
	doom->sprites->txt_assault_ammo_pickup = NULL;
	doom->sprites->txt_jetpack_ammo_pickup = NULL;
	doom->sprites->txt_key_hud = NULL;
	doom->sprites->txt_light_off = NULL;
	doom->sprites->txt_light_on = NULL;
	doom->sprites->txt_switch_off = NULL;
	doom->sprites->txt_switch_on = NULL;
	doom->sprites->txt_poster_off = NULL;
}

static void	destroy_sprites3(t_doom *doom)
{
	destroy_pickups(doom);
	free(doom->sprites);
	doom->sprites = NULL;
}

static void	destroy_sprites2(t_doom *doom)
{
	SDL_FreeSurface(doom->sprites->txt_pistol_bar);
	doom->sprites->txt_pistol_bar = NULL;
	SDL_FreeSurface(doom->sprites->txt_shotgun_bar);
	doom->sprites->txt_shotgun_bar = NULL;
	SDL_FreeSurface(doom->sprites->txt_rifle_bar);
	doom->sprites->txt_rifle_bar = NULL;
	SDL_FreeSurface(doom->sprites->txt_clip_bar);
	doom->sprites->txt_clip_bar = NULL;
	SDL_FreeSurface(doom->sprites->txt_loading_none);
	doom->sprites->txt_loading_none = NULL;
	SDL_FreeSurface(doom->sprites->txt_loading_0);
	doom->sprites->txt_loading_0 = NULL;
	SDL_FreeSurface(doom->sprites->txt_loading_1);
	doom->sprites->txt_loading_1 = NULL;
	SDL_FreeSurface(doom->sprites->txt_loading_2);
	doom->sprites->txt_loading_2 = NULL;
	SDL_FreeSurface(doom->sprites->txt_loading_won);
	doom->sprites->txt_loading_won = NULL;
	SDL_FreeSurface(doom->sprites->txt_loading_lost);
	doom->sprites->txt_loading_lost = NULL;
	destroy_single_sprites(doom);
	set_single_sprite_null(doom);
	destroy_walls_floors(doom);
	destroy_enemies(doom);
	destroy_sprites3(doom);
}

int	destroy_sprites(t_doom *doom)
{
	SDL_FreeSurface(doom->sprites->txt_window);// total of 72 allocated sprites -> 72 free surfaces calls - done
	doom->sprites->txt_window = NULL;
	SDL_FreeSurface(doom->sprites->txt_door_frame);
	doom->sprites->txt_door_frame = NULL;
	SDL_FreeSurface(doom->sprites->txt_rifle_image);
	doom->sprites->txt_rifle_image = NULL;
	SDL_FreeSurface(doom->sprites->txt_pistol_image);
	doom->sprites->txt_pistol_image = NULL;
	SDL_FreeSurface(doom->sprites->txt_shotgun_image);
	doom->sprites->txt_shotgun_image = NULL;
	SDL_FreeSurface(doom->sprites->txt_transparent);
	doom->sprites->txt_transparent = NULL;
	SDL_FreeSurface(doom->sprites->txt_lever_off);
	doom->sprites->txt_lever_off = NULL;
	SDL_FreeSurface(doom->sprites->txt_lever_on);
	doom->sprites->txt_lever_on = NULL;
	SDL_FreeSurface(doom->sprites->txt_poster_on);
	doom->sprites->txt_poster_on = NULL;
	SDL_FreeSurface(doom->sprites->txt_panel_off);
	doom->sprites->txt_panel_off = NULL;
	SDL_FreeSurface(doom->sprites->txt_panel_on);
	doom->sprites->txt_panel_on = NULL;
	destroy_sprites2(doom);
	return (1);
}
