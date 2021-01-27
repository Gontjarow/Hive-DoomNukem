/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 20:00:45 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/27 19:27:21 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		init_hud(t_doom *doom)
{
	int n;

	n = 0;
	while (n <= 9)
	{
		if (!(doom->mdl->player.weap_arr[n].weap_img = (SDL_Surface*)malloc
			(sizeof(SDL_Surface))))
			ft_die("Error allocating weap_img!\n");
		n++;
	}
}

void		handle_player_health_bar(t_doom *doom)
{
	float	percentage;
	int		pos;

	percentage = doom->mdl->player.hp.cur / 10;
	pos = (int)percentage;
	doom->mdl->player.active_health_bar = doom->sprites->txt_health_bar[pos];
}

static void draw_surface(int x, int y, SDL_Surface *surf, SDL_Surface *buff)
{
    int *pix[2];
    int i;
    int iter;
    int limit_x;
    int limit_total;

    iter = 0;
    i = x;
    limit_x = x + surf->w;
    limit_total = surf->w * surf->h;
    pix[0] = (int*)buff->pixels;
    pix[1] = (int*)surf->pixels;
    while (iter < limit_total)
    {
        x = i;
        while (x < limit_x)
            pix[0][(y * buff->w) + x++] = pix[1][iter++];
        y++;
    }
}

void        render_hud(t_doom *doom)
{
    draw_surface(512, 450, doom->mdl->player.active_health_bar, doom->game->buff);
}

void		handle_game_hud(t_doom *doom)
{
	handle_player_health_bar(doom);
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
		while (k < (doom->menu->alphabet[(int)c]->w * doom->menu->alphabet
			[(int)c]->h))
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
		x += 50 * doom->minimap->scale;
		str++;
	}
}
