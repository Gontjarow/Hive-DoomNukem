/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 20:00:45 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/29 16:32:59 by msuarez-         ###   ########.fr       */
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

void		handle_player_ammo_bar(t_doom *doom)
{
	int		ammo;
	int		i;
	int		space;
	int		newline;

	ammo = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur;
	i = 0;
	space = 10;
	newline = 0;
	while (i < ammo)
	{
		if (doom->mdl->player.weap_id == 0)
			print_minimap_single_sprite(doom, doom->sprites->txt_pistol_ammo_bar);
		else if (doom->mdl->player.weap_id == 1)
			print_minimap_single_sprite(doom, doom->sprites->txt_smg_ammo_bar);
		else if (doom->mdl->player.weap_id == 2)
			print_minimap_single_sprite(doom, doom->sprites->txt_assault_ammo_bar);
		// if (i % 5 == 0)
		// {
		// 	newline += 10;
		// 	space = 10;
		// }
		// else
			space += 10;
		i++;
	}
}

void		handle_game_hud(t_doom *doom)
{
	handle_player_health_bar(doom);
	// handle_player_ammo_bar(doom);
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
