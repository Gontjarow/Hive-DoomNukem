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

void	update_player_string(t_editor *edt)
{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Player spawn: %d %d | rot: %d | tail: %d %d\n",
				edt->player.x, edt->player.y, edt->player.rot, edt->player.tail.x, edt->player.tail.y);
		if (edt->player_string)
			free(edt->player_string);
		edt->player_string = ft_strnew(1);
		edt->player_string = ft_strjoin(edt->player_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
}

void	expand_enemy_string(t_editor *edt)
{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Enemy id: %d | start: %d %d | rot: %d | tail: %d %d | hp: %d | wep id: %d\n",
				edt->enemies->id, edt->enemies->x, edt->enemies->y, edt->enemies->rot,
				edt->enemies->tail.x, edt->enemies->tail.y, edt->enemies->hp.max, edt->enemies->wep.type_id);
		if (!edt->enemy_string)
			edt->enemy_string = ft_strnew(1);
		edt->enemy_string = ft_strjoin(edt->enemy_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
}

void	add_wall_to_string(t_editor *edt, t_wall *wall)
{
	edt->join_string = ft_strnew(255);
	sprintf(edt->join_string, "Wall id: %d | start: %d %d | end: %d %d\n",
			wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
	if (!edt->wall_string)
		edt->wall_string = ft_strnew(1);
	edt->wall_string = ft_strjoin(edt->wall_string, edt->join_string);
	free(edt->join_string);
	edt->join_string = NULL;
}

void	expand_wall_string(t_editor *edt)
{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Wall id: %d | start: %d %d | end: %d %d\n",
				edt->walls->id, edt->walls->start.x, edt->walls->start.y, edt->walls->end.x, edt->walls->end.y);
		if (!edt->wall_string)
			edt->wall_string = ft_strnew(1);
		edt->wall_string = ft_strjoin(edt->wall_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
}

void	add_room_to_string(t_editor *edt, t_room *room)
{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Room id: %d | first_wall_id: %d | wall_count: %d | floor_height: %d | roof_height: %d\n",
				room->id, room->first_wall_id, room->wall_count, room->floor_height, room->roof_height);
		if (!edt->room_string)
			edt->room_string = ft_strnew(1);
		edt->room_string = ft_strjoin(edt->room_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
}

void 	expand_room_string(t_editor *edt)
{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Room id: %d | first_wall_id: %d | wall_count: %d | floor_height: %d | roof_height: %d\n",
				edt->rooms->id, edt->rooms->first_wall_id, edt->rooms->wall_count, edt->rooms->floor_height, edt->rooms->roof_height);
		if (!edt->room_string)
			edt->room_string = ft_strnew(1);
		edt->room_string = ft_strjoin(edt->room_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
}

void 	add_portal_to_string(t_editor *edt, t_wall *portal)
{
	edt->join_string = ft_strnew(255);
	sprintf(edt->join_string, "Portal id: %d | start: %d %d | end: %d %d\n",
			portal->id, portal->start.x, portal->start.y, portal->end.x, portal->end.y);
	if (!edt->portal_string)
		edt->portal_string = ft_strnew(1);
	edt->portal_string = ft_strjoin(edt->portal_string, edt->join_string);
	free(edt->join_string);
	edt->join_string = NULL;
}

void	expand_portal_string(t_editor *edt)
{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Portal id: %d | start: %d %d | end: %d %d\n",
				edt->portals->id, edt->portals->start.x, edt->portals->start.y, edt->portals->end.x, edt->portals->end.y);
		if (!edt->portal_string)
			edt->portal_string = ft_strnew(1);
		edt->portal_string = ft_strjoin(edt->portal_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
}

int		write_mapfile(t_editor *edt)
{
	int opened;
	char new_line[2];

	if (!edt->player_set)
		ft_die("Fatal Error: write_mapfile failure when saving map. No player start position specified in editor.");
	new_line[0] = '\n';
	new_line[1] = '\0';
	if (!edt->write_maps)
		return (0);
	opened = open(edt->map_path, O_WRONLY | O_CREAT, 0666);
	if (opened > 1)
	{
		if (edt->wall_count > 0)
			write(opened, edt->wall_string, ft_strlen(edt->wall_string));
		write(opened, new_line, 1);
		if (edt->room_count > 0)
			write(opened, edt->room_string, ft_strlen(edt->room_string));
		write(opened, new_line, 1);
		if (edt->portal_count > 0)
			write(opened, edt->portal_string, ft_strlen(edt->portal_string));
		write(opened, new_line, 1);
		if (edt->enemy_count > 0)
			write(opened, edt->enemy_string, ft_strlen(edt->enemy_string));
		write(opened, new_line, 1);
		write(opened, edt->player_string, ft_strlen(edt->player_string));
		ft_putstr("Hive-DoomNukem: write_mapfile saved mapdata to mapfile: ");
		ft_putendl(edt->map_path);
		close(opened);
		return (1);
	}
	return (0);
}

int 	overwrite_mapfile(t_editor *edt)
{
	char *backup;

	backup = ft_strjoin(edt->map_path, ".backup");
	rename(edt->map_path, backup);
	ft_putstr("Hive-DoomNukem: renamed earlier version of mapfile as: ");
	ft_putendl(backup);
	free(backup);
	edt->write_maps = 1;
	write_mapfile(edt);
	edt->write_maps = 0;
	return (1);
}

