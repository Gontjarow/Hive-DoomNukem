/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/20 22:28:29 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "minimap.h"

static void	print_walls(t_doom *doom)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = doom->mdl->wall_count;
	wall = doom->mdl->wall_first;
	//printf("Walls | wall_count %d\n------------------\n\n", wc);
	while (wc--)
	{
		//printf("Wall id: %d | start: %d, %d | end: %d, %d\n",
		//		wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		while (wall->start.x * doom->minimap->scale > 400)
			doom->minimap->scale -= 0.1;
		while (wall->start.y * doom->minimap->scale > 400)
			doom->minimap->scale -= 0.1;
		while (wall->end.x * doom->minimap->scale > 400)
			doom->minimap->scale -= 0.1;
		while (wall->end.y * doom->minimap->scale > 400)
			doom->minimap->scale -= 0.1;
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

static void	print_player(t_doom *doom)
{
	t_line	line;

	line.x1 = doom->mdl->player.x * doom->minimap->scale;
	line.y1 = doom->mdl->player.y * doom->minimap->scale;
	line.x2 = doom->mdl->player.tail.x * doom->minimap->scale;
	line.y2 = doom->mdl->player.tail.y * doom->minimap->scale;
	line.color = 0xffffff00;
	line.buff = doom->minimap->buff;
	render_line(&line);
}

void		init_minimap(t_doom *doom)
{
	int			win_x;
	int			win_y;

	if (!(doom->minimap = (t_minimap *)malloc(sizeof(t_minimap))))
		return ;
	doom->minimap->win = SDL_CreateWindow("Debug Minimap", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, MINIMAP_WIN_WIDTH, MINIMAP_WIN_HEIGHT, 0);
	if (doom->minimap->win == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window with SDL_CreateWindow on init_minimap.");
	doom->minimap->buff = SDL_GetWindowSurface(doom->minimap->win);
	if (doom->minimap->buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Surface with SDL_GetWindowSurface on init_minimap.");
	doom->minimap->scale = 1.0;
	SDL_GetWindowPosition(doom->win, &win_x, &win_y);
	SDL_SetWindowPosition(doom->minimap->win, win_x + WIN_WIDTH, win_y);
	print_walls(doom);
	print_player(doom);
	SDL_UpdateWindowSurface(doom->minimap->win);
}
