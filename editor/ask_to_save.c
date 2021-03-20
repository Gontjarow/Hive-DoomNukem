/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ask_to_save.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 19:38:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/03 19:41:28 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void				draw_confirmation(char *input, t_doom *doom)
{
	get_state()->gui->change_zoom(get_state());
	flood_buffer(doom->edt->buff, 0xff000000);
	SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
	editor_back_buffer()->rendering_on = 0;
	edt_gridify();
	print_glyph_str("saved map as ", doom->edt->buff, 480, 860);
	print_glyph_str(input, doom->edt->buff, 922, 860);
	print_glyph_str("saved map as ", editor_back_buffer()->buff, 480, 860);
	print_glyph_str(input, editor_back_buffer()->buff, 922, 860);
}

static void				draw_input(t_doom *doom)
{
	get_state()->gui->change_zoom(get_state());
	flood_buffer(doom->edt->buff, 0xff000000);
	SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
	editor_back_buffer()->rendering_on = 0;
	edt_gridify();
	print_glyph_str(STRING_ENTER_MAPFILE, doom->edt->buff, 80, 50);
	print_glyph_str(STRING_VALID_CHAR_INFO, doom->edt->buff, 90, 90);
}

static void				decide_table(int x, char *arr, int *i, t_doom *doom)
{
	char				*chars;

	chars = (char*)get_alphabet_tables(CHARS);
	if (*i == 255)
	{
		if (doom->keystates[SDL_SCANCODE_BACKSPACE])
		{
			if (propose_character(8, arr, i, table_index(chars[x])))
			{
				draw_input(doom);
				print_glyph_str(arr, doom->edt->buff, 100, 130);
			}
		}
	}
	else if (propose_character(chars[x], arr, i, table_index(chars[x])))
	{
		draw_input(doom);
		print_glyph_str(arr, doom->edt->buff, 100, 130);
	}
}

static void				keystate_input(char *arr, int *i, t_doom *doom)
{
	SDL_Scancode		*alphabet;
	int					detections;
	int					x;

	alphabet = (SDL_Scancode*)get_alphabet_tables(SCANCODES);
	detections = 0;
	x = 0;
	while (x < TABLE_SIZE)
	{
		if (doom->keystates[alphabet[x]])
		{
			decide_table(x, arr, i, doom);
			detections++;
		}
		x++;
	}
	if (!detections)
		propose_character(0, NULL, NULL, -1);
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
		if (doom->keystates[SDL_SCANCODE_RETURN])
			finished = 1;
		keystate_input(&input, &i, doom);
		SDL_UpdateWindowSurface(doom_ptr()->edt->win);
	}
	draw_confirmation(&input, doom);
	return (ft_strdup(&input));
}
