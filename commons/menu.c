/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:44:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/01/11 18:25:19 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	render_menu(t_menu *menu)
{
    int selection_offset;

    selection_offset = 0;
    render_animation(menu->parent);
    menu->alphabet_scale = 2;
    print_alphabet("doomed", menu->parent, 310, 29);
    menu->alphabet_scale = 1;
    print_alphabet("by", menu->parent, 650, 55);
    print_alphabet("ngontjar krusthol msuarez", menu->parent, 512 - 350, 29 + 29 + 29);
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
    doom->menu->mousing_at = -1;
    doom->menu->esc_lock = 0;
    doom->menu->update_argc_argv = 0;
    doom->menu->added_arg = NULL;
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
    render_menu(doom->menu);
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
    if (doom->menu->added_arg)
        free(doom->menu->added_arg);
    free(doom->menu->ani_thunder.surfaces);
    doom->menu->ani_thunder.surfaces = NULL;
    SDL_FreeSurface(doom->menu->thunder);
    destroy_alphabet(doom->menu);
    free(doom->menu);
    doom->menu = NULL;
}

static int ms_since(double milliseconds, double *compared_to)
{
    double	now;

    now = SDL_GetTicks();
    if (now - *compared_to > milliseconds)
    {
        *compared_to = now;
        return (1);
    }
    return (0);
}

static void	start_game_from_menu(t_doom *doom, int argc, char **argv)
{
    static double 	spam_lock = 0;

    if (argc != 2)
    {
        if (ms_since(200, &spam_lock))
        {
            ft_putendl("No level specified as argument!");
            Mix_PlayChannel(-1, doom->sounds->mcSteam, 0);
        }
        return ;
    }
    SDL_MinimizeWindow(doom->win);
    doom->buff = SDL_GetWindowSurface(doom->win);
    init_game(doom, argc, argv);
    doom->game_quit = 0;
    doom->menu_out_of_focus = 1;
    load_model(doom);
    doom->game->world = load_world(get_world());
    // PLAYER X, Y IN MODEL IS -1 AND -1, CAUSING MINIMAP LINE OFF BUFFER?
    //printf("x %f | %f\n", doom->mdl->player.x, doom->mdl->player.y);
    if (DEBUG == 1)
    {
		init_minimap(doom);
		SDL_SetWindowPosition(doom->minimap->win, 0, 0);
		SDL_RaiseWindow(doom->game->win);
	}
    else
	{
		if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0)
			ft_putendl("Warning: Failed to capture mouse to window!");
	}
    SDL_UpdateWindowSurface(doom->game->win);
    Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
    init_player_z(doom);
}

static void	start_editor_from_menu(t_doom *doom, int argc, char **argv)
{
    SDL_MinimizeWindow(doom->win);
    doom->buff = SDL_GetWindowSurface(doom->win);
    init_edt(doom, argc, argv);
    doom->edt_quit = 0;
    doom->menu_out_of_focus = 1;
    if (!load_model(doom))
        ft_die("Fatal error: Could not load model when entering Editor from the main menu.");
    get_state()->saving_choice = 1;
    redraw_editor_to_backbuffer(0xffffffff);
    SDL_UpdateWindowSurface(doom->edt->win);
    Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
}

static void quit_program_from_menu(t_doom *doom)
{
    Mix_PlayChannel( -1, doom->sounds->mcSword, 0 );
    SDL_Delay(300);
    doom->quit = 1;
}

static void select_if_mousing_at(t_doom *doom, int argc, char **argv)
{
    if (doom->menu->mousing_at == -1)
        return ;
    else if (doom->menu->mousing_at == 0)
        start_game_from_menu(doom, argc, argv);
    else if (doom->menu->mousing_at == 1)
        start_editor_from_menu(doom, argc, argv);
    else if (doom->menu->mousing_at == 2)
        quit_program_from_menu(doom);
}

void    render_animation(t_doom *doom)
{
    unsigned int	*pixels;
    unsigned int 	*reference;
    SDL_Surface     *ani_surface;
    int 			i;
    int 			j;
    int 			k;
    int 			cutoff;

    pixels = doom->buff->pixels;
    if (doom->menu->ani_thunder.current == 10)
        doom->menu->ani_thunder.current = 0;
    ani_surface = doom->menu->ani_thunder.surfaces[doom->menu->ani_thunder.current];
    i = 0;
    k = 0;
    reference = ani_surface->pixels;
    j = (WIN_WIDTH - ani_surface->w) / 2 + ((WIN_HEIGHT - ani_surface->h) / 2 * WIN_WIDTH);
    cutoff = 20 * WIN_WIDTH;
    j += cutoff;
    while (k < ((ani_surface->w * ani_surface->h) - cutoff))
    {
        j = i == ani_surface->w ? j + WIN_WIDTH : j;
        i = i == ani_surface->w ? 0 : i;
        pixels[i++ + j] = reference[k++];
    }
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

static void cycle_animation(t_doom *doom)
{
    doom->menu->ani_thunder.current++;
    if (doom->menu->ani_thunder.current == doom->menu->ani_thunder.frames)
        doom->menu->ani_thunder.current = 0;
}

void		window_and_menu_events(t_doom *doom, int argc, char **argv)
{
    static int cycle_repeat_lock = 0;

    if (cycle_repeat_lock)
        cycle_repeat_lock--;
    if (doom->event.type == SDL_MOUSEMOTION && !cycle_repeat_lock)
    {
        cycle_animation(doom);
        cycle_repeat_lock = 16;
    }
    if (doom->event.type == SDL_MOUSEMOTION)
    {
        int x = doom->event.motion.x;
        int y = doom->event.motion.y;
        if (x > 300 && x < 700 && y > 299 && y < 390 && doom->menu->selected != 0)
        {
            doom->menu->selected = 0;
            doom->menu->mousing_at = 0;
            Mix_PlayChannel(-1, doom->sounds->mcSteam, 0);
        }
        else if (x > 300 && x < 700 && y > 389 && y < 440 && doom->menu->selected != 1)
        {
            doom->menu->selected = 1;
            doom->menu->mousing_at = 1;
            Mix_PlayChannel(-1, doom->sounds->mcSteam, 0);
        }
        else if (x > 300 && x < 700 && y > 439 && y < 512 && doom->menu->selected != 2)
        {
            doom->menu->selected = 2;
            doom->menu->mousing_at = 2;
            Mix_PlayChannel(-1, doom->sounds->mcSteam, 0);
        }
        else if (x < 300 || x > 700 || y < 300)
            doom->menu->mousing_at = -1;
    }
    if (doom->event.type == SDL_MOUSEBUTTONDOWN)
        select_if_mousing_at(doom, argc, argv);
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
		if (doom->map->was_filled)
		{
			destroy_mapfile(doom->map);
			doom->map = NULL;
		}
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
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void		main_menu_loop(t_doom *doom, int argc, char **argv)
{
	static int key_repeat_lock = 0;
	static int cycle_repeat_lock = 0;

	if (cycle_repeat_lock)
	    cycle_repeat_lock--;
	if (!cycle_repeat_lock)
    {
	    cycle_animation(doom);
	    cycle_repeat_lock = ((int)doom->fps >> 4);
    }
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
		key_repeat_lock =  (int)doom->fps >> 3;
	}
	else if (doom->keystates[SDL_SCANCODE_DOWN] && !key_repeat_lock)
	{
		doom->menu->selected++;
		if (doom->menu->selected > 2)
			doom->menu->selected = 2;
		Mix_PlayChannel( -1, doom->sounds->mcSteam, 0 );
		key_repeat_lock = (int)doom->fps >> 3;
	}
	else if (doom->keystates[SDL_SCANCODE_RETURN] && doom->menu->selected == 0 && doom->edt_quit && doom->game_quit)
		start_game_from_menu(doom, argc, argv);
	else if (doom->keystates[SDL_SCANCODE_RETURN] && doom->menu->selected == 1 && doom->edt_quit && doom->game_quit)
		start_editor_from_menu(doom, argc, argv);
	else if (doom->keystates[SDL_SCANCODE_RETURN] && doom->menu->selected == 2)
	    quit_program_from_menu(doom);
	if (doom->menu->esc_lock)
		doom->menu->esc_lock--;
    render_menu(doom->menu);
}
