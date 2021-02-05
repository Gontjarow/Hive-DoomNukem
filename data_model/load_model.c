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

static void	create_data_model(t_doom *doom, char *map_file_path)
{
	ft_putstr("Loaded mapfile data from file: ");
	ft_putendl(map_file_path);
	map_to_model(doom->map, doom->mdl);
	link_mdl_rooms(doom->mdl);
	expand_mdl_polygon_maps(doom->mdl);
}

int			load_model(t_doom *doom)
{
	init_model(doom);
	if (!doom->edt_quit)
	{
			//ft_putendl("Load_model initiated with the editor loading.");
		if (!doom->edt->map_supplied)
		{
				//ft_putendl("Skipped load_model, no map was supplied");
			return (1);
		}
		if (!stringify_mapfile(doom, doom->edt->map_path))
		{
				//ft_putendl("Warning: Failed to read mapfile, no model was loaded.");
			return (1);
		}
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
	}
	return (1);
}
