/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_model.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 17:13:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/01/27 19:44:54 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void expand_mdl_polygon_maps(t_model *mdl)
{
    static int times = 0;
    int rc;
    t_room *room;

    rc = mdl->room_count;
    room = mdl->room_first;
    while (rc--)
    {
        if (mdl->poly_map == NULL || room == NULL || mdl->parent == NULL)
            ft_die("Fatal error: No poly_map, room or mdl_parent set at expand_mdl_polygon_maps");
        add_room_polymap(room, mdl->poly_map, get_conv_colors());
        room = room->next;
    }
}

static void		assign_enemy_cd(t_model *mdl)
{
	t_enemy *enemy;
	int		ec;

	ec = mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = mdl->enemy_first;
	while (ec--)
	{
		enemy->stun_time = 0;
		enemy->stun_cd = 0;
		enemy->anim.done = IDLE;
		enemy->anim_phase = 0;
		enemy->shoot_cd = 0;
		enemy->wep.cooldown = 15;
		enemy = enemy->next;
	}
}

static void		link_mdl_rooms(t_model *mdl)
{
    t_room *room;
    int 	rc;

    rc = mdl->room_count;
    room = mdl->room_first;
    while (rc--)
    {
    	room->first_wall = wall_by_id(room->first_wall_id);
        room = room->next;
    }
}

static void print_map_strings(t_mapfile *map)
{
	if (map->wall_string)
		ft_putendl(map->wall_string);
	if (map->room_string)
		ft_putendl(map->room_string);
	if (map->portal_string)
		ft_putendl(map->portal_string);
	if (map->enemy_string)
		ft_putendl(map->enemy_string);
	if (map->pickup_string)
		ft_putendl(map->pickup_string);
	if (map->effect_string)
		ft_putendl(map->effect_string);
	if (map->player_string)
		ft_putendl(map->player_string);
	if (map->chain_string)
		ft_putendl(map->chain_string);
	ft_putendl("Printed all map strings!");
}

static void create_model_appended(t_doom *doom)
{
	ft_putendl("Loaded appended mapfile data!");
	print_map_strings(doom->map);
	map_to_model(doom->map, doom->mdl);
	link_mdl_rooms(doom->mdl);
	expand_mdl_polygon_maps(doom->mdl);
}

static void	create_data_model(t_doom *doom, char *map_file_path)
{
	//puts("CREATE DATA BORN!!!!");
	ft_putstr("Loaded mapfile data from file: ");
	ft_putendl(map_file_path);
	map_to_model(doom->map, doom->mdl);
	link_mdl_rooms(doom->mdl);
	expand_mdl_polygon_maps(doom->mdl);
}

static void assign_player_room(t_doom *doom)
{
	int location_id;

	location_id = check_location(doom, doom->mdl->player.x, doom->mdl->player.y);
	doom->mdl->player.room_id = location_id;
	doom->mdl->player.room = room_by_id(location_id);
}

int			load_model(t_doom *doom)
{
	init_model(doom);
	if (!doom->edt_quit)
	{
		if (!doom->edt->map_supplied)
		{
//			if (!load_appended(doom))
//				ft_putendl("Failed to load appended map file, tried because no map was supplied as argument.");
//			else
//				create_model_appended(doom);
			return (1);
		}
		else if (!stringify_mapfile(doom, doom->edt->map_path))
			return (0);
		create_data_model(doom, doom->edt->map_path);
	}
	else if (!doom->game_quit)
	{
		if (!doom->game->map_supplied)
			return (0);
		if (!stringify_mapfile(doom, doom->game->map_path))
			return (0);
		create_data_model(doom, doom->game->map_path);
		assign_enemy_cd(doom->mdl);
		assign_player_room(doom);
	}
	return (1);
}
