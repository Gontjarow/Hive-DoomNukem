/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 16:47:10 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/09 14:14:41 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	destroy_walls_floors(t_doom *doom)
{
	int i;

	i = -1;
	while (++i < 10)
	{
		SDL_FreeSurface(doom->sprites->txt_wall[i]);
		doom->sprites->txt_wall[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_floor[i]);
		doom->sprites->txt_floor[i] = NULL;
	}
	free(doom->sprites->txt_wall);
	doom->sprites->txt_wall = NULL;
	free(doom->sprites->txt_floor);
	doom->sprites->txt_floor = NULL;
	free(doom->sprites->txt_door);
	doom->sprites->txt_door = NULL;
}

static void	destroy_more_enemies(t_doom *doom)
{
	int i;

	i = -1;
	while (++i < 4)
	{
		SDL_FreeSurface(doom->sprites->txt_ranged_front_walk[i]);
		doom->sprites->txt_ranged_front_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_ranged_left_walk[i]);
		doom->sprites->txt_ranged_left_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_ranged_right_walk[i]);
		doom->sprites->txt_ranged_right_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_ranged_back_walk[i]);
		doom->sprites->txt_ranged_back_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_melee_front_walk[i]);
		doom->sprites->txt_melee_front_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_melee_left_walk[i]);
		doom->sprites->txt_melee_left_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_melee_right_walk[i]);
		doom->sprites->txt_melee_right_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_melee_back_walk[i]);
		doom->sprites->txt_melee_back_walk[i] = NULL;
	}
	free(doom->sprites->txt_ranged_front_walk);
	doom->sprites->txt_ranged_front_walk = NULL;
	free(doom->sprites->txt_ranged_left_walk);
	doom->sprites->txt_ranged_left_walk = NULL;
	free(doom->sprites->txt_ranged_right_walk);
	doom->sprites->txt_ranged_right_walk = NULL;
	free(doom->sprites->txt_ranged_back_walk);
	doom->sprites->txt_ranged_back_walk = NULL;
	free(doom->sprites->txt_melee_front_walk);
	doom->sprites->txt_melee_front_walk = NULL;
	free(doom->sprites->txt_melee_left_walk);
	doom->sprites->txt_melee_left_walk = NULL;
	free(doom->sprites->txt_melee_right_walk);
	doom->sprites->txt_melee_right_walk = NULL;
	free(doom->sprites->txt_melee_back_walk);
	doom->sprites->txt_melee_back_walk = NULL;
	i = -1;
	while (++i < 6)
	{
		SDL_FreeSurface(doom->sprites->txt_melee_death[i]);
		doom->sprites->txt_melee_death[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_boss_front_walk[i]);
		doom->sprites->txt_boss_front_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_boss_left_walk[i]);
		doom->sprites->txt_boss_left_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_boss_right_walk[i]);
		doom->sprites->txt_boss_right_walk[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_boss_back_walk[i]);
		doom->sprites->txt_boss_back_walk[i] = NULL;
	}
	free(doom->sprites->txt_melee_death);
	doom->sprites->txt_melee_death = NULL;
	free(doom->sprites->txt_boss_front_walk);
	doom->sprites->txt_boss_front_walk = NULL;
	free(doom->sprites->txt_boss_left_walk);
	doom->sprites->txt_boss_left_walk = NULL;
	free(doom->sprites->txt_boss_right_walk);
	doom->sprites->txt_boss_right_walk = NULL;
	free(doom->sprites->txt_boss_back_walk);
	doom->sprites->txt_boss_back_walk = NULL;
}

void	destroy_enemies(t_doom *doom)
{
	int i;
	
	i = -1;
	while (++i < 2)
	{
		SDL_FreeSurface(doom->sprites->txt_boss_front_attack[i]);
		doom->sprites->txt_boss_front_attack[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_boss_left_attack[i]);
		doom->sprites->txt_boss_left_attack[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_boss_right_attack[i]);
		doom->sprites->txt_boss_right_attack[i] = NULL;
	}
	free(doom->sprites->txt_boss_front_attack);
	doom->sprites->txt_boss_front_attack = NULL;
	free(doom->sprites->txt_boss_left_attack);
	doom->sprites->txt_boss_left_attack = NULL;
	free(doom->sprites->txt_boss_right_attack);
	doom->sprites->txt_boss_right_attack = NULL;
	i = -1;
	while (++i < 3)
	{
		SDL_FreeSurface(doom->sprites->txt_melee_front_attack[i]);
		doom->sprites->txt_melee_front_attack[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_melee_left_attack[i]);
		doom->sprites->txt_melee_left_attack[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_melee_right_attack[i]);
		doom->sprites->txt_melee_right_attack[i] = NULL;
	}
	free(doom->sprites->txt_melee_front_attack);
	doom->sprites->txt_melee_front_attack = NULL;
	free(doom->sprites->txt_melee_left_attack);
	doom->sprites->txt_melee_left_attack = NULL;
	free(doom->sprites->txt_melee_right_attack);
	doom->sprites->txt_melee_right_attack = NULL;
	i = -1;
	while (++i < 7)
	{
		SDL_FreeSurface(doom->sprites->txt_ranged_death[i]);
		doom->sprites->txt_ranged_death[i] = NULL;
	}
	free(doom->sprites->txt_ranged_death);
	doom->sprites->txt_ranged_death = NULL;
	i = -1;
	while (++i < 9)
	{
		SDL_FreeSurface(doom->sprites->txt_boss_death[i]);
		doom->sprites->txt_boss_death[i] = NULL;
	}
	free(doom->sprites->txt_boss_death);
	doom->sprites->txt_boss_death = NULL;
	destroy_more_enemies(doom);
}

void	destroy_single_sprites(t_doom *doom)
{
	SDL_FreeSurface(doom->sprites->txt_ranged_front_attack);
	SDL_FreeSurface(doom->sprites->txt_ranged_left_attack);
	SDL_FreeSurface(doom->sprites->txt_ranged_right_attack);
	SDL_FreeSurface(doom->sprites->txt_ranged_front_idle);
	SDL_FreeSurface(doom->sprites->txt_ranged_left_idle);
	SDL_FreeSurface(doom->sprites->txt_ranged_right_idle);
	SDL_FreeSurface(doom->sprites->txt_ranged_back_idle);
	SDL_FreeSurface(doom->sprites->txt_melee_front_idle);
	SDL_FreeSurface(doom->sprites->txt_melee_left_idle);
	SDL_FreeSurface(doom->sprites->txt_melee_right_idle);
	SDL_FreeSurface(doom->sprites->txt_melee_back_idle);
	SDL_FreeSurface(doom->sprites->txt_boss_front_idle);
	SDL_FreeSurface(doom->sprites->txt_boss_left_idle);
	SDL_FreeSurface(doom->sprites->txt_boss_right_idle);
	SDL_FreeSurface(doom->sprites->txt_boss_back_idle);
	SDL_FreeSurface(doom->sprites->txt_health_pickup);
	SDL_FreeSurface(doom->sprites->txt_shotgun_ammo_pickup);
	SDL_FreeSurface(doom->sprites->txt_assault_ammo_pickup);
	SDL_FreeSurface(doom->sprites->txt_jetpack_ammo_pickup);
	SDL_FreeSurface(doom->sprites->txt_key_hud);
	SDL_FreeSurface(doom->sprites->txt_light_off);
	SDL_FreeSurface(doom->sprites->txt_light_on);
	SDL_FreeSurface(doom->sprites->txt_switch_off);
	SDL_FreeSurface(doom->sprites->txt_switch_on);
	SDL_FreeSurface(doom->sprites->txt_poster_off);
}

void	destroy_pickups(t_doom *doom)
{
	int i;

	i = -1;
	while (++i < 11)
	{
		SDL_FreeSurface(doom->sprites->txt_fuel_bar[i]);
		doom->sprites->txt_fuel_bar[i] = NULL;
		SDL_FreeSurface(doom->sprites->txt_health_bar[i]);
		doom->sprites->txt_health_bar[i] = NULL;
	}
	free(doom->sprites->txt_fuel_bar);
	doom->sprites->txt_fuel_bar = NULL;
	free(doom->sprites->txt_health_bar);
	doom->sprites->txt_health_bar = NULL;
	i = -1;
	while (++i < 16)
	{
		SDL_FreeSurface(doom->sprites->txt_portal[i]);
		doom->sprites->txt_portal[i] = NULL;
	}
	SDL_FreeSurface(doom->sprites->txt_portal);
	doom->sprites->txt_portal = NULL;
}
