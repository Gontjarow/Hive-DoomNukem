/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 17:07:45 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/11 18:28:18 by msuarez-         ###   ########.fr       */
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

void 		load_numbers(t_doom *doom)
{
	char	num;
	char	*path;

	num = '0';
	while (num <= '9')
	{
		path = extension_num_path("img/num/", num, ".png");
		doom->mdl->player.hud_num[(int)num] = load_texture(doom, path);
		free(path);
		num++;
	}
}

void 		destroy_numbers(t_doom *doom)
{
	char num;

	num = '0';
	while (num <= '9')
	{
		SDL_FreeSurface(doom->mdl->player.hud_num[(int)num]);
		num++;
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
	if (doom->mdl->player.hud_num[(int)c])
	{
		reference = doom->mdl->player.hud_num[(int)c]->pixels;
		j = x + (WIN_WIDTH * y);
		while (k < (doom->mdl->player.hud_num[(int)c]->w * doom->mdl->player.hud_num[(int)c]->h))
		{
			j = i == doom->mdl->player.hud_num[(int)c]->w ? j + WIN_WIDTH : j;
			i = i == doom->mdl->player.hud_num[(int)c]->w ? 0 : i;
			pixels[i++ + j] = reference[k++];
		}
	}
}

void	game_print_numbers(const char *str, t_doom *doom, int x, int y)
{
	while (*str)
	{
		render_number(*str, doom, x, y);
		x += 75 * doom->minimap->scale;
		str++;
	}
}
