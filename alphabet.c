/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alphabet.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 10:58:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/13 11:07:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static char *extension_path(char *folder, char c, char *extension)
{
	char *path;
	char *join;

	join = ft_strnew(1);
	join[0] = c;
	path = ft_strjoin(folder, join);
	free(join);
	join = path;
	path = ft_strjoin(path, extension);
	free(join);
	return (path);
}

void 		load_alphabet(t_doom *doom)
{
	char c;
	char *path;

	c = 'a';
	while (c <= 'z')
	{
		path = extension_path("img/robo/robo_", c, ".png");
		doom->alphabet[(int)c] = load_texture(doom, path);
		free(path);
		c++;
	}
	c = ' ';
	doom->alphabet[(int)c] = load_texture(doom, "img/robo/robo_space.png");
}

static void render_character(char c, t_doom *doom, int x, int y)
{
	unsigned int 	*reference;
	unsigned int	*pixels;
	int 			i;
	int 			j;
	int 			k;

	if (c >= 'A' && c <= 'Z')
		c += 48;
	i = 0;
	k = 0;
	pixels = doom->buff->pixels;
	if (doom->alphabet[(int)c])
	{
		// K is the raw index for reading all single pixels from reference
		// I is the rolling pixel index offset as column changes (single increments)
		// J is the x, y offset (total offset)
		reference = doom->alphabet[(int)c]->pixels;
		j = x + (WIN_WIDTH * y);
		while (k < (doom->alphabet[(int)c]->w * doom->alphabet[(int)c]->h))
		{
			j = i == doom->alphabet[(int)c]->w ? j + WIN_WIDTH : j;
			i = i == doom->alphabet[(int)c]->w ? 0 : i;
			pixels[i++ + j] = reference[k++];
		}
	}
}

void 	print_alphabet(const char *str, t_doom *doom, int x, int y)
{
	while (*str)
	{
		render_character(*str, doom, x, y);
		x += 28;
		str++;
	}
}