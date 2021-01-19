/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom-nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/22 17:07:30 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		ft_die(const char *error_message)
{
	ft_putendl(error_message);
	exit(0);
}

void		ft_assert(int eval, const char *error_message)
{
	if (eval == 0)
	{
		ft_putendl(error_message);
		exit(0);
	}
}

static void	init_doom(t_doom *doom)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2 video.");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2 audio.");
	if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
		ft_die("Fatal error: SDL_mixer could not initialize!");
	doom->win = SDL_CreateWindow("Hive-DoomNukem", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	if (doom->win == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window with SDL_CreateWindow on init_doom.");
	doom->buff = SDL_GetWindowSurface(doom->win);
	if (doom->buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Surface with SDL_GetWindowSurface on init_doom.");
	load_sounds(doom);
	doom->quit = 0;
	doom->edt_quit = 1;
	doom->game_quit = 1;
	doom->minimap_quit = 1;
	doom->menu_out_of_focus = 0;
	doom->edt = NULL;
	doom->keystates = NULL;
	doom->minimap = NULL;
	doom->mdl = NULL;
	doom->app_start = SDL_GetTicks();
	doom->map_data_initialized = 0;
}

static int	destroy_and_quit(t_doom *doom)
{
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
	SDL_FreeSurface(doom->buff);
	SDL_DestroyWindow(doom->win);
	Mix_Quit();
	SDL_Quit();
	return (0);
}

static void distribute_inputs(t_doom *doom)
{
	while (SDL_PollEvent(&doom->event) != 0)
	{
		if (doom->event.type == SDL_MOUSEMOTION && !doom->edt_quit &&
			doom->event.window.windowID == SDL_GetWindowID(doom->edt->win))
			edt_mouse_motion(doom);
		else if ((doom->event.type == SDL_MOUSEBUTTONDOWN || doom->event.type == SDL_MOUSEWHEEL)
		&& !doom->edt_quit && doom->event.window.windowID == SDL_GetWindowID(doom->edt->win))
			edt_mouse_down(doom);
		else if (doom->event.type == SDL_MOUSEMOTION && !doom->game_quit &&
				 doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
			game_mouse_motion(doom);
		else if (doom->event.type == SDL_MOUSEBUTTONDOWN && !doom->game_quit &&
				 doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
			game_mouse_down(doom);
		else
			window_and_menu_events(doom);
	}
}

static void run_loops(t_doom *doom, int argc, char **argv)
{
	main_menu_loop(doom, argc, argv);
	if (!doom->edt_quit)
		edt_render(doom);
	if (!doom->game_quit)
		game_render(doom);
}

int			main(int argc, char *argv[])
{
	t_doom			doom;
	uint32_t		frame_ticks;

	init_doom(&doom);
	set_doom_singleton(&doom);
	init_menu(&doom);
	while (!doom.quit)
	{
		doom.keystates = SDL_GetKeyboardState(NULL);
		doom.frame_start = SDL_GetTicks();
		// Distribute inputs via SDL_Events, also handle window management with window_and_menu_events()
		distribute_inputs(&doom);
		run_loops(&doom, argc, argv);
		// Delay until next frame
		frame_ticks = SDL_GetTicks() - doom.frame_start;
		if (frame_ticks < TICKS_PER_FRAME)
		{
			SDL_Delay(TICKS_PER_FRAME - frame_ticks);
		}
		SDL_UpdateWindowSurface(doom.win);
	}
	return (destroy_and_quit(&doom));
}
