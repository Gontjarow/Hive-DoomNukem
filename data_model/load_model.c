/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_model.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 17:13:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/08/18 17:07:53 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	void_polymap_door_ranks(t_model *mdl)
{
	t_wall	*portal;
	int		pc;

	portal = mdl->portal_first;
	pc = mdl->portal_count;
	while (pc--)
	{
		if (portal->portal_type == DOOR_PORTAL)
		{
			puts("Should void this DOOR_PORTAL in polymap with void pixel ranks on both sides + on top of the vector, to prevent player sinking into door!");
			// TODO Come up with a voiding / de-voiding algorithm for the polymap for the door during game loading / gameplay when opened
			// eg. here we would call something like polymap_manipulate_door(door, CLOSE);
			// polymap_manipulate_door(t_wall *door, int open)
			//{
			//	algorithm that manipulates ranks of pixels on top of the door vector into mdl->polymap with color 0 or color room_by_portal(door)->id;
			//}
			// eg. gameplay_code()->door_open() should call something like polymap_manipulate_door(door, OPEN);
			// TODO test airtightness with a single rank, triple rank, quintuple rank, septuple rank and select the first completely airtight rank count
		}
		portal = portal->next;
	}
}

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
		enemy->anim.dead = 0;
		enemy->anim.count = 0;
		enemy->stun_time = 0;
		enemy->stun_cd = 0;
		enemy->anim.done = IDLE;
		enemy->anim_phase = 0;
		enemy->shoot_cd = 0;
		enemy->wep.cooldown = 15;
		enemy = enemy->next;
	}
}

static void		link_mdl_wall_textures(t_model *mdl)
{
	t_wall	*wall;
	int		wc;

	wc = mdl->wall_count;
	wall = mdl->wall_first;
	while (wc--)
	{
		if (wall->texture_id < 0 || wall->texture_id > 9)
		{
			wall->active_sprite = NULL;
			printf("Warning: Link_mdl_wall_textures function aborted and linked a NULL pointer wall->active_sprite, texture_id out of bounds was %d\n", wall->texture_id);
			wall = wall->next;
			continue ;
		}
		wall->active_sprite = doom_ptr()->sprites->txt_wall[wall->texture_id];
		if (wall->active_sprite == NULL)
			printf("Warning: Link_mdl_wall_textures function linked to NULL pointer wall->active_sprite, texture_id was %d\n", wall->texture_id);
		wall = wall->next;
	}
	//ft_putendl("LINKED WALL TEXTURE_IDS TO WALL ACTIVE SPRITES IN DOOM->MDL");
}

static int		wall_is_also_portal(t_wall *wall)
{
	t_wall	*portal;
	int		pc;

	portal = get_model()->portal_first;
	pc = get_model()->portal_count;
	while (pc--)
	{
		if (matching_walls(wall, portal))
			return (1);
		portal = portal->next;
	}	
	return (0);
}

static void		lightknob_room(t_room *room)
{
	t_wall	*wall;
	int		wc;

	wall = room->first_wall;
	wc = room->wall_count;
	while (wc--)
	{
		if (!(wall_is_also_portal(wall)))
		{
			if (effect_lightknob_wall(wall))
			{
				//puts("Lightknobbing a room that was not already lightknobbed!");
				return ;
			}
		}
		wall = wall->next;
	}
	puts("Failed to lightknob a room that was not already lightknobbed! No non-portal walls found in room!");
}

static void		link_mdl_rooms(t_model *mdl)
{
    t_room	*room;
    int		rc;
	int		lightknob_auto_count;

	lightknob_auto_count = 0;
    rc = mdl->room_count;
    room = mdl->room_first;
    while (rc--)
    {
    	room->first_wall = wall_by_id(room->first_wall_id);
		// Calculate barymetric center point for convex polygon (can be used to place sprite of light to room for example)
		find_visual_xy(room);
		//puts("SET BARYMETRIC CENTERPOINT FOR ROOM->VISUAL.X AND ROOM->VISUAL.Y!");
		if (!already_lightknobbed_room(room))
		{
			lightknob_room(room);
			lightknob_auto_count++;
		}		
        room = room->next;
    }
	if (lightknob_auto_count)
		puts("Editor: Set Lightknobs automatically to some rooms which were missing them!");
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

static void	fill_lever_effect_data(t_model *mdl, t_wall *door)
{
	t_xy		door_vec;
	t_xy		lever_position;

	// Target.x means to convey, that for target_id (door), render on 20% between 0,1 to the right and 50% between 0,1 on the vertical axis
	mdl->effects->target.x = 90;
	mdl->effects->target.y = 90;
	// Here it could also calculate similar position to the 2D world coordinates to the loc.x and loc.y data fields
	door_vec = vec2(door->end.x - door->start.x, door->end.y - door->start.y);
	lever_position = vec2_add(vec2(door->start.x, door->start.y), vec2_mul(vec2_norm(door_vec), 0.1f * vec2_mag(door_vec)));
	mdl->effects->loc.x = (int)lever_position.x;
	mdl->effects->loc.y = (int)lever_position.y;
	// Assign the correct effector type_id, id and default the active sprite to txt_lever_on sprite and target_id to the door of which lever it is
	mdl->effects->id = mdl->effect_count;
	mdl->effects->type_id = EFFECT_LEVER;
	mdl->effects->active_sprite = doom_ptr()->sprites->txt_lever_on;
	mdl->effects->target_id = door->id;
	mdl->effects->activated = 1;
}

// For each PORTAL_DOOR, loop and create EFFECT_LEVER type of effector
static void create_lever_effectors(t_model *mdl)
{
	t_effect	*new_effect;	
	t_wall		*portal = mdl->portal_first;
	int			pc = mdl->portal_count;

	while (pc--)
	{
		if (portal->portal_type != DOOR_PORTAL || already_targeted_door_portal(portal))
		{
			portal = portal->next;
			continue;
		}
		ft_putendl("Debug (Kalle): Created an EFFECT_LEVER!");
		fill_lever_effect_data(mdl, portal);
		new_effect = (t_effect*)malloc(sizeof(t_effect));
		if (!new_effect)
			ft_die("Fatal error: Could not malloc new_effect at create_lever_effectors!");
		mdl->effects->next = new_effect;
		mdl->effect_count++;
		if (mdl->effect_count == 1)
			mdl->effect_first = mdl->effects;
		mdl->effects = new_effect;
		portal = portal->next;
	}	
}

static void	create_data_model(t_doom *doom, char *map_file_path)
{
	puts("DATA MODEL BORN!!!!");
	ft_putstr("Loaded mapfile data from file: ");
	ft_putendl(map_file_path);
	map_to_model(doom->map, doom->mdl);
	link_mdl_wall_textures(doom->mdl);
	link_mdl_rooms(doom->mdl);
	expand_mdl_polygon_maps(doom->mdl);
	if (doom->edt_quit)
		create_lever_effectors(doom->mdl);
}

static void assign_player_room(t_doom *doom)
{
	int location_id;

	location_id = check_location(doom, doom->mdl->player.x, doom->mdl->player.y);
	doom->mdl->player.room_id = location_id;
	doom->mdl->player.room = room_by_id(location_id);
}

int		mapfile_opens(t_doom *doom, char* map_path)
{
	int opened;

	opened = open(map_path, O_RDONLY);
	if (opened > 1)
	{
		close(opened);
		return (1);
	}	
	return (0);
}

int			load_model(t_doom *doom)
{
	init_model(doom);
	if (!doom->edt_quit)
	{
		if (doom->edt->map_supplied)
			doom->edt->map_exists = mapfile_opens(doom, doom->edt->map_path);
		if (!doom->edt->map_supplied)
		{
			return (1);
		}
		else if (!mapfile_opens(doom, doom->edt->map_path))
		{
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
