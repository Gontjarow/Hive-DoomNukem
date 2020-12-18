/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:07:38 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/15 20:27:48 by msuarez-         ###   ########.fr       */
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
