/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alphabet.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 10:58:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/01/19 18:44:28 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

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

void 		destroy_alphabet(t_menu *menu)
{
	char c;

	c = 'a';
	while (c <= 'z')
	{
		SDL_FreeSurface(menu->alphabet[(int)c]);
		c++;
	}
	c = ' ';
	SDL_FreeSurface(menu->alphabet[(int)c]);
	c = '.';
	SDL_FreeSurface(menu->alphabet[(int)c]);
}

void 		load_alphabet(t_menu *menu)
{
	char c;
	char *path;

	c = 'a';
	while (c <= 'z')
	{
		path = extension_path("img/robo/robo_", c, ".png");
		menu->alphabet[(int)c] = load_texture(menu->parent, path);
		free(path);
		c++;
	}
	c = ' ';
	menu->alphabet[(int)c] = load_texture(menu->parent, "img/robo/robo_space.png");
	c = '/';
	menu->alphabet[(int)c] = load_texture(menu->parent, "img/robo/robo_slash.png");
	c = '.';
	menu->alphabet[(int)c] = load_texture(menu->parent, "img/robo/robo_period.png");
}

static void render_character2x(char c, t_doom *doom, int x, int y)
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
	if (doom->menu->alphabet[(int)c])
	{
		reference = doom->menu->alphabet[(int)c]->pixels;
		j = x + (WIN_WIDTH * y);
		while (k < (doom->menu->alphabet[(int)c]->w * doom->menu->alphabet[(int)c]->h))
		{
			j = i >= doom->menu->alphabet[(int)c]->w * 2 ? j + WIN_WIDTH * 2: j;
			i = i >= doom->menu->alphabet[(int)c]->w * 2 ? 0 : i;
			pixels[i + j] = reference[k];
			pixels[i + j + 1] = reference[k];
			j += WIN_WIDTH;
			pixels[i + j] = reference[k];
			pixels[i + j + 1] = reference[k];
			j -= WIN_WIDTH;
			i += 2;
			k++;
		}
	}
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
	if (doom->menu->alphabet[(int)c])
	{
		reference = doom->menu->alphabet[(int)c]->pixels;
		j = x + (WIN_WIDTH * y);
		while (k < (doom->menu->alphabet[(int)c]->w * doom->menu->alphabet[(int)c]->h))
		{
			j = i == doom->menu->alphabet[(int)c]->w ? j + WIN_WIDTH : j;
			i = i == doom->menu->alphabet[(int)c]->w ? 0 : i;
			pixels[i++ + j] = reference[k++];
		}
	}
}

void 	print_alphabet(const char *str, t_doom *doom, int x, int y)
{
	while (*str)
	{
		if(doom->menu->alphabet_scale == 2)
			render_character2x(*str, doom, x, y);
		else
			render_character(*str, doom, x, y);
		x += 28 * doom->menu->alphabet_scale;
		str++;
	}
}

static void render_glyph(char c, SDL_Surface *buff, int x, int y)
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
	pixels = buff->pixels;
	if (doom_ptr()->menu->alphabet[(int)c])
	{
		reference = doom_ptr()->menu->alphabet[(int)c]->pixels;
		j = x + (buff->w * y);
		while (k < (doom_ptr()->menu->alphabet[(int)c]->w * doom_ptr()->menu->alphabet[(int)c]->h))
		{
			j = i == doom_ptr()->menu->alphabet[(int)c]->w ? j + buff->w : j;
			i = i == doom_ptr()->menu->alphabet[(int)c]->w ? 0 : i;
			pixels[i++ + j] = reference[k++];
		}
	}
}

// TODO CONVERT UPPERCASE TO LOWERCASE
void 	print_glyph_str(const char *str, SDL_Surface *buff, int x, int y)
{
	while (*str)
	{
		render_glyph(*str, buff, x, y);
		x += doom_ptr()->menu->alphabet[(int)*str]->w + 2;
		str++;
	}
}
