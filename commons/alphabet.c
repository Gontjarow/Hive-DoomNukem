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

static SDL_Surface	*alphabet_from_atlas(char c, SDL_Surface *atlas)
{
	SDL_Surface		*alphabet_surface;
	char 			a;
	int 			char_x;
	int 			char_y;
	int 			char_order;

	a = 'a';
	char_order = ALPHABET_A_INDEX + (c - a);
	char_x = char_order % ALPHABET_GRID_W;
	char_y = char_order / ALPHABET_GRID_W;
	//printf("Char [%c] Index [%d] | x, y (%d, %d)\n", c, char_order, char_x, char_y);
	char_x *= ALPHABET_W;
	char_y *= ALPHABET_H;
	alphabet_surface = SDL_CreateRGBSurfaceWithFormat(0, ALPHABET_W, ALPHABET_H, 32, SDL_PIXELFORMAT_ARGB32);
	//printf("Copying from %d, %d out of atlas to atlas_surface!\n", char_x, char_y);
	copy_from_atlas(char_x, char_y, atlas, alphabet_surface);
	return (alphabet_surface);
}

static void			set_alphabet_null(t_menu *menu)
{
	int		i;

	i = 0;
	while (i < ALPHABET_SIZE)
		menu->alphabet[i++] = NULL;
}

static SDL_Surface	*create_alphabet(int grid_x, int grid_y, SDL_Surface *atlas)
{
	SDL_Surface		*ret;
	int				x;
	int				y;

	ret = SDL_CreateRGBSurfaceWithFormat(0, ALPHABET_W, ALPHABET_H, 32, SDL_PIXELFORMAT_ARGB32);
	x = grid_x * ALPHABET_W;
	y = grid_y * ALPHABET_H;
	if (!ret)
		ft_die("Fatal error: Could not allocate memory for a character in create_alphabet.");
	copy_from_atlas(x, y, atlas, ret);
	return (ret);
}

void 				load_alphabet_atlas(t_menu *menu)
{
	SDL_Surface		*atlas;
	char			c;

	set_alphabet_null(menu);
	atlas = load_xpm("img/robo/robo_atlas.xpm");
	if (!atlas)
		ft_die("Fatal error: Could not load font atlas for Robocop font.");
	c = 'a';
	while (c <= 'z')
	{
		menu->alphabet[(int)c] = alphabet_from_atlas(c, atlas);
		c++;
	}
	menu->alphabet[(int)' '] = create_alphabet(0,0, atlas);
	menu->alphabet[(int)'.'] = create_alphabet(4,1, atlas);
	menu->alphabet[(int)'/'] = create_alphabet(5,1, atlas);
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

void 	print_glyph_str(const char *str, SDL_Surface *buff, int x, int y)
{
	while (*str)
	{
		render_glyph(*str, buff, x, y);
		x += doom_ptr()->menu->alphabet[(int)*str]->w - 2;
		str++;
	}
}
