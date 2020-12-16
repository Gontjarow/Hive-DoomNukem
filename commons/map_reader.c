#include "doom-nukem.h"

// [Wall] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 190
/*
void 			map_to_model(t_mapfile *map, t_model *mdl)
{
	if (!map->player_string)
		ft_die("Fatal error: map_to_model mapfile player data missing from mapfile.");
	else
		map_player(map, mdl);
	if (map->wall_string) {
		map_walls(map, mdl);
		ft_putendl("Converted walls from map to model at map_to_model");
	}
	if (map->room_string) {
		map_rooms(map, mdl);
		ft_putendl("Converted walls from map to model at map_to_model");
	}
	if (map->portal_string) {
		map_portals(map, mdl);
		ft_putendl("Converted walls from map to model at map_to_model");
	}
	if (map->enemy_string) {
		map_enemies(map, mdl);
		ft_putendl("Converted walls from map to model at map_to_model");
	}
	//printf("%s\n%s\n%s\n%s\n[BIG DEBUG DATA]\n", doom->map->wall_string, doom->map->enemy_string, doom->map->portal_string, doom->map->player_string);
	if (!map->wall_string && !map->room_string && !map->portal_string
			&& !map->enemy_string && !map->player_string)
	{
		ft_putendl("Warning: Map data strings empty at parse_mapfile.");
		doom_ptr()->map->was_filled = 0;
		return ;
	}
}*/