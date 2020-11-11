/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:44:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/25 13:44:18 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void 		init_menu(t_doom *doom)
{
	unsigned int	*pixels;
	unsigned int 	*reference;
	int				i;
	int 			j;
	int 			k;

	if (!(doom->menu = (t_menu*)malloc(sizeof(t_menu))))
		ft_die("Fatal error: Failed mallocing doom->menu at init_menu.");
	doom->menu->alphabet_scale = 1;
	doom->menu->selected = 1;
	doom->menu->esc_lock = 0;
	doom->menu->parent = doom;
	doom->menu->thunder = load_texture(doom, IMG_THUNDER0);
	if (doom->menu->thunder)
	{
		i = 0;
		k = 0;
		pixels = doom->buff->pixels;
		if (doom->menu->thunder->w > WIN_WIDTH || doom->menu->thunder->h > WIN_HEIGHT)
			ft_die("Fatal error: Too large sized texture loaded to display on window at init_menu.");
		reference = doom->menu->thunder->pixels;
		j = (WIN_WIDTH - doom->menu->thunder->w) / 2 + ((WIN_HEIGHT - doom->menu->thunder->h) / 2 * WIN_WIDTH);
		while (k < (doom->menu->thunder->w * doom->menu->thunder->h))
		{
			j = i == doom->menu->thunder->w ? j + WIN_WIDTH : j;
			i = i == doom->menu->thunder->w ? 0 : i;
			pixels[i++ + j] = reference[k++];
		}
	}
	else
		ft_die("Fatal error: Texture loading failure at init_menu.");
	load_alphabet(doom->menu);
	load_animation(doom->menu);
	Mix_PlayChannel( -1, doom->sounds->mcThunder, 0);
}

void 		destroy_menu(t_doom *doom)
{
	int i;

	i = doom->menu->ani_thunder.frames;
	while (i--)
	{
		SDL_FreeSurface(doom->menu->ani_thunder.surfaces[i]);
		doom->menu->ani_thunder.surfaces[i] = NULL;
	}
	free(doom->menu->ani_thunder.surfaces);
	doom->menu->ani_thunder.surfaces = NULL;
	SDL_FreeSurface(doom->menu->thunder);
	destroy_alphabet(doom->menu);
	free(doom->menu);
	doom->menu = NULL;
}

static void	render_menu(t_menu *menu)
{
	int selection_offset;

	selection_offset = 0;
	menu->alphabet_scale = 2;
	print_alphabet("hive doom nukem", menu->parent, 512 - 420, 29);
	menu->alphabet_scale = 1;
	print_alphabet("ngontjar krusthol msuarez", menu->parent, 512 - 364, 29 + 29 + 29);
	menu->alphabet_scale = menu->selected == 0 ? 2 : 1;
	selection_offset = menu->selected ==  0 ? -14 : 0;
	print_alphabet("play level", menu->parent, 512 - (140 * menu->alphabet_scale), 349 + selection_offset);
	menu->alphabet_scale = menu->selected == 1 ? 2 : 1;
	selection_offset = menu->selected ==  1 ? -14 : 0;
	print_alphabet("level editor", menu->parent, 512 - (168 * menu->alphabet_scale), 349 + 29 + 29 + selection_offset);
	menu->alphabet_scale = menu->selected == 2 ? 2 : 1;
	selection_offset = menu->selected ==  2 ? -14 : 0;
	print_alphabet("quit", menu->parent, 512 - (56 * menu->alphabet_scale), 349 + 29 + 29 + 29 + 29 + selection_offset);
}

void		render_animation(t_doom *doom)
{
	unsigned int	*pixels;
	unsigned int 	*reference;
	SDL_Surface     *ani_surface;
	int 			i;
	int 			j;
	int 			k;
	int 			cutoff;

	pixels = doom->buff->pixels;
	ani_surface = doom->menu->ani_thunder.surfaces[doom->menu->ani_thunder.current];
	i = 0;
	k = 0;
	reference = ani_surface->pixels;
	j = (WIN_WIDTH - ani_surface->w) / 2 + ((WIN_HEIGHT - ani_surface->h) / 2 * WIN_WIDTH);
	cutoff = 45 * WIN_WIDTH;
	j += cutoff;
	while (k < ((ani_surface->w * ani_surface->h) - cutoff))
	{
		j = i == ani_surface->w ? j + WIN_WIDTH : j;
		i = i == ani_surface->w ? 0 : i;
		pixels[i++ + j] = reference[k++];
	}
	render_menu(doom->menu);
}

void		load_animation(t_menu *menu)
{
	int i;
	char *path;
	char *join;

	i = 0;
	menu->ani_thunder.frames = 11;
	menu->ani_thunder.current = 0;
	menu->ani_thunder.surfaces = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * menu->ani_thunder.frames);
	if (menu->ani_thunder.surfaces == NULL)
		ft_die("Fatal error: Could not malloc SDL_Surfaces in menu->ani_thunder.");
	while (i < menu->ani_thunder.frames)
	{
		join = ft_itoa(i);
		path = ft_strjoin("img/thunder/", join);
		free(join);
		join = path;
		path = ft_strjoin(path, ".png");
		free(join);
		if (!(menu->ani_thunder.surfaces[i] = (SDL_Surface*)malloc(sizeof(SDL_Surface))))
			ft_die("Fatal error: Could not malloc SDL_Surface in menu->ani_thunder->surfaces.");
		menu->ani_thunder.surfaces[i] = load_texture(menu->parent, path);
		free(path);
		if (menu->ani_thunder.surfaces[i] == NULL)
			ft_die("Fatal error: Could not load_texture for menu->ani_thunder->surfaces.");
		i++;
	}
}

void		window_and_menu_events(t_doom *doom)
{
	if (doom->event.type == SDL_WINDOWEVENT && doom->event.window.event == SDL_WINDOWEVENT_CLOSE && !doom->edt_quit && doom->event.window.windowID == SDL_GetWindowID(doom->edt->win))
	{
		doom->edt_quit = 1;
		destroy_edt(doom);
		destroy_model(doom);
		SDL_RestoreWindow(doom->win);
		doom->buff = SDL_GetWindowSurface(doom->win);
		doom->menu_out_of_focus = 0;
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
	}
	else if (doom->event.type == SDL_WINDOWEVENT && doom->event.window.event == SDL_WINDOWEVENT_CLOSE && !doom->game_quit && doom->event.window.windowID == SDL_GetWindowID(doom->game->win))
	{
		doom->game_quit = 1;
		destroy_game(doom);
		destroy_model(doom);
		SDL_RestoreWindow(doom->win);
		doom->buff = SDL_GetWindowSurface(doom->win);
		doom->menu_out_of_focus = 0;
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
	}
	else if (doom->event.type == SDL_QUIT || (doom->event.type == SDL_KEYDOWN && doom->event.button.button == SDL_SCANCODE_ESCAPE && doom->edt_quit && doom->game_quit && !doom->menu->esc_lock))
	{
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
		SDL_Delay(300);
		doom->quit = 1;
	}
	else if (doom->event.type == SDL_QUIT || (doom->event.type == SDL_KEYDOWN && doom->event.button.button == SDL_SCANCODE_ESCAPE && !doom->edt_quit))
	{
		doom->edt_quit = 1;
		destroy_edt(doom);
		destroy_model(doom);
		doom->menu->esc_lock = 40;
		SDL_RestoreWindow(doom->win);
		doom->buff = SDL_GetWindowSurface(doom->win);
		doom->menu_out_of_focus = 0;
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
	}
	else if (doom->event.type == SDL_QUIT || (doom->event.type == SDL_KEYDOWN && doom->event.button.button == SDL_SCANCODE_ESCAPE && !doom->game_quit))
	{
		if (DEBUG == 1)
		{
			doom->minimap_quit = 1;
			destroy_minimap(doom);
		}
		doom->game_quit = 1;
		destroy_game(doom);
		destroy_model(doom);
		doom->menu->esc_lock = 40;
		SDL_RestoreWindow(doom->win);
		doom->buff = SDL_GetWindowSurface(doom->win);
		doom->menu_out_of_focus = 0;
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
	}
}

void		main_menu_loop(t_doom *doom, int argc, char **argv)
{
	static int key_repeat_lock = 0;

	if (doom->menu_out_of_focus)
		return ;
	if (key_repeat_lock)
		key_repeat_lock--;
	if (doom->keystates[SDL_SCANCODE_UP] && !key_repeat_lock)
	{
		doom->menu->selected--;
		if (doom->menu->selected < 0)
			doom->menu->selected = 0;
		Mix_PlayChannel( -1, doom->sounds->mcSteam, 0 );
		key_repeat_lock = 8;
	}
	else if (doom->keystates[SDL_SCANCODE_DOWN] && !key_repeat_lock)
	{
		doom->menu->selected++;
		if (doom->menu->selected > 2)
			doom->menu->selected = 2;
		Mix_PlayChannel( -1, doom->sounds->mcSteam, 0 );
		key_repeat_lock = 8;
	}
	else if (doom->keystates[SDL_SCANCODE_RETURN] && doom->menu->selected == 0 && doom->game_quit && doom->edt_quit)
	{
		SDL_MinimizeWindow(doom->win);
		doom->buff = SDL_GetWindowSurface(doom->win);
		init_game(doom, argc, argv);
		doom->game_quit = 0;
		doom->menu_out_of_focus = 1;
		load_model(doom);
		if (DEBUG == 1)
			init_minimap(doom);
		SDL_UpdateWindowSurface(doom->game->win);
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
	}
	else if (doom->keystates[SDL_SCANCODE_RETURN] && doom->menu->selected == 1 && doom->edt_quit && doom->game_quit)
	{
		SDL_MinimizeWindow(doom->win);
		doom->buff = SDL_GetWindowSurface(doom->win);
		init_edt(doom);
		doom->edt_quit = 0;
		doom->menu_out_of_focus = 1;
		if (load_model(doom))
			ft_putendl("Editor started.");
		//if (doom->map->was_filled)
		//	ft_putendl("Model loaded from mapfile.");
		SDL_UpdateWindowSurface(doom->edt->win);
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
	}
	else if (doom->keystates[SDL_SCANCODE_RETURN] && doom->menu->selected == 2)
	{
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
		SDL_Delay(300);
		doom->quit = 1;
	}
	if (doom->menu->ani_thunder.current < doom->menu->ani_thunder.frames && doom->game_quit && doom->edt_quit
		&& doom->frame_start - doom->app_start > (unsigned int)doom->menu->ani_thunder.current * 66)
	{
		doom->menu->ani_thunder.current++;
		if (doom->menu->ani_thunder.current == doom->menu->ani_thunder.frames)
		{
			doom->menu->ani_thunder.current = 0;
			doom->app_start = doom->frame_start;
		}
		render_animation(doom);
	}
	if (doom->menu->esc_lock)
		doom->menu->esc_lock--;
}
