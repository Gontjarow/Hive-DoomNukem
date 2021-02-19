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

// TODO Resources for textures of wall, ceiling and floor will live in single array
// 	Referenced by ID value which is the index for this array
// 	Defined the size in DEFINE MACROS to allocate the correct size array
// 	Editor will support switching a rooms wall, ceil and floor texture_id
// 	Editor will have ways to preview and look at these texture resources

# define EXECUTABLE_NAME "doom-nukem"
// TODO CHANGE FROM DEFINES TO DYNAMIC IN EXECUTABLE NAME MAKE DOOM->EXEC_NAME FROM ARGV[0]
# define OFFSET 10645720
# define ATLAS_SIZE 65102
# define THUNDER_SIZE 1060469
# define TOTAL_SIZE 11771291
// TODO FIX TO DYNAMIC OFFSET INSTEAD OF LS -LA HANDCALCULATED ONE
// TODO FIX TO MEMORYMAPPED AREAS OF FILE WITH PREDEFINED BYTE SIZES

void 		load_resources(t_model *mdl)
{
	if (mdl->rsrcs == NULL)
		return ;
		//puts("Resources are uninitialized, can load resources!");
}

int 		load_appended_atlas(t_doom *doom)
{
	// TODO FILL OUT LOAD_APPENDED WITH AUTOMATIC LOADING OF ATLAS XPM FONT RESOURCE AFTER THE OFFSET BYTE
	SDL_Surface		*test;
	SDL_Surface		*conv;
	SDL_PixelFormat	*fmt;
	SDL_RWops		*opened;
	SDL_RWops		*map_rw;
	char			atlas_map[ATLAS_SIZE];
	char 			thunder_map[THUNDER_SIZE];

	// LOADING FONT ATLAS WITH SDL RWOPS METHOD
	opened = SDL_RWFromFile(EXECUTABLE_NAME, "rb");
	if (!opened)
		ft_putendl("Warning: Failed to create RWFromFile!");
	SDL_RWseek(opened, OFFSET, RW_SEEK_SET);
	SDL_RWread(opened, (void*)atlas_map, sizeof(char), ATLAS_SIZE / sizeof(char));
	//test = IMG_Load(&atlas_map);
	map_rw = SDL_RWFromMem((void*)atlas_map, sizeof(atlas_map));
	//SDL_RWseek(opened, ATLAS_SIZE, RW_SEEK_END);
	test = IMG_LoadXPM_RW(map_rw);
	if (!test)
	{
		printf("IMG_LoadXPM_RW: %s\n", IMG_GetError());
		ft_die("Fatal error loading atlas XPM!");
	}
	SDL_FreeRW(map_rw);

	// LOADING THUNDER WITH THE SAME METHOD
	SDL_RWseek(opened, OFFSET, RW_SEEK_SET);
	SDL_RWseek(opened, ATLAS_SIZE, RW_SEEK_CUR);
	SDL_RWread(opened, (void*)thunder_map, sizeof(char), THUNDER_SIZE / sizeof(char));
	map_rw = SDL_RWFromMem((void*)thunder_map, sizeof(thunder_map));
	if (doom->menu != NULL)
		doom->menu->thunder = IMG_LoadXPM_RW(map_rw);
	if (!doom->menu->thunder)
	{
		printf("IMG_LoadXPM_RW: %s\n", IMG_GetError());
		ft_die("Fatal error loading thunder XPM!");
	}
	SDL_FreeRW(map_rw);
	SDL_RWclose(opened);

	// Converting surfaces to ARGB8888 format
	fmt = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
	conv = SDL_ConvertSurface(test, fmt, 0);
	SDL_FreeSurface(test);
	doom->font_atlas = conv;
	conv = SDL_ConvertSurface(doom->menu->thunder, fmt, 0);
	SDL_FreeSurface(doom->menu->thunder);
	doom->menu->thunder = conv;
	SDL_FreeFormat(fmt);
}

// TODO REMOVE AND EXTRACT PROOF OF CONCEPT IDEAS INTO FRESH WRITTEN NEW FUNCTION
int			load_appended_map(t_doom *doom)
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
