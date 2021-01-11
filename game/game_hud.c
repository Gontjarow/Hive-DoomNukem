/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 20:00:45 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/11 16:16:25 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static char *extension_num_path(char *folder, char c, char *extension)
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

void 		load_numbers(t_menu *menu)
{
	char	num;
	char	*path;

	num = '0';
	while (num <= '9')
	{
		path = extension_num_path("img/num/", num, ".png");
		menu->hud_num[(int)num] = load_texture(menu->parent, path);
		free(path);
		num++;
	}
}

void 		destroy_numbers(t_menu *menu)
{
	char c;

	c = '0';
	while (c <= '9')
	{
		SDL_FreeSurface(menu->hud_num[(int)c]);
		c++;
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
	pixels = doom->minimap->buff->pixels;				// here comes game buff
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

void	game_print_alphabet(const char *str, t_doom *doom, int x, int y)
{
	while (*str)
	{
		render_character(*str, doom, x, y);
		x += 28 * doom->menu->alphabet_scale;
		str++;
	}
}

static void render_number(char c, t_doom *doom, int x, int y)
{
	unsigned int 	*reference;
	unsigned int	*pixels;
	int 			i;
	int 			j;
	int 			k;

	i = 0;
	k = 0;
	pixels = doom->minimap->buff->pixels;				// here comes game buff
	if (doom->menu->hud_num[(int)c])
	{
		reference = doom->menu->hud_num[(int)c]->pixels;
		j = x + (WIN_WIDTH * y);
		while (k < (doom->menu->hud_num[(int)c]->w * doom->menu->hud_num[(int)c]->h))
		{
			j = i == doom->menu->hud_num[(int)c]->w ? j + WIN_WIDTH : j;
			i = i == doom->menu->hud_num[(int)c]->w ? 0 : i;
			pixels[i++ + j] = reference[k++];
		}
	}
}

void	game_print_numbers(const char *str, t_doom *doom, int x, int y)
{
	while (*str)
	{
		render_number(*str, doom, x, y);
		x += 50 * doom->menu->alphabet_scale;
		str++;
	}
}
