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