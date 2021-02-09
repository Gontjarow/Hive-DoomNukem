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
	map->effect_string = NULL;
	map->pickup_string = NULL;
	map->chain_string = NULL;
	map->was_filled = 0;
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
	if (map->effect_string != NULL)
		free(map->effect_string);
	if (map->pickup_string != NULL)
		free(map->pickup_string);
	map->join_string = NULL;
	map->enemy_string = NULL;
	map->player_string = NULL;
	map->portal_string = NULL;
	map->wall_string = NULL;
	map->room_string = NULL;
	map->effect_string = NULL;
	map->pickup_string = NULL;
	free(map);
}

int			stringify_mapfile(t_doom *doom, char *map_path)
{
	int		opened;
	char	*line;
	char 	*join;

	if (doom->map == NULL)
		doom->map = init_mapfile();
	opened = open(map_path, O_RDONLY);
	line = NULL;
	if (opened > 1)
	{
		while (get_next_line(opened, &line))
		{
			if (ft_strlen(line) < 3) {
				free(line);
				continue;
			}
			if (ft_strnstr(line, "[Wall]", 6))
			{
				if (doom->map->wall_string == NULL)
					doom->map->wall_string = ft_strnew(1);
				join = doom->map->wall_string;
				doom->map->wall_string = ft_strjoin(doom->map->wall_string, line);
				free(join);
				join = doom->map->wall_string;
				doom->map->wall_string = ft_strjoin(doom->map->wall_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Room]", 6)) {
				if (doom->map->room_string == NULL)
					doom->map->room_string = ft_strnew(1);
				join = doom->map->room_string;
				doom->map->room_string = ft_strjoin(doom->map->room_string, line);
				free(join);
				join = doom->map->room_string;
				doom->map->room_string = ft_strjoin(doom->map->room_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Portal]", 8)) {
				if (doom->map->portal_string == NULL)
					doom->map->portal_string = ft_strnew(1);
				join = doom->map->portal_string;
				doom->map->portal_string = ft_strjoin(doom->map->portal_string, line);
				free(join);
				join = doom->map->portal_string;
				doom->map->portal_string = ft_strjoin(doom->map->portal_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Enemy]", 7)) {
				if (doom->map->enemy_string == NULL)
					doom->map->enemy_string = ft_strnew(1);
				join = doom->map->enemy_string;
				doom->map->enemy_string = ft_strjoin(doom->map->enemy_string, line);
				free(join);
				join = doom->map->enemy_string;
				doom->map->enemy_string = ft_strjoin(doom->map->enemy_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Effect]", 8)) {
				if (doom->map->effect_string == NULL)
					doom->map->effect_string = ft_strnew(1);
				join = doom->map->effect_string;
				doom->map->effect_string = ft_strjoin(doom->map->effect_string, line);
				free(join);
				join = doom->map->effect_string;
				doom->map->effect_string = ft_strjoin(doom->map->effect_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Pickup]", 8)) {
				if (doom->map->pickup_string == NULL)
					doom->map->pickup_string = ft_strnew(1);
				join = doom->map->pickup_string;
				doom->map->pickup_string = ft_strjoin(doom->map->pickup_string, line);
				free(join);
				join = doom->map->pickup_string;
				doom->map->pickup_string = ft_strjoin(doom->map->pickup_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Player]", 8)){
				if (doom->map->player_string == NULL)
					doom->map->player_string = ft_strnew(1);
				join = doom->map->player_string;
				doom->map->player_string = ft_strjoin(doom->map->player_string, line);
				free(join);
				join = doom->map->player_string;
				doom->map->player_string = ft_strjoin(doom->map->player_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Chain_to_mapfile]", 18)){
				if (doom->map->chain_string == NULL)
					doom->map->chain_string = ft_strnew(1);
				join = doom->map->chain_string;
				doom->map->chain_string = ft_strjoin(doom->map->chain_string, line);
				free(join);
				join = doom->map->chain_string;
				doom->map->chain_string = ft_strjoin(doom->map->chain_string, "\n");
				free(join);
			}
			free(line);
		}
		close(opened);
		return (1);
	}
	return (0);
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
		{
			write(opened, map->wall_string, ft_strlen(map->wall_string));
			write(opened, new_line, 1);
		}
		if (mdl->room_count > 0)
		{
			write(opened, map->room_string, ft_strlen(map->room_string));
			write(opened, new_line, 1);
		}
		if (mdl->portal_count > 0)
		{
			write(opened, map->portal_string, ft_strlen(map->portal_string));
			write(opened, new_line, 1);
		}
		if (mdl->enemy_count > 0)
		{
			write(opened, map->enemy_string, ft_strlen(map->enemy_string));
			write(opened, new_line, 1);
		}
		if (mdl->effect_count > 0)
		{
			write(opened, map->effect_string, ft_strlen(map->effect_string));
			write(opened, new_line, 1);
		}
		else
			ft_putendl("Warning: No level exit defined for map! Map is infinite and flagged as suitable for debugging.");
		if (mdl->pickup_count > 0)
		{
			write(opened, map->pickup_string, ft_strlen(map->pickup_string));
			write(opened, new_line, 1);
		}
		if (map->chain_string)
		{
			write(opened, map->chain_string, ft_strlen(map->chain_string));
			write(opened, new_line, 1);
		}
		write(opened, map->player_string, ft_strlen(map->player_string));
		if (mdl->effect_count == 0)
			write(opened, DEBUGGING_USE_FLAG, ft_strlen(DEBUGGING_USE_FLAG));
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