/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 16:04:13 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/26 16:07:30 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void init_img(void)
{
	if (!(IMG_Init(IMG_INIT_PNG)))
		ft_die("Fatal error: IMG_INIT_PNG failure.");
}
static void init_defaults(t_doom *doom)
{
	doom->sounds = NULL;
	doom->sprites = NULL;
	doom->quit = 0;
	doom->edt_quit = 1;
	doom->game_quit = 1;
	doom->minimap_quit = 1;
	doom->menu_out_of_focus = 0;
	doom->edt = NULL;
	doom->keystates = NULL;
	doom->minimap = NULL;
	doom->mdl = NULL;
	doom->map = NULL;
	doom->font_atlas = NULL;
	doom->fps = 30;
}

static char	*parse_exec_name(char *exec_name)
{
	char	*name;
	int		before_slash;
	int 	after_slash;

	before_slash = 0;
	after_slash = 0;
	name = exec_name;
	while (*name != '/')
	{
		before_slash++;
		name++;
	}
	while (*name)
	{
		if (*name == '/')
		{
			before_slash += after_slash;
			after_slash = 0;
		}
		after_slash++;
		name++;
	}
	return (ft_strsub(exec_name, ++before_slash, --after_slash));
}

void		init_doom(t_doom *doom, char *exec_name)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2 video.");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2 audio.");
	if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
		ft_die("Fatal error: SDL_mixer could not initialize!");
	doom->win = SDL_CreateWindow("Doomed 3D", SDL_WINDOWPOS_CENTERED,
								 SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	if (doom->win == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window with SDL_CreateWindow on init_doom.");
	doom->buff = SDL_GetWindowSurface(doom->win);
	if (doom->buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Surface with SDL_GetWindowSurface on init_doom.");
	doom->exec_name = parse_exec_name(exec_name);
	init_defaults(doom);
	set_doom_singleton(doom);
	init_img();
	load_sounds(doom);
	load_sprites(doom);
}

int			destroy_doom_and_quit(t_doom *doom)
{
	free(doom->exec_name);
	destroy_menu(doom);
	if (!doom->edt_quit)
		destroy_edt(doom);
	if (!doom->game_quit)
		destroy_game(doom);
	if (!doom->minimap_quit)
		destroy_minimap(doom);
	if (doom->mdl)
		destroy_model(doom);
	destroy_sounds(doom);
	destroy_sprites(doom);
	SDL_FreeSurface(doom->buff);
	SDL_DestroyWindow(doom->win);
	Mix_Quit();
	SDL_Quit();
	return (0);
}
