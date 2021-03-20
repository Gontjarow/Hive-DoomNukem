/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 13:14:13 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/11 13:20:30 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"
#define EXECUTABLE_NAME "doom-nukem"

// Makefile cat blobbing ideas
// Compile binary
// Save size of binary with ls -la to a Makefile scripting temp variable
// Append executable binary file with data using "cat datafile >> binaryfile"
// Append executable binary file with the original compiled size from the temp variable

// Resources for textures of wall, ceiling and floor will live in single array
// Referenced by ID value which is the index for this array
// Defined the size in DEFINE MACROS to allocate the correct size array
// Editor will support switching a rooms wall, ceil and floor texture_id
// Editor will have ways to preview and look at these texture resources
// TODO FIX TO DYNAMIC OFFSET INSTEAD OF LS -LA HANDCALCULATED ONE
# define OFFSET 10639472
// TODO USE SPRITE ATLAS FOR LOADING RESOURCES
// TODO CHANGE FROM DEFINES TO DYNAMIC IN EXECUTABLE NAME MAKE DOOM->EXEC_NAME FROM ARGV[0]
//if (!(doom->sprites->txt_boss_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
//	return (-1);
//if (!(doom->sprites->txt_boss_back_walk_atlas = load_xpm("img/sprites/boss/back/walk.xpm")))
//	return (-1);
//printf("Loaded back walk atlas W: %d H: %d\n", doom->sprites->txt_boss_back_walk_atlas->w, doom->sprites->txt_boss_back_walk_atlas->h);
//i = '0';
//doom->sprites->txt_boss_back_walk[0] = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 32, SDL_PIXELFORMAT_ARGB32);
//copy_from_atlas(0, 0, doom->sprites->txt_boss_back_walk_atlas, doom->sprites->txt_boss_back_walk[0]);
//while (i < '6')
//{
//	doom->sprites->txt_boss_back_walk[(i - 48)] = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 32, SDL_PIXELFORMAT_ARGB32);
//	if (doom->sprites->txt_boss_back_walk[(i - 48)] == NULL)
//		ft_die("Fatal error: Could not malloc SDL_Surface for sprite");
//	copy_from_atlas(128 * (i - 48), 0, doom->sprites->txt_boss_back_walk_atlas, doom->sprites->txt_boss_back_walk[(i - 48)]);
//	i++;
//}

void 		load_resources(t_model *mdl)
{
	if (mdl->rsrcs == NULL)
		return ;
		//puts("Resources are uninitialized, can load resources!");
}

int			load_appended(t_doom *doom)
{
	int		opened;
	off_t	seeked;
	char	*line;
	char 	*join;

	if (doom->map == NULL)
		doom->map = init_mapfile();
	opened = open(EXECUTABLE_NAME, O_RDONLY);
	line = NULL;
	if (opened > 1)
	{
		seeked = lseek(opened, OFFSET, SEEK_SET);
		if (seeked == -1)
		{
			ft_putendl("Error offsetting when reading appended data!");
			return (0);
		}
		off_t old_position = lseek(opened, 0, SEEK_CUR);
		off_t end_position = lseek(opened, 0, SEEK_END);
		if (old_position == end_position)
		{
			ft_putendl("No appended data detected! Initializing a blank map.");
			return (0);
		}
		ft_putendl("Appended data detected!");
		lseek(opened, OFFSET, SEEK_SET);
		while (get_next_line(opened, &line))
		{
			if (ft_strlen(line) < 3 || ft_strlen(line) > 255)
			{
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
			} else if (ft_strnstr(line, "[Room]", 6))
			{
				if (doom->map->room_string == NULL)
					doom->map->room_string = ft_strnew(1);
				join = doom->map->room_string;
				doom->map->room_string = ft_strjoin(doom->map->room_string, line);
				free(join);
				join = doom->map->room_string;
				doom->map->room_string = ft_strjoin(doom->map->room_string, "\n");
				free(join);
			} else if (ft_strnstr(line, "[Portal]", 8))
			{
				if (doom->map->portal_string == NULL)
					doom->map->portal_string = ft_strnew(1);
				join = doom->map->portal_string;
				doom->map->portal_string = ft_strjoin(doom->map->portal_string, line);
				free(join);
				join = doom->map->portal_string;
				doom->map->portal_string = ft_strjoin(doom->map->portal_string, "\n");
				free(join);
			} else if (ft_strnstr(line, "[Enemy]", 7))
			{
				if (doom->map->enemy_string == NULL)
					doom->map->enemy_string = ft_strnew(1);
				join = doom->map->enemy_string;
				doom->map->enemy_string = ft_strjoin(doom->map->enemy_string, line);
				free(join);
				join = doom->map->enemy_string;
				doom->map->enemy_string = ft_strjoin(doom->map->enemy_string, "\n");
				free(join);
			} else if (ft_strnstr(line, "[Effect]", 8))
			{
				if (doom->map->effect_string == NULL)
					doom->map->effect_string = ft_strnew(1);
				join = doom->map->effect_string;
				doom->map->effect_string = ft_strjoin(doom->map->effect_string, line);
				free(join);
				join = doom->map->effect_string;
				doom->map->effect_string = ft_strjoin(doom->map->effect_string, "\n");
				free(join);
			} else if (ft_strnstr(line, "[Pickup]", 8))
			{
				if (doom->map->pickup_string == NULL)
					doom->map->pickup_string = ft_strnew(1);
				join = doom->map->pickup_string;
				doom->map->pickup_string = ft_strjoin(doom->map->pickup_string, line);
				free(join);
				join = doom->map->pickup_string;
				doom->map->pickup_string = ft_strjoin(doom->map->pickup_string, "\n");
				free(join);
			} else if (ft_strnstr(line, "[Player]", 8))
			{
				if (doom->map->player_string == NULL)
					doom->map->player_string = ft_strnew(1);
				join = doom->map->player_string;
				doom->map->player_string = ft_strjoin(doom->map->player_string, line);
				free(join);
				join = doom->map->player_string;
				doom->map->player_string = ft_strjoin(doom->map->player_string, "\n");
				free(join);
			} else if (ft_strnstr(line, "[Chain_to_mapfile]", 18))
			{
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
