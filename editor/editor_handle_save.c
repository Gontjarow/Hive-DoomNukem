/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_handle_save.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 19:44:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/03/04 19:45:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void 	draw_save(t_doom *doom)
{
	get_state()->gui->change_zoom(get_state());
	flood_buffer(doom->edt->buff, 0xff000000);
	SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
	editor_back_buffer()->rendering_on = 0;
	edt_gridify();
	print_glyph_str(STRING_CONFIRM_SAVING, doom->edt->buff, 80, 50);
	if (get_state()->saving_choice == 0)
		print_glyph_str("yes or no y..n", doom->edt->buff, 90, 90);
}

int 			edt_handle_confirm_save(t_doom *doom)
{
	int 			finished;

	get_state()->gui->deactivate(get_state());
	get_state()->job_abort = 1;
	if (get_state()->saving_choice == 1)
		return (1);
	finished = 0;
	while (!finished)
	{
		draw_save(doom);
		SDL_UpdateWindowSurface(doom_ptr()->edt->win);
		SDL_PumpEvents();
		doom->keystates = SDL_GetKeyboardState(NULL);
		if (doom->keystates[SDL_SCANCODE_Y] || doom->keystates[SDL_SCANCODE_N])
			finished = 1;
		get_state()->saving_choice = doom->keystates[SDL_SCANCODE_Y] - doom->keystates[SDL_SCANCODE_N];
	}
	return (get_state()->saving_choice);
}

static void				draw_discarding(t_doom *doom)
{
	get_state()->gui->change_zoom(get_state());
	flood_buffer(doom->edt->buff, 0xff000000);
	SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
	editor_back_buffer()->rendering_on = 0;
	edt_gridify();
	print_glyph_str("discarded map", doom->edt->buff, 480, 860);
	print_glyph_str("returning to menu", doom->edt->buff, 922, 860);
	SDL_UpdateWindowSurface(doom_ptr()->edt->win);
}

char					*ask_to_save(t_doom *doom)
{
	int					finished;
	int					i;
	static int			last_i = 0;
	char				input[255];

	draw_input(doom);
	finished = 0;
	while (finished < 255)
		input[finished++] = '\0';
	finished = 0;
	i = 0;
	while (!finished)
	{
		SDL_PumpEvents();
		doom->keystates = SDL_GetKeyboardState(NULL);
		if (doom->esc_lock && !doom->keystates[SDL_SCANCODE_ESCAPE])
			doom->esc_lock--;
		if (doom->keystates[SDL_SCANCODE_ESCAPE] && !doom->esc_lock)
		{	
			draw_discarding(doom);
			return (NULL);
		}
		if (doom->keystates[SDL_SCANCODE_RETURN])
			finished = 1;
		keystate_input(&input, &i, doom);
		SDL_UpdateWindowSurface(doom_ptr()->edt->win);
	}
	draw_confirmation(&input, doom);
	return (ft_strdup(&input));
}