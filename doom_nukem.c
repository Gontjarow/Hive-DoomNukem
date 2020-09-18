/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/11 14:02:23 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		ft_die(const char *error_message)
{
	ft_putendl(error_message);
	exit(0);
}

static void load_sounds(t_doom *doom)
{
	if (!(doom->sounds = (t_sounds*)malloc(sizeof(t_sounds))))
		ft_die("Fatal error: Failed mallocing doom->sounds at init_doom.");
	if (!(doom->sounds->mcThunder = Mix_LoadWAV(WAV_THUNDER)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_THUNDER!");
	if (!(doom->sounds->mcSteam = Mix_LoadWAV(WAV_STEAM0)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_STEAM0!");
	if (!(doom->sounds->mcPlop = Mix_LoadWAV(WAV_PLOP)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PLOP!");
	if (!(doom->sounds->mcSword = Mix_LoadWAV(WAV_SWORD)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SWORD!");
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
	doom->menu_out_of_focus = 0;
	doom->edt = NULL;
	doom->keystates = NULL;
	doom->app_start = SDL_GetTicks();
}

static void	destroy_sounds(t_doom *doom)
{
	Mix_FreeChunk(doom->sounds->mcThunder);
	Mix_FreeChunk(doom->sounds->mcSteam);
	Mix_FreeChunk(doom->sounds->mcSword);
	Mix_FreeChunk(doom->sounds->mcPlop);
}

static int	destroy_and_quit(t_doom *doom)
{
	destroy_menu(doom);
	if (!doom->edt_quit)
		destroy_edt(doom);
	if (!doom->game_quit)
		destroy_game(doom);
	destroy_sounds(doom);
	SDL_FreeSurface(doom->buff);
	SDL_DestroyWindow(doom->win);
	Mix_Quit();
	SDL_Quit();
	return (0);
}

static void handle_events(t_doom *doom)
{
	while (SDL_PollEvent(&doom->event) != 0)
	{
		if (doom->event.type == SDL_MOUSEMOTION && !doom->edt_quit && doom->event.window.windowID == SDL_GetWindowID(doom->edt->win))
			edt_mouse_motion(doom);
		else if (doom->event.type == SDL_MOUSEBUTTONDOWN && !doom->edt_quit && doom->event.window.windowID == SDL_GetWindowID(doom->edt->win))
			edt_mouse_down(doom);
		else if (doom->event.type == SDL_MOUSEMOTION && !doom->game_quit && doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
			game_mouse_motion(doom);
		else if (doom->event.type == SDL_MOUSEBUTTONDOWN && !doom->game_quit && doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
			game_mouse_down(doom);
		else if (doom->event.type == SDL_WINDOWEVENT && doom->event.window.event == SDL_WINDOWEVENT_CLOSE && !doom->edt_quit && doom->event.window.windowID == SDL_GetWindowID(doom->edt->win))
		{
			doom->edt_quit = 1;
			destroy_edt(doom);
		}
		else if (doom->event.type == SDL_WINDOWEVENT && doom->event.window.event == SDL_WINDOWEVENT_CLOSE && !doom->game_quit && doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
		{
			doom->game_quit = 1;
			destroy_game(doom);
		}
		else if (doom->event.type == SDL_QUIT || (doom->event.type == SDL_KEYDOWN && doom->event.button.button == SDL_SCANCODE_ESCAPE && doom->edt_quit && !doom->menu->esc_lock))
		{
			Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
			SDL_Delay(300);
			doom->quit = 1;
		}
		else if (doom->event.type == SDL_QUIT || (doom->event.type == SDL_KEYDOWN && doom->event.button.button == SDL_SCANCODE_ESCAPE && !doom->edt_quit))
		{
			doom->edt_quit = 1;
			destroy_edt(doom);
			doom->menu->esc_lock = 40;
			SDL_RestoreWindow(doom->win);
			doom->menu_out_of_focus = 0;
			Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
			doom->buff = SDL_GetWindowSurface(doom->win);
		}
		else if (doom->event.type == SDL_QUIT || (doom->event.type == SDL_KEYDOWN && doom->event.button.button == SDL_SCANCODE_ESCAPE && !doom->game_quit))
		{
			doom->game_quit = 1;
			destroy_game(doom);
			doom->menu->esc_lock = 40;
			SDL_RestoreWindow(doom->win);
			doom->menu_out_of_focus = 0;
			Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
			doom->buff = SDL_GetWindowSurface(doom->win);
		}
	}
}

static void run_loops(t_doom *doom, int argc, char **argv)
{
	main_menu_loop(doom, argc, argv);
	if (!doom->edt_quit)
		edt_render(doom);
	if (!doom->game_quit)
	{
		game_loop(doom);
		game_render(doom);
	}
}

int			main(int argc, char **argv)
{
	t_doom			doom;
	uint32_t		frame_ticks;

	init_doom(&doom);
	init_menu(&doom);
	while (!doom.quit)
	{
		doom.keystates = SDL_GetKeyboardState(NULL);
		doom.frame_start = SDL_GetTicks();
		// Events, including MOUSE and KEYBOARD inputs, Window red crossing ETC! SDL Events!
		handle_events(&doom);
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
