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

void 				load_resources(t_model *mdl)
{
	if (mdl->rsrcs == NULL)
		return ;
		//puts("Resources are uninitialized, can load resources!");
}

void 				convert_thunder_frame(int i, SDL_PixelFormat *fmt, t_doom *doom)
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
