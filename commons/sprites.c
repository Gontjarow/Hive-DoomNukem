/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 16:32:02 by msuarez-          #+#    #+#             */
/*   Updated: 2021/07/18 16:16:01 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static char *extension_num_path(char *folder, char c, char *extension)
{
	char *path;
	char *join;

	join = ft_strnew(1);
	join[0] = c;
	path = ft_strjoin(folder, join);
	free(join);
	join = path;
	path = ft_strjoin(path, extension);
	free(join);
	return (path);
}

static int	load_texture_sprite(t_doom *doom)
{
	int		i;
	char	*path;

	if (!(doom->sprites->txt_wall = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 10)))
		return (-1);
	i = -1;
	while (++i < 10)
	{
		doom->sprites->txt_wall[i] = NULL;
		path = extension_num_path("img/textures/wall/wall_", i + '0', ".png");
		doom->sprites->txt_wall[i] = load_texture(doom, path);
		free(path);
	}
	if (!(doom->sprites->txt_floor = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 10)))
		return (-1);
	i = -1;
	while (++i < 10)
	{
		doom->sprites->txt_floor[i] = NULL;
		path = extension_num_path("img/textures/floor/floor_", i + '0', ".png");
		doom->sprites->txt_floor[i] = load_texture(doom, path);
		free(path);
	}
	doom->sprites->txt_door = load_texture(doom, "img/textures/wall/door.png");
}

static int	load_ranged_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_ranged_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_left_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_right_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 7)))
		return (-1);
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/ranged/front/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_front_walk[i - 48] = load_texture(doom, path);
		free(path);
		path = extension_num_path("img/sprites/ranged/side/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_left_walk[i - 48] = load_texture(doom, path);
		free(path);
		doom->sprites->txt_ranged_right_walk[i - 48] = flip_horizontal(doom->sprites->txt_ranged_left_walk[i - 48]);
		path = extension_num_path("img/sprites/ranged/back/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_back_walk[i - 48] = load_texture(doom, path);
		free(path);
		i++;
	}
	i = '0';
	while (i < '7')
	{
		path = extension_num_path("img/sprites/ranged/death/death_", i + 1, ".png");
		doom->sprites->txt_ranged_death[i - 48] = load_texture(doom, path);
		free(path);
		i++;
	}
	doom->sprites->txt_ranged_front_attack = load_texture(doom, "img/sprites/ranged/front/attack_1.png");
	doom->sprites->txt_ranged_left_attack = load_texture(doom, "img/sprites/ranged/side/attack_1.png");
	doom->sprites->txt_ranged_right_attack = flip_horizontal(doom->sprites->txt_ranged_left_attack);
	doom->sprites->txt_ranged_front_idle = load_texture(doom, "img/sprites/ranged/front/idle.png");
	doom->sprites->txt_ranged_left_idle = load_texture(doom, "img/sprites/ranged/side/idle.png");
	doom->sprites->txt_ranged_right_idle = flip_horizontal(doom->sprites->txt_ranged_left_idle);
	doom->sprites->txt_ranged_back_idle = load_texture(doom, "img/sprites/ranged/back/idle.png");
	return (1);
}

static void		copy_from_atlas(int x, int y, SDL_Surface *atlas, SDL_Surface *buff)
{
	int *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;
	int to;
	int from;

	iter = 0;
	i = x;
	limit_x = x + buff->w;
	limit_total = buff->w * buff->h;
	pix[0] = (int*)buff->pixels;
	pix[1] = (int*)atlas->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			from = (atlas->w * y) + x;
			to = iter;
			pix[0][to] = pix[1][from];
			x++;
			iter++;
		}
		y++;
	}
}

static int	load_melee_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_melee_front_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
		return (-1);
	if (!(doom->sprites->txt_melee_left_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
		return (-1);
	if (!(doom->sprites->txt_melee_right_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
		return (-1);
	if (!(doom->sprites->txt_melee_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_left_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_right_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	i = '0';
	while (i < '3')
	{
		path = extension_num_path("img/sprites/melee/front/attack_", i + 1, ".png");
		doom->sprites->txt_melee_front_attack[i - 48] = load_texture(doom, path);
		free(path);
		path = extension_num_path("img/sprites/melee/side/attack_", i + 1, ".png");
		doom->sprites->txt_melee_left_attack[i - 48] = load_texture(doom, path);
		free(path);
		doom->sprites->txt_melee_right_attack[i - 48] = flip_horizontal(doom->sprites->txt_melee_left_attack[i - 48]);
		i++;
	}
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/melee/front/walk_", i + 1, ".png");
		doom->sprites->txt_melee_front_walk[i - 48] = load_texture(doom, path);
		free(path);
		path = extension_num_path("img/sprites/melee/side/walk_", i + 1, ".png");
		doom->sprites->txt_melee_left_walk[i - 48] = load_texture(doom, path);
		free(path);
		doom->sprites->txt_melee_right_walk[i - 48] = flip_horizontal(doom->sprites->txt_melee_left_walk[i - 48]);
		path = extension_num_path("img/sprites/melee/back/walk_", i + 1, ".png");
		doom->sprites->txt_melee_back_walk[i - 48] = load_texture(doom, path);
		free(path);
		i++;
	}
	i = '0';
	while (i < '6')
	{
		path = extension_num_path("img/sprites/melee/death/death_", i + 1, ".png");
		doom->sprites->txt_melee_death[i - 48] = load_texture(doom, path);
		free(path);
		i++;
	}
	doom->sprites->txt_melee_front_idle = load_texture(doom, "img/sprites/melee/front/idle.png");
	doom->sprites->txt_melee_left_idle = load_texture(doom, "img/sprites/melee/side/idle.png");
	doom->sprites->txt_melee_right_idle = flip_horizontal(doom->sprites->txt_melee_left_idle);
	doom->sprites->txt_melee_back_idle = load_texture(doom, "img/sprites/melee/back/idle.png");
	return (1);
}

static int	load_boss_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_boss_front_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
		return (-1);
	if (!(doom->sprites->txt_boss_left_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
		return (-1);
	if (!(doom->sprites->txt_boss_right_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
		return (-1);
	if (!(doom->sprites->txt_boss_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_left_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_right_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 9)))
		return (-1);
	i = '0';
	while (i < '2')
	{
		path = extension_num_path("img/sprites/boss/front/attack_", i + 1, ".png");
		doom->sprites->txt_boss_front_attack[i - 48] = load_texture(doom, path);
		free(path);
		path = extension_num_path("img/sprites/boss/side/attack_", i + 1, ".png");
		doom->sprites->txt_boss_left_attack[i - 48] = load_texture(doom, path);
		free(path);
		doom->sprites->txt_boss_right_attack[i - 48] = flip_horizontal(doom->sprites->txt_boss_left_attack[i - 48]);
		i++;
	}
	i = '0';
	while (i < '6')
	{
		path = extension_num_path("img/sprites/boss/front/walk_", i + 1, ".png");
		doom->sprites->txt_boss_front_walk[i - 48] = load_texture(doom, path);
		free(path);
		path = extension_num_path("img/sprites/boss/side/walk_", i + 1, ".png");
		doom->sprites->txt_boss_left_walk[i - 48] = load_texture(doom, path);
		free(path);
		doom->sprites->txt_boss_right_walk[i - 48] = flip_horizontal(doom->sprites->txt_boss_left_walk[i - 48]);
		path = extension_num_path("img/sprites/boss/back/walk_", i + 1, ".png");
		doom->sprites->txt_boss_back_walk[i - 48] = load_texture(doom, path);
		free(path);
		i++;
	}
	i = '0';
	while (i < '9')
	{
		path = extension_num_path("img/sprites/boss/death/death_", i + 1, ".png");
		doom->sprites->txt_boss_death[i - 48] = load_texture(doom, path);
		free(path);
		i++;
	}
	doom->sprites->txt_boss_front_idle = load_texture(doom, "img/sprites/boss/front/idle.png");
	doom->sprites->txt_boss_left_idle = load_texture(doom, "img/sprites/boss/side/idle.png");
	doom->sprites->txt_boss_right_idle = flip_horizontal(doom->sprites->txt_boss_left_idle);
	doom->sprites->txt_boss_back_idle = load_texture(doom, "img/sprites/boss/back/idle.png");
	return (1);
}

void		init_enemy_sprite(t_doom *doom)
{
	int		ec;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (enemy->ai.type_id == RANGED)
			enemy->active_sprite = doom->sprites->txt_ranged_front_idle;
		else if (enemy->ai.type_id == MELEE)
			enemy->active_sprite = doom->sprites->txt_melee_front_idle;
		else if (enemy->ai.type_id == BOSS)
			enemy->active_sprite = doom->sprites->txt_boss_front_idle;
		enemy = enemy->next;
	}
}

int			load_pickup_sprite(t_doom *doom)
{
	doom->sprites->txt_health_pickup = load_texture(doom, "img/pickups/health.png");
	doom->sprites->txt_shotgun_ammo_pickup = load_texture(doom, "img/pickups/shotgunammo.png");
	doom->sprites->txt_assault_ammo_pickup = load_texture(doom, "img/pickups/akammo.png");
	doom->sprites->txt_jetpack_ammo_pickup = load_texture(doom, "img/pickups/jetpackammo.png");
	doom->sprites->txt_key_hud = load_texture(doom, "img/pickups/key.png");
	return (1);
}

void		load_health_bars(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_health_bar = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 11)))
		return ;
	i = 0;
	while (i < 10)
	{
		path = extension_num_path("img/sprites/hud/healthbar/health_", i + '0', ".png");
		doom->sprites->txt_health_bar[i] = load_texture(doom, path);
		free(path);
		i++;
	}
	doom->sprites->txt_health_bar[10] = load_texture(doom, "img/sprites/hud/healthbar/health_10.png");
}

void		load_fuel_bars(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_fuel_bar = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 11)))
		return ;
	doom->sprites->txt_fuel_bar[0] = load_texture(doom, "img/sprites/hud/healthbar/health_0.png");
	i = 1;
	while (i < 10)
	{
		path = extension_num_path("img/sprites/hud/fuelbar/fuel_", i + '0', ".png");
		doom->sprites->txt_fuel_bar[i] = load_texture(doom, path);
		free(path);
		i++;
	}
	doom->sprites->txt_fuel_bar[10] = load_texture(doom, "img/sprites/hud/fuelbar/fuel_10.png");
}

static int	load_portal_sprite(t_doom *doom, char *path, int i)
{
	doom->sprites->portal_phase = 0;
	if (!(doom->sprites->txt_portal = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 16))) //missing
		return (-1);
	i = 0;
	while (i < 10)
	{
		path = extension_num_path("img/sprites/portal/portal_", i + '0', ".png");
		doom->sprites->txt_portal[i] = load_texture(doom, path);
		free(path);
		i++;
	}
	i = 0;
	while (i < 6)
	{
		path = extension_num_path("img/sprites/portal/portal_1", i + '0', ".png");
		doom->sprites->txt_portal[i + 10] = load_texture(doom, path);
		free(path);
		i++;
	}
	return (1);
}

static void	load_effector_sprites(t_doom *doom)
{
	doom->sprites->txt_light_off = load_texture(doom, "img/textures/effectors/light_off.png");
	doom->sprites->txt_light_on = load_texture(doom, "img/textures/effectors/light_on.png");
	doom->sprites->txt_switch_off = load_texture(doom, "img/textures/effectors/switch_off.png");
	doom->sprites->txt_switch_on = load_texture(doom, "img/textures/effectors/switch_on.png");
	doom->sprites->txt_poster_off = load_texture(doom, "img/textures/effectors/poster_off.png");
	doom->sprites->txt_poster_on = load_texture(doom, "img/textures/effectors/poster_on.png");
	doom->sprites->txt_panel_off = load_texture(doom, "img/textures/effectors/panel_off.png");
	doom->sprites->txt_panel_on = load_texture(doom, "img/textures/effectors/panel_on.png");
	doom->sprites->txt_lever_off = load_texture(doom, "img/textures/effectors/lever_off.png");
	doom->sprites->txt_lever_on = load_texture(doom, "img/textures/effectors/lever_on.png");
}

void		load_ammo_bars(t_doom *doom)
{
	doom->sprites->txt_pistol_ammo_bar = load_texture(doom, "img/sprites/hud/ammobar/pistolammo.png");
	doom->sprites->txt_shotgun_ammo_bar = load_texture(doom, "img/sprites/hud/ammobar/shotgunammo.png");
	doom->sprites->txt_assault_ammo_bar = load_texture(doom, "img/sprites/hud/ammobar/assaultammo.png");
	doom->sprites->txt_clip_bar = load_texture(doom, "img/sprites/hud/ammobar/cliptest.png");
}

static void load_loading_screen(t_doom *doom)
{
	doom->sprites->txt_loading_none = load_texture(doom, "img/loading/chapter_none.png");
	doom->sprites->txt_loading_0 = load_texture(doom, "img/loading/first_chapter.png");
	doom->sprites->txt_loading_1 = load_texture(doom, "img/loading/second_chapter.png");
	doom->sprites->txt_loading_2 = load_texture(doom, "img/loading/third_chapter.png");
	doom->sprites->txt_loading_won = load_texture(doom, "img/loading/won.png");
	doom->sprites->txt_loading_lost = load_texture(doom, "img/loading/lost.png");
	//puts("LOADING SCREEN IMAGES LOADED TO MEMORY");
}

int			load_sprites(t_doom *doom)
{
	char	i;
	char	*path;

	if (doom->sprites == NULL)
		doom->sprites = (t_sprites*)malloc(sizeof(t_sprites));
	else
		ft_putendl("Warning: Doom->sprites was not NULL when load_sprites");
	if (doom->sprites == NULL)
		ft_putendl("Warning: Doom->sprites was not malloced at load_sprites");
	load_texture_sprite(doom);
	load_ranged_sprite(doom, path, i);
	load_melee_sprite(doom, path, i);
	load_boss_sprite(doom, path, i);
	load_pickup_sprite(doom);
	load_health_bars(doom, path, i);
	load_fuel_bars(doom, path, i);
	load_ammo_bars(doom);
	load_loading_screen(doom);
	load_portal_sprite(doom, path, i);
	load_effector_sprites(doom);
	return (1);
}

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
	doom->sprites->txt_light_on =  NULL;
	doom->sprites->txt_switch_off = NULL;
	doom->sprites->txt_switch_on = NULL;
	doom->sprites->txt_poster_off = NULL;
}

int			destroy_sprites(t_doom *doom)
{
	// total of 66 allocated sprites -> 66 free surfaces calls - done
	// Single pointer surfaces
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
	SDL_FreeSurface(doom->sprites->txt_pistol_ammo_bar);
	doom->sprites->txt_pistol_ammo_bar = NULL;
	SDL_FreeSurface(doom->sprites->txt_shotgun_ammo_bar);
	doom->sprites->txt_shotgun_ammo_bar = NULL;
	SDL_FreeSurface(doom->sprites->txt_assault_ammo_bar);
	doom->sprites->txt_assault_ammo_bar = NULL;
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
	destroy_pickups(doom);
	free(doom->sprites);
	doom->sprites = NULL;
	return (1);
}
