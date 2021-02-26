/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom-nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/22 18:38:20 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void distribute_inputs(t_doom *doom, int argc, char **argv)
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
		else if ((doom->event.type == SDL_MOUSEBUTTONDOWN || doom->event.type == SDL_MOUSEBUTTONUP)
		&& !doom->game_quit && doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
			game_mouse_updown(doom);
		else
			window_and_menu_events(doom, argc, argv);
	}
}

static void distribute_loops(t_doom *doom, int argc, char **argv)
{
    menu_render(doom, argc, argv);
	if (!doom->edt_quit)
		edt_render(doom);
	if (!doom->game_quit)
		game_render(doom);
}

static void loop_game(t_doom *doom, int argc, char *argv[])
{
	uint32_t		frame_ticks;
	int 			fps_cooldown;
	uint32_t		frames_between;
	uint32_t 		time_between;
	float 			acc_time;

	fps_cooldown = 100;
	frames_between = 0;
	time_between = SDL_GetTicks();
	while (!doom->quit)
	{
		doom->frame_start = SDL_GetTicks();
		if (argc == 1 && doom->menu->update_argc_argv)
		{
			argc = 2;
			argv[1] = doom->menu->added_arg;
			doom->menu->update_argc_argv = 0;
		}
		doom->keystates = SDL_GetKeyboardState(NULL);
		distribute_inputs(doom, argc, argv);
		distribute_loops(doom, argc, argv);
		if (fps_cooldown)
		{
			frames_between++;
			fps_cooldown--;
		}
		else
		{
			acc_time = ((float)SDL_GetTicks() - (float)time_between) / 1000.0f;
			doom->fps = (float)frames_between / acc_time;
			fps_cooldown = 100;
			time_between = SDL_GetTicks();
			frames_between = 0;
		}
		SDL_UpdateWindowSurface(doom->win);
		frame_ticks = SDL_GetTicks() - doom->frame_start;
		if (frame_ticks < TICKS_PER_FRAME)
		{
			SDL_Delay(TICKS_PER_FRAME - frame_ticks);
		}
	}
}

int			main(int argc, char *argv[])
{
	t_doom			doom;

	init_doom(&doom, argv[0]);
	init_menu(&doom);
	loop_game(&doom, argc, argv);
	return (destroy_doom_and_quit(&doom));
}



