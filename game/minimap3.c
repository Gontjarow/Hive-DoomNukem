/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:07:38 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/22 18:57:08 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void		rescale_minimap(t_wall *wall, t_doom *doom)
{
	while (wall->start.x * doom->minimap->scale > MWIN_WIDTH)
		doom->minimap->scale -= 0.1;
	while (wall->start.y * doom->minimap->scale > MWIN_HEIGHT)
		doom->minimap->scale -= 0.1;
	while (wall->end.x * doom->minimap->scale > MWIN_WIDTH)
		doom->minimap->scale -= 0.1;
	while (wall->end.y * doom->minimap->scale > MWIN_HEIGHT)
		doom->minimap->scale -= 0.1;
}

void			print_minimap_walls(t_doom *doom)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = doom->mdl->wall_count;
	wall = doom->mdl->wall_first;
	while (wc--)
	{
		rescale_minimap(wall, doom);
		line.x1 = wall->start.x * doom->minimap->scale;
		line.y1 = wall->start.y * doom->minimap->scale;
		line.x2 = wall->end.x * doom->minimap->scale;
		line.y2 = wall->end.y * doom->minimap->scale;
		line.color = 0xffffffff;
		line.buff = doom->minimap->buff;
		render_line(&line);
		wall = wall->next;
	}
}

static void		pickup_square(t_doom *doom, SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;
	xy.x *= doom->minimap->scale;
	xy.y *= doom->minimap->scale;
	xy.x -= (radius / 2);
	xy.y -= (radius / 2);
	//printf("square_to_buffering point %d, %d\n", xy.x, xy.y);
	y = 0;
	while (y <= radius)
	{
		x = 0;
		while (x <= radius)
		{
			if (y == 0 || y == radius || x == 0 || x == radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
					pixels[address] = color;
				else
					ft_putendl("Warning: square_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
	}
}

static void		pickup_cross(t_doom *doom, SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	t_line 		horz_line;
	t_line 		vert_line;

	horz_line.buff = buff;
	vert_line.buff = buff;
	horz_line.color = color;
	vert_line.color = color;
	horz_line.x1 = (xy.x - radius / 2) * doom->minimap->scale;
	horz_line.y1 = xy.y * doom->minimap->scale;
	horz_line.x2 = (xy.x + radius / 2) * doom->minimap->scale;
	horz_line.y2 = xy.y * doom->minimap->scale;
	vert_line.x1 = xy.x * doom->minimap->scale;
	vert_line.y1 = (xy.y - radius / 2) * doom->minimap->scale;
	vert_line.x2 = xy.x * doom->minimap->scale;
	vert_line.y2 = (xy.y + radius / 2) * doom->minimap->scale;
	render_line(&horz_line);
	render_line(&vert_line);
}

void			print_minimap_pickups(t_doom *doom)
{
	int			pc;
	t_pickup	*pickup;

	pc = doom->mdl->pickup_count;
	if (pc == 0)
		return ;
	pickup = doom->mdl->pickup_first;
	while (pc--)
	{
		pickup_square(doom, doom->minimap->buff, (t_point){pickup->loc.x,
		pickup->loc.y}, PICKING_RADIUS, COLOR_HEALTH_PICKUP);
		pickup_cross(doom, doom->minimap->buff, (t_point){pickup->loc.x,
		pickup->loc.y}, PICKING_RADIUS / 2, COLOR_HEALTH_PICKUP);
		pickup = pickup->next;
	}
}
