/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:14:13 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/11 13:20:30 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

// TODO Resources for textures of wall, ceiling and floor will live in single array
// 	Referenced by ID value which is the index for this array
// 	Defined the size in DEFINE MACROS to allocate the correct size array
// 	Editor will support switching a rooms wall, ceil and floor texture_id
// 	Editor will have ways to preview and look at these texture resources

# define EXECUTABLE_NAME "doom-nukem"
# define OFFSET	11000000
# define ATLAS_SIZE 65102
# define THUNDER_SIZE 540000

void 		load_resources(t_model *mdl)
{
	if (mdl->rsrcs == NULL)
		return ;
		//puts("Resources are uninitialized, can load resources!");
}

void 		convert_thunder_frame(int i, SDL_PixelFormat *fmt, t_doom *doom)
{
	SDL_Surface		*conv;

	conv = SDL_ConvertSurface(doom->menu->thunder[i], fmt, 0);
	SDL_FreeSurface(doom->menu->thunder[i]);
	doom->menu->thunder[i] = conv;
}

void 				load_thunder_frame(int i, SDL_RWops *opened, t_doom *doom)
{
	char 			*thunder_map;
	SDL_RWops		*map_rw;
	int 			seeks;

	seeks = 0;
	SDL_RWseek(opened, OFFSET, RW_SEEK_SET);
	SDL_RWseek(opened, ATLAS_SIZE, RW_SEEK_CUR);
	while (seeks++ < i)
		SDL_RWseek(opened, THUNDER_SIZE, RW_SEEK_CUR);
	thunder_map = (char*)malloc(sizeof(char)*THUNDER_SIZE);
	if (!thunder_map)
		ft_die("Fatal error: Could not malloc at load_appended_atlas.");
	SDL_RWread(opened, (void*)thunder_map, sizeof(char), THUNDER_SIZE / sizeof(char));
	map_rw = SDL_RWFromMem((void*)thunder_map, THUNDER_SIZE);
	if (doom->menu != NULL)
		doom->menu->thunder[i] = IMG_LoadXPM_RW(map_rw);
	if (!doom->menu->thunder)
		ft_die("Fatal error loading bundled thunder_frame XPM file! Incorrect offset or missing appendment!");
	SDL_FreeRW(map_rw);
	free(thunder_map);
}

static SDL_Surface 	*load_bundled_atlas(SDL_RWops *opened, t_doom *doom)
{
	SDL_Surface		*bundled_atlas;
	char			atlas_map[ATLAS_SIZE];
	SDL_RWops		*map_rw;

	SDL_RWseek(opened, OFFSET, RW_SEEK_SET);
	SDL_RWread(opened, (void*)atlas_map, sizeof(char), ATLAS_SIZE / sizeof(char));
	map_rw = SDL_RWFromMem((void*)atlas_map, sizeof(atlas_map));
	bundled_atlas = IMG_LoadXPM_RW(map_rw);
	if (!bundled_atlas)
		ft_die("Fatal error loading bundled robocop_font XPM file! Incorrect offset or missing appendment!");
	SDL_FreeRW(map_rw);
	return (bundled_atlas);
}

int 				load_appended_atlas(t_doom *doom)
{
	SDL_Surface		*bundled_atlas;
	SDL_Surface		*conv;
	SDL_PixelFormat	*fmt;
	SDL_RWops		*opened;
	int 			i;

	opened = SDL_RWFromFile(EXECUTABLE_NAME, "rb");
	if (!opened)
		ft_putendl("Warning: Failed to create RWFromFile!");
	bundled_atlas = load_bundled_atlas(opened, doom);
	i = 0;
	while (i < 10)
		load_thunder_frame(i++, opened, doom);
	SDL_RWclose(opened);
	fmt = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
	conv = SDL_ConvertSurface(bundled_atlas, fmt, 0);
	SDL_FreeSurface(bundled_atlas);
	doom->font_atlas = conv;
	i = 0;
	while (i < 10)
		convert_thunder_frame(i++, fmt, doom);
	SDL_FreeFormat(fmt);
}

// This was used as an experimental feature but it taught me how to do bundling of assets to binary -krusthol
// Set to return (0) by default to bypass any usages for now.
int			load_appended_map(t_doom *doom)
{
	int		opened;
	off_t	seeked;
	char	*line;
	char 	*join;

	return (0);
	if (doom->map == NULL)
		doom->map = init_mapfile();
	opened = open(doom->exec_name, O_RDONLY);
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
