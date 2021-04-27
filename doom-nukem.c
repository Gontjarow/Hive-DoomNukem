/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom-nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2021/04/27 16:32:13 by msuarez-         ###   ########.fr       */
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
	doom->win = SDL_CreateWindow("Doomed 3D", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	if (doom->win == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window with SDL_CreateWindow on init_doom.");
	doom->buff = SDL_GetWindowSurface(doom->win);
	if (doom->buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Surface with SDL_GetWindowSurface on init_doom.");
	doom->sounds = NULL;
	load_sounds(doom);
	doom->sprites = NULL;
	load_sprites(doom);
	doom->quit = 0;
	doom->chapter_index = 0;
	doom->edt_quit = 1;
	doom->game_quit = 1;
	doom->minimap_quit = 1;
	doom->menu_out_of_focus = 0;
	doom->edt = NULL;
	doom->keystates = NULL;
	doom->minimap = NULL;
	doom->mdl = NULL;
	doom->map = NULL;
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
	destroy_sprites(doom);
	SDL_FreeSurface(doom->buff);
	SDL_DestroyWindow(doom->win);
	Mix_Quit();
	SDL_Quit();
	return (0);
}

static void distribute_inputs(t_doom *doom, int argc, char **argv)
{
	if (!doom->game_quit && doom->game->level_exit_reached)
		window_and_menu_events(doom, argc, argv);
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
		else if ((doom->event.type == SDL_MOUSEBUTTONDOWN || doom->event.type == SDL_MOUSEBUTTONUP)
		&& !doom->game_quit && doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
			game_mouse_updown(doom);
		else
			window_and_menu_events(doom, argc, argv);
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
	int 			fps_cooldown;
	uint32_t		frames_between;
	uint32_t 		time_between;
	float 			acc_time;

	init_doom(&doom);
	set_doom_singleton(&doom);
	init_menu(&doom);
	fps_cooldown = 100;
	frames_between = 0;
	time_between = SDL_GetTicks();
	doom.fps = 30;
	while (!doom.quit)
	{
		doom.frame_start = SDL_GetTicks();
		if (doom.menu->update_argc_argv) // put if argc == 1 if problems , back
		{
			argc = 2;
			argv[1] = doom.menu->added_arg;
			doom.menu->update_argc_argv = 0;
			//printf("\nUPDATED ARGV[1]\nNOW [%s]\n\n", argv[1]);
		}
		doom.keystates = SDL_GetKeyboardState(NULL);
		distribute_inputs(&doom, argc, argv);
		run_loops(&doom, argc, argv);
		if (fps_cooldown)
		{
			frames_between++;
			fps_cooldown--;
		}
		else
		{
			acc_time = ((float)SDL_GetTicks() - (float)time_between) / 1000.0f;
			doom.fps = (float)frames_between / acc_time;
			// ft_putnbr((int)doom.fps);
			// ft_putendl(" fps");
			fps_cooldown = 100;
			time_between = SDL_GetTicks();
			frames_between = 0;
		}
 		SDL_UpdateWindowSurface(doom.win);
		frame_ticks = SDL_GetTicks() - doom.frame_start;
		if (frame_ticks < TICKS_PER_FRAME)
		{
			SDL_Delay(TICKS_PER_FRAME - frame_ticks);
		}
		frame_ticks = SDL_GetTicks() - doom.frame_start;
		doom.delta_time = frame_ticks / 1000.0f;
		doom.delta_anim = 0.07f / doom.delta_time;
		// printf("delta time: %f\n", doom.delta_time);
	}
	return (destroy_and_quit(&doom));
}
