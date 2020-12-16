/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 16:43:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/18 16:54:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

t_mapfile	*init_mapfile(void)
{
	t_mapfile		*map;

	map = (t_mapfile*)malloc(sizeof(t_mapfile));
	if (!map)
		ft_die("Fatal error: Mallocing mapfile struct failed at init_mapfile.");
	map->join_string = NULL;
	map->enemy_string = NULL;
	map->player_string = NULL;
	map->portal_string = NULL;
	map->wall_string = NULL;
	map->room_string = NULL;
	map->was_filled = 0; // Silly...
	//map_data_initialized = 1;
		//puts("MAPFILE INITIALIZED!");
	return (map);
}

void 	destroy_mapfile(t_mapfile *map)
{
	if (map->join_string != NULL)
		free(map->join_string);
	if (map->player_string != NULL)
		free(map->player_string);
	if (map->wall_string != NULL)
		free(map->wall_string);
	if (map->room_string != NULL)
		free(map->room_string);
	if (map->portal_string != NULL)
		free(map->portal_string);
	if (map->enemy_string != NULL)
		free(map->enemy_string);
	map->join_string = NULL;
	map->enemy_string = NULL;
	map->player_string = NULL;
	map->portal_string = NULL;
	map->wall_string = NULL;
	map->room_string = NULL;
	free(map);
	map = NULL;
	//map_data_initialized = 0;
}

void	update_player_string(t_model *mdl, t_mapfile *map)
{
		map->join_string = ft_strnew(255);
		sprintf(map->join_string, "Player spawn: %d %d | rot: %d | tail: %d %d\n",
				(int)mdl->player.x, (int)mdl->player.y, mdl->player.rot, mdl->player.tail.x, mdl->player.tail.y);
		if (map->player_string)
			free(map->player_string);
		map->player_string = ft_strnew(1);
		map->player_string = ft_strjoin(map->player_string, map->join_string);
		free(map->join_string);
		map->join_string = NULL;
}

void	add_enemy_to_string(t_enemy *enemy, t_mapfile *map)
{
		map->join_string = ft_strnew(255);
		sprintf(map->join_string, "Enemy id: %d | start: %d %d | rot: %d | tail: %d %d | hp: %d | wep id: %d\n",
				enemy->id, enemy->x, enemy->y, enemy->rot,
				enemy->tail.x, enemy->tail.y, enemy->hp.max, enemy->wep.type_id);
		if (!map->enemy_string)
			map->enemy_string = ft_strnew(1);
		map->enemy_string = ft_strjoin(map->enemy_string, map->join_string);
		free(map->join_string);
		map->join_string = NULL;
}

void	add_wall_to_string(t_wall *wall, t_mapfile *map)
{
	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "Wall id: %d | start: %d %d | end: %d %d\n",
			wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
	if (!map->wall_string)
		map->wall_string = ft_strnew(1);
	map->wall_string = ft_strjoin(map->wall_string, map->join_string);
	free(map->join_string);
	map->join_string = NULL;
}

void 	add_portal_to_string(t_wall *portal, t_mapfile *map)
{
	map->join_string = ft_strnew(255);
	sprintf(map->join_string, "Portal id: %d | start: %d %d | end: %d %d\n",
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
	sprintf(map->join_string, "Room id: %d | first_wall_id: %d | wall_count: %d | floor_height: %d | roof_height: %d\n",
			room->id, room->first_wall_id, room->wall_count, room->floor_height, room->roof_height);
	if (!map->room_string)
		map->room_string = ft_strnew(1);
	map->room_string = ft_strjoin(map->room_string, map->join_string);
	free(map->join_string);
	map->join_string = NULL;
}

int		write_mapfile(char *map_path, t_mapfile *map)
{
	t_model		*mdl;
	int			opened;
	char		new_line[2];

	if (map->player_string == NULL)
		ft_die("Fatal Error: write_mapfile failure when saving map. No player start position specified.");
	new_line[0] = '\n';
	new_line[1] = '\0';
	mdl = get_model();
	opened = open(map_path, O_WRONLY | O_CREAT, 0666);
	if (opened > 1)
	{
		if (mdl->wall_count > 0)
			write(opened, map->wall_string, ft_strlen(map->wall_string));
		write(opened, new_line, 1);
		if (mdl->room_count > 0)
			write(opened, map->room_string, ft_strlen(map->room_string));
		write(opened, new_line, 1);
		if (mdl->portal_count > 0)
			write(opened, map->portal_string, ft_strlen(map->portal_string));
		write(opened, new_line, 1);
		if (mdl->enemy_count > 0)
			write(opened, map->enemy_string, ft_strlen(map->enemy_string));
		write(opened, new_line, 1);
		write(opened, map->player_string, ft_strlen(map->player_string));
		ft_putstr("Hive-DoomNukem: write_mapfile saved mapdata to mapfile: ");
		ft_putendl(map_path);
		close(opened);
		return (1);
	}
	return (0);
}

int 	overwrite_mapfile(char *map_path, t_mapfile *map)
{
	char *backup;

	backup = ft_strjoin(map_path, ".backup");
	rename(map_path, backup);
	ft_putstr("Hive-DoomNukem: renamed earlier version of mapfile as: ");
	ft_putendl(backup);
	free(backup);
	return (write_mapfile(map_path, map));
}