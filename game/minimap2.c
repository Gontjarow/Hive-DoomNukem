/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:03:18 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/19 19:23:50 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void			print_minimap_weapons(t_doom *doom)
{
	unsigned int	*pixels;
	unsigned int	*ref;
	int				x;
	int				y;
	int				k;
	int				cut;

	pixels = doom->minimap->buff->pixels;
	ref = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].weap_img->pixels;
	cut = 15 * WIN_WIDTH;
	y = (WIN_WIDTH - doom->mdl->player.weap_arr[doom->mdl->player.weap_id].weap_img->w) + ((WIN_HEIGHT - doom->mdl->player.weap_arr[doom->mdl->player.weap_id].weap_img->h) * WIN_WIDTH) - cut;
	y += cut;
	x = 0;
	k = 0;
	while (k < ((doom->mdl->player.weap_arr[doom->mdl->player.weap_id].weap_img->w * doom->mdl->player.weap_arr[doom->mdl->player.weap_id].weap_img->h)))
	{
		y = x == doom->mdl->player.weap_arr[doom->mdl->player.weap_id].weap_img->w ? y + WIN_WIDTH : y;
		x = x == doom->mdl->player.weap_arr[doom->mdl->player.weap_id].weap_img->w ? 0 : x;
		pixels[x++ + y] = ref[k++];
	}
}

static void		mcircle_enemy(t_doom *doom, t_enemy *enemy, unsigned int color)
{
	unsigned int	*pixels;
	int				radius;
	int				x;
	int				y;
	t_point			scaled;

	pixels = doom->minimap->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			scaled.x = enemy->x * doom->minimap->scale;
			scaled.y = enemy->y * doom->minimap->scale;
			if (x * x + y * y > radius * radius - radius && x * x + y * y <
				radius * radius + radius)
				pixels[scaled.x + x + ((scaled.y + y) * MWIN_WIDTH)] = color;
			x++;
		}
		y++;
		x = -radius;
	}
}

void			print_minimap_enemies(t_doom *doom)
{
	t_enemy			*enemy;
	t_line			line;
	int				ec;
	unsigned int	color;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		line.x1 = enemy->x * doom->minimap->scale;
		line.y1 = enemy->y * doom->minimap->scale;
		line.x2 = enemy->tail.x * doom->minimap->scale;
		line.y2 = enemy->tail.y * doom->minimap->scale;
		if (enemy->hp.cur <= 0)
			color = 0xffff0000;
		else
			color = 0xff00ff00;
		line.color = color;
		line.buff = doom->minimap->buff;
		render_line(&line);
		mcircle_enemy(doom, enemy, color);
		enemy = enemy->next;
	}
}

static void		minimap_circle_player(t_doom *doom, unsigned int color)
{
	unsigned int	*pixels;
	int				radius;
	int				x;
	int				y;
	t_point			scaled;

	pixels = doom->minimap->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			scaled.x = (int)doom->mdl->player.x * doom->minimap->scale;
			scaled.y = (int)doom->mdl->player.y * doom->minimap->scale;
			if (x * x + y * y > radius * radius - radius && x * x + y * y <
				radius * radius + radius)
				pixels[scaled.x + x + ((scaled.y + y) * MWIN_WIDTH)] = color;
			x++;
		}
		y++;
		x = -radius;
	}
}

void			print_minimap_player(t_doom *doom)
{
	t_line			line;
	unsigned int	color;

	color = 0xffffff00;
	line.x1 = doom->mdl->player.x * doom->minimap->scale;
	line.y1 = doom->mdl->player.y * doom->minimap->scale;
	line.x2 = doom->mdl->player.tail.x * doom->minimap->scale;
	line.y2 = doom->mdl->player.tail.y * doom->minimap->scale;
	line.color = color;
	line.buff = doom->minimap->buff;
	render_line(&line);
	minimap_circle_player(doom, color);
}
