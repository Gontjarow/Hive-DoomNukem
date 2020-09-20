/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_model.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 17:13:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/18 17:34:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void scan_walls(t_doom *doom, t_model *mdl)
{
	int 	safety;
	int		tokens;
	int 	dist;
	char 	*strchr;
	char 	*next;
	char	*sub;
	t_wall 	*next_wall;

	next = NULL;
	safety = 100;
	strchr = ft_strchr(doom->map.wall_string, '\n');
	dist = strchr - doom->map.wall_string;
	sub = ft_strsub(doom->map.wall_string, 0, dist);
	while (ft_strlen(strchr) > 0 || !safety--)
	{
		tokens = sscanf(sub, "Wall id: %d | start: %d %d | end: %d %d\n",
						&mdl->walls->id, &mdl->walls->start.x, &mdl->walls->start.y, &mdl->walls->end.x,
						&mdl->walls->end.y);
		if (tokens != 5)
		{
			//ft_putendl("Warning: sscanf did not find 5 tokens from wall_string.");
			break ;
		}
		else
		{
			next_wall = (t_wall*)malloc(sizeof(t_wall));
			if (!next_wall)
				ft_die("Fatal error: Could not malloc t_wall at scan_walls.");
			if (mdl->wall_count++ == 0)
				mdl->wall_first = mdl->walls;
			mdl->walls->next = next_wall;
			mdl->walls = next_wall;
		}
		if (next != NULL)
			strchr = next;
		if (ft_strlen(strchr) < 2)
			break;
		strchr++;
		next = ft_strchr(strchr, '\n');
		dist = next - strchr;
		free(sub);
		sub = ft_strsub(strchr, 0, dist);
	}
	if (!safety)
		ft_die("Fatal error: scan_walls terminated due to safety valve.");
	free(sub);
}

static void scan_portals(t_doom *doom, t_model *mdl)
{
	int 	safety;
	int		tokens;
	int 	dist;
	char 	*strchr;
	char 	*next;
	char	*sub;
	t_wall 	*next_portal;

	next = NULL;
	safety = 100;
	strchr = ft_strchr(doom->map.portal_string, '\n');
	dist = strchr - doom->map.portal_string;
	sub = ft_strsub(doom->map.portal_string, 0, dist);
	while (ft_strlen(strchr) > 0 || !safety--)
	{
		tokens = sscanf(sub, "Portal id: %d | start: %d %d | end: %d %d\n",
						&mdl->portals->id, &mdl->portals->start.x, &mdl->portals->start.y, &mdl->portals->end.x,
						&mdl->portals->end.y);
		if (tokens != 5)
		{
			//ft_putendl("Warning: sscanf did not find 5 tokens from portal_string.");
			break ;
		}
		else {
			next_portal = (t_wall *) malloc(sizeof(t_wall));
			if (!next_portal)
				ft_die("Fatal error: Could not malloc t_wall at scan_portals.");
			if (mdl->portal_count++ == 0)
				mdl->portal_first = mdl->portals;
			mdl->portals->next = next_portal;
			mdl->portals = next_portal;
		}
		if (next != NULL)
			strchr = next;
		if (ft_strlen(strchr) < 2)
			break;
		strchr++;
		next = ft_strchr(strchr, '\n');
		dist = next - strchr;
		free(sub);
		sub = ft_strsub(strchr, 0, dist);
	}
	if (!safety)
		ft_die("Fatal error: scan_portals terminated due to safety valve.");
	free(sub);
}

static void scan_enemies(t_doom *doom, t_model *mdl)
{
	int 	safety;
	int		tokens;
	int 	dist;
	char 	*strchr;
	char 	*next;
	char	*sub;
	t_enemy	*next_enemy;

	next = NULL;
	safety = 100;
	strchr = ft_strchr(doom->map.enemy_string, '\n');
	dist = strchr - doom->map.enemy_string;
	sub = ft_strsub(doom->map.enemy_string, 0, dist);
	while (ft_strlen(strchr) > 0 || !safety--)
	{
		tokens = sscanf(sub, "Enemy id: %d | start: %d %d | rot: %d | tail: %d %d | hp: %d | wep id: %d\n",
						&mdl->enemies->id, &mdl->enemies->x, &mdl->enemies->y, &mdl->enemies->rot,
						&mdl->enemies->tail.x, &mdl->enemies->tail.y, &mdl->enemies->hp.max, &mdl->enemies->wep.type_id);
		if (tokens != 8)
		{
			//ft_putendl("Warning: sscanf did not find 8 tokens from enemy_string.");
			break ;
		}
		else
		{
			next_enemy = (t_enemy*)malloc(sizeof(t_enemy));
			if (!next_enemy)
				ft_die("Fatal error: Could not malloc t_enemy at scan_enemies.");
			if (mdl->enemy_count++ == 0)
				mdl->enemy_first = mdl->enemies;
			mdl->enemies->next = next_enemy;
			mdl->enemies = next_enemy;
		}
		if (next != NULL)
			strchr = next;
		if (ft_strlen(strchr) < 2)
			break;
		strchr++;
		next = ft_strchr(strchr, '\n');
		dist = next - strchr;
		free(sub);
		sub = ft_strsub(strchr, 0, dist);
	}
	if (!safety)
		ft_die("Fatal error: scan_enemies terminated due to safety valve.");
	free(sub);
}

static void parse_mapfile(t_doom *doom, t_model *mdl)
{
	// ULTRA WARNING!!!
	// SSCANF is forbidden. Will need to write similar functionality by hand before return of project.
	//  - krusthol
	int tokens;

	doom->map.was_filled = 1;
	if (doom->map.wall_string)
		scan_walls(doom, mdl);
	if (doom->map.portal_string)
		scan_portals(doom, mdl);
	if (doom->map.enemy_string)
		scan_enemies(doom, mdl);
	if (!doom->map.wall_string && !doom->map.portal_string && !doom->map.enemy_string && !doom->map.player_string)
	{
		ft_putendl("Warning: Map data strings empty at parse_mapfile.");
		doom->map.was_filled = 0;
		return ;
	}
	if (!doom->map.player_string)
		ft_die("Fatal error: parse_mapfile player data missing from mapfile.");
	tokens = sscanf(doom->map.player_string, "Player spawn: %d %d | rot: %d | tail: %d %d\n",
		   &mdl->player.x, &mdl->player.y, &mdl->player.rot, &mdl->player.tail.x, &mdl->player.tail.y);
	if (tokens != 5)
		ft_die("Fatal error: parse_mapfile player tokens from mapfile did not equal to 5.");
}

static char *expand_string(char *base, char *add)
{
	char *join;
	char *nl;

	if (base == NULL)
		base = ft_strnew(1);
	join = base;
	base = ft_strjoin(base, add);
	free(join);
	free(add);
	nl = ft_strnew(1);
	nl[0] = '\n';
	join = base;
	base = ft_strjoin(base, nl);
	free(join);
	free(nl);
	return (base);
}

static int read_mapfile(t_doom *doom, char *map_path)
{
	int		opened;
	char	*line;

	opened = open(map_path, O_RDONLY);
	if (opened > 1)
	{
		while (get_next_line(opened, &line))
		{
			if (ft_strlen(line) < 3)
				continue;
			if (ft_strnstr(line, "Wal", 3))
				doom->map.wall_string = expand_string(doom->map.wall_string, line);
			else if (ft_strnstr(line, "Por", 3))
				doom->map.portal_string = expand_string(doom->map.portal_string, line);
			else if (ft_strnstr(line, "Ene", 3))
				doom->map.enemy_string = expand_string(doom->map.enemy_string, line);
			else if (ft_strnstr(line, "Pla", 3))
				doom->map.player_string = expand_string(doom->map.player_string, line);
			else
				free(line);
		}
		close(opened);
		return (1);
	}
	ft_putendl("Warning: read_mapfile failed to open supplied map path:");
	ft_putendl(map_path);
	return (0);
}

static void init_model(t_doom *doom)
{
	doom->mdl = (t_model*)malloc(sizeof(t_model));
	if (!doom->mdl)
		ft_die("Fatal error: Mallocing model struct failed at init_model.");
	doom->mdl->parent = doom;
	doom->mdl->walls = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->mdl->walls)
		ft_die("Fatal error: Mallocing walls struct failed at init_model.");
	doom->mdl->portals = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->mdl->portals)
		ft_die("Fatal error: Mallocing portals struct failed at init_model.");
	doom->mdl->enemies = (t_enemy*)malloc(sizeof(t_enemy));
	if (!doom->mdl->enemies)
		ft_die("Fatal error: Mallocing enemies struct failed at init_model.");
	doom->mdl->wall_first = NULL;
	doom->mdl->portal_first = NULL;
	doom->mdl->enemy_first = NULL;
	doom->mdl->wall_count = 0;
	doom->mdl->portal_count = 0;
	doom->mdl->enemy_count = 0;
}

void 		destroy_model(t_doom *doom)
{
	// WARNING! INCOMPLETE MEMORY MANAGEMENT
	// NEEDS TO ITERATE THROUGH LINKED LISTS AND DESTROY THEM SYSTEMATICALLY
	// - krusthol
	free(doom->mdl->enemies);
	free(doom->mdl->portals);
	free(doom->mdl->walls);
	doom->mdl->wall_first = NULL;
	doom->mdl->portal_first = NULL;
	doom->mdl->enemy_first = NULL;
	free(doom->mdl);
	doom->mdl = NULL;
}

int			load_model(t_doom *doom)
{
	init_model(doom);
	if (!doom->edt_quit)
	{
		if (!doom->edt->load_map)
			return (0);
		if (!read_mapfile(doom, doom->edt->map_path))
			return (0);
		ft_putstr("Loaded mapfile data from file: ");
		ft_putendl(doom->edt->map_path);
		parse_mapfile(doom, doom->mdl);
	}
	else if (!doom->game_quit) {
		if (!doom->game->map_supplied)
			return (0);
		if (!read_mapfile(doom, doom->game->map_path))
			return (0);
		ft_putstr("Loaded mapfile data from file: ");
		ft_putendl(doom->game->map_path);
		parse_mapfile(doom, doom->mdl);
	}
	return (1);
}