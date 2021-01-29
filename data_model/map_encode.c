#include "doom-nukem.h"

//[Wall] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 310
//[Room] id = 0 | first_wall_id = 0 | wall_count = 3 | floor_height = 1000 | roof_height = 1300
//[Portal] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 310
//[Enemy] id = 0 | x = 335 | y = 278 | rot = 7 | hp = 100 | wep.type_id = 0
//[Player] spawn.x = 100 | spawn.y = 100 |  rot = 90

void	update_player_string(t_model *mdl, t_mapfile *map)
{
	char *tmp;

	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "[Player] spawn.x = %d | spawn.y = %d | rot = %d\n",
			(int)mdl->player.x, (int)mdl->player.y, mdl->player.rot);
	//sprintf(map->join_string, "Player spawn: %d %d | rot: %d | tail: %d %d\n",
	//		(int)mdl->player.x, (int)mdl->player.y, mdl->player.rot, mdl->player.tail.x, mdl->player.tail.y);
	if (map->player_string)
		free(map->player_string);
	map->player_string = ft_strnew(1);
	tmp = map->player_string;
	map->player_string = ft_strjoin(map->player_string, map->join_string);
	free(map->join_string);
	free(tmp);
	map->join_string = NULL;
}

void	update_chain_string(t_model *mdl, t_mapfile *map)
{
	char *tmp;

	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "[Chain_to_mapfile] %s\n", mdl->chain);
	if (map->chain_string)
		free(map->chain_string);
	map->chain_string = ft_strnew(1);
	tmp = map->chain_string;
	map->chain_string = ft_strjoin(map->chain_string, map->join_string);
	free(map->join_string);
	free(tmp);
	map->join_string = NULL;
}

void	add_enemy_to_string(t_enemy *enemy, t_mapfile *map)
{
	map->join_string = ft_strnew(255);

	sprintf(map->join_string, "[Enemy] id = %d | x = %d | y = %d | rot = %d | hp = %d | wep.type_id = %d | ai.type_id = %d | sprite_id = %d\n",
			enemy->id, enemy->x, enemy->y, enemy->rot, enemy->hp.max, enemy->wep.type_id, enemy->ai.type_id, 0);
	//TODO REPLACE OR REMOVE 0 WITH ENEMY->SPRITE_ID IF GOING TO USE THAT FIELD
	if (!map->enemy_string)
		map->enemy_string = ft_strnew(1);
	map->enemy_string = ft_strjoin(map->enemy_string, map->join_string);
	free(map->join_string);
	map->join_string = NULL;
}

void	add_wall_to_string(t_wall *wall, t_mapfile *map)
{
	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "[Wall] id = %d | start.x = %d | start.y = %d | end.x = %d | end.y = %d\n",
			wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
	if (!map->wall_string)
		map->wall_string = ft_strnew(1);
	map->wall_string = ft_strjoin(map->wall_string, map->join_string);
	free(map->join_string);
	map->join_string = NULL;
}

void	add_pickup_to_string(t_pickup *pickup, t_mapfile *map)
{
	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "[Pickup] id = %d | loc.x = %d | loc.y = %d | flavor = %d | weapon_type_id = %d\n",
			pickup->id, pickup->loc.x, pickup->loc.y, pickup->flavor, pickup->weapon_type_id);
	if (!map->pickup_string)
		map->pickup_string = ft_strnew(1);
	map->pickup_string = ft_strjoin(map->pickup_string, map->join_string);
	free(map->join_string);
	map->join_string = NULL;
}

void 	add_portal_to_string(t_wall *portal, t_mapfile *map)
{
	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "[Portal] id = %d | start.x = %d | start.y = %d | end.x = %d | end.y = %d\n",
			portal->id, portal->start.x, portal->start.y, portal->end.x, portal->end.y);
	if (!map->portal_string)
		map->portal_string = ft_strnew(1);
	map->portal_string = ft_strjoin(map->portal_string, map->join_string);
	free(map->join_string);
	map->join_string = NULL;
}

void	add_room_to_string(t_room *room, t_mapfile *map)
{
	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "[Room] id = %d | first_wall_id = %d | wall_count = %d | floor_height = %d | roof_height = %d\n",
			room->id, room->first_wall_id, room->wall_count, room->floor_height, room->roof_height);
	if (!map->room_string)
		map->room_string = ft_strnew(1);
	map->room_string = ft_strjoin(map->room_string, map->join_string);
	free(map->join_string);
	map->join_string = NULL;
}