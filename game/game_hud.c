/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 20:00:45 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/01 18:43:56 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		init_hud(t_doom *doom)
{
	int n;

	n = 0;
	while (n <= 9)
	{
		if (!(doom->mdl->player.weap_arr[n].weap_img =
		(SDL_Surface*)malloc(sizeof(SDL_Surface))))
			ft_die("Error allocating weap_img!\n");
		n++;
	}
}

// Surf refers to WHAT you want to draw
// Buff refers to WHERE you want to draw it TO
// X, Y refer to the X, Y location of the BUFF surface
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

void		handle_player_health_bar(t_doom *doom)
{
	float	percentage;
	int		pos;

	percentage = doom->mdl->player.hp.cur / 10;
	pos = (int)percentage;
	doom->mdl->player.active_health_bar = doom->sprites->txt_health_bar[pos];
	draw_surface(0, WIN_HEIGHT - 35, doom->mdl->player.active_health_bar, doom->minimap->buff);
}

void		handle_player_ammo_bar(t_doom *doom)
{
	int		ammo;
	int		i;
	int		space;

	ammo = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur;
	i = 1;
	space = 0;
	while (i <= ammo)
	{
		if (doom->mdl->player.reload_time == 0)
		{
			if (doom->mdl->player.weap_id == 0)
				draw_surface((WIN_WIDTH - 10) + space, WIN_HEIGHT - 40, doom->sprites->txt_pistol_ammo_bar, doom->minimap->buff);
			else if (doom->mdl->player.weap_id == 1)
				draw_surface((WIN_WIDTH - 10) + space, WIN_HEIGHT - 40, doom->sprites->txt_smg_ammo_bar, doom->minimap->buff);
			else if (doom->mdl->player.weap_id == 2)
				draw_surface((WIN_WIDTH - 10) + space, WIN_HEIGHT - 40, doom->sprites->txt_assault_ammo_bar, doom->minimap->buff);
			space -= 10;
		}
		i++;
	}
}

void		handle_clip_bar(t_doom *doom)
{
	int		clips;
	int		i;
	int		space;

	clips = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res;
	space = 0;
	i = 0;
	while (i < clips)
	{
		draw_surface((WIN_WIDTH - 20) + space, WIN_HEIGHT - 80, doom->sprites->txt_clip_bar, doom->minimap->buff);
		space -= 20;
		i++;
	}
}

void		handle_weapon_bar(t_doom *doom)
{
	if (doom->mdl->player.weap_id == 0)
		draw_surface(WIN_WIDTH - 150, 0, doom->mdl->player.weap_arr[0].weap_img, doom->minimap->buff);
	else if (doom->mdl->player.weap_id == 1)
		draw_surface(WIN_WIDTH - 240, 0, doom->mdl->player.weap_arr[1].weap_img, doom->minimap->buff);
	else if (doom->mdl->player.weap_id == 2)
		draw_surface(WIN_WIDTH - 250, 0, doom->mdl->player.weap_arr[2].weap_img, doom->minimap->buff);
}

void		handle_key_bar(t_doom *doom)
{
	/*
		Placeholder to check if player owns a key to display it later on game hud
	*/
	draw_surface(WIN_WIDTH - 50, 300, doom->sprites->txt_key_hud, doom->minimap->buff);
}

void        render_hud(t_doom *doom)
{
    draw_surface(512, 450, doom->mdl->player.active_health_bar, doom->game->buff);
}

void		handle_game_hud(t_doom *doom)
{
	handle_player_health_bar(doom);
	handle_player_ammo_bar(doom);
	handle_clip_bar(doom);
	handle_weapon_bar(doom);
	// handle_key_bar(doom);	//WIP
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
