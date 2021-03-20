/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alphabet_table.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 19:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/03 19:28:28 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void					*get_alphabet_tables(int type)
{
	static SDL_Scancode	alphabet[TABLE_SIZE] = { SDL_SCANCODE_A, SDL_SCANCODE_B,
		SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F,
		SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_I, SDL_SCANCODE_J,
		SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_M, SDL_SCANCODE_N,
		SDL_SCANCODE_O, SDL_SCANCODE_P, SDL_SCANCODE_Q, SDL_SCANCODE_R,
		SDL_SCANCODE_S, SDL_SCANCODE_T, SDL_SCANCODE_U, SDL_SCANCODE_V,
		SDL_SCANCODE_W, SDL_SCANCODE_X, SDL_SCANCODE_Y, SDL_SCANCODE_Z,
		SDL_SCANCODE_PERIOD, SDL_SCANCODE_BACKSPACE };
	static char			chars[TABLE_SIZE] = { 'a', 'b', 'c', 'd', 'e',
		'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
		'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', 8 };

	if (type == SCANCODES)
		return (&alphabet);
	if (type == CHARS)
		return (&chars);
}

int						table_index(char c)
{
	int					i;
	char				*table;

	table = get_alphabet_tables(CHARS);
	i = TABLE_SIZE;
	while (i--)
	{
		if (table[i] == c)
			return (i);
	}
	ft_die("Fatal error: Could not index for a character in alphabet table");
	return (-1);
}

static int				accept_character(char c, char *buff, int *i)
{
	if (!(c == 8 || c == 0))
	{
		buff[*i] = c;
		(*i)++;
	}
	else if (c == 8 && (*i) > 0)
	{
		buff[*i - 1] = '\0';
		(*i)--;
	}
	return (1);
}

static int				clear_table_hits(int *hits, int *passes)
{
	int					i;

	i = TABLE_SIZE;
	while (i--)
	{
		hits[i] = 0;
		passes[i] = 0;
	}
	return (0);
}

int						propose_character(char c, char *buff, int *i, int index)
{
	int					accepted;
	double				now_time;
	static double		last_time[TABLE_SIZE] = { 0.0 };
	static int			repeated_hit[TABLE_SIZE] = { 0 };
	static int			passed_initial_delay[TABLE_SIZE] = { 0 };

	accepted = 0;
	now_time = SDL_GetTicks();
	if (index == -1)
		return (clear_table_hits(&repeated_hit, &passed_initial_delay));
	if (!repeated_hit[index])
	{
		repeated_hit[index] = 1;
		accepted = accept_character(c, buff, i);
	}
	else if (!passed_initial_delay[index] && now_time - last_time[index] > 500)
	{
		passed_initial_delay[index] = 1;
		accepted = accept_character(c, buff, i);
	}
	else if (passed_initial_delay[index] && now_time - last_time[index] > 30)
		accepted = accept_character(c, buff, i);
	if (accepted)
		last_time[index] = now_time;
	return (accepted);
}
