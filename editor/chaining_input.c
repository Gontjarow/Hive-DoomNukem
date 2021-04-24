/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chaining_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 19:44:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/03/04 19:45:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void 	draw_chain_confirmation(char *input, t_doom *doom)
{
	get_state()->gui->change_zoom(get_state());
	flood_buffer(doom->edt->buff, 0xff000000);
	SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
	editor_back_buffer()->rendering_on = 0;
	edt_gridify();
	print_glyph_str("set to chain to ", doom->edt->buff, 480, 860);
	print_glyph_str(input, doom->edt->buff, 922, 860);
	print_glyph_str("set to chain to ", editor_back_buffer()->buff, 480, 860);
	print_glyph_str(input,editor_back_buffer()->buff, 922, 860);
}

void 			edt_chaining_input_loop(t_doom *doom)
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
		if (doom->keystates[SDL_SCANCODE_RETURN])
			finished = 1;
		keystate_input(&input, &i, doom);
		SDL_UpdateWindowSurface(doom_ptr()->edt->win);
	}
	if (doom->mdl->chain != NULL)
		free(doom->mdl->chain);
	doom->mdl->chain = ft_strdup(&input);
	draw_chain_confirmation(&input, doom);
	get_state()->saving_choice = 0;
}
