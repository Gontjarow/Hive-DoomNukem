/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:41:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/14 18:02:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void 		le_mouse_motion(t_doom *doom)
{
	if (doom)
		return ;
}

static void print_walls(t_le *le)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = le->wall_count;
	wall = le->wall_begin;
	printf("Printing walls | wall_count %d\n------------------\n\n", wc);
	while (wc--)
	{
		printf("Wall id: %d | start: %d, %d | end: %d, %d\n",
		 		wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		line.x1 = wall->start.x;
		line.y1 = wall->start.y;
		line.x2 = wall->end.x;
		line.y2 = wall->end.y;
		line.color = 0xffffffff;
		line.buff = le->buff;
		render_line(&line);
		wall = wall->next;
	}
}

static void record_wall(int x, int y, t_le *le)
{
	t_wall *next_wall;

	if (le->is_wall_start)
	{
		le->walls->id = le->wall_count;
		le->walls->start.x = x;
		le->walls->start.y = y;
		le->is_wall_start = 0;
	} else
	{
		le->walls->end.x = x;
		le->walls->end.y = y;
		le->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (le->wall_count == 1)
			le->wall_begin = le->walls;
		le->walls->next = next_wall;
		le->walls = next_wall;
		le->is_wall_start = 1;
		print_walls(le);
	}
}

void 		le_mouse_down(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;

	i = doom->event.motion.x;
	j = doom->event.motion.y * LE_WIN_WIDTH;
	pixels = doom->le->buff->pixels;
	pixels[i + j] = 0xffffffff;
	pixels[i + j - 1] = 0xffffffff;
	pixels[i + j + 1] = 0xffffffff;
	pixels[i + j + LE_WIN_WIDTH] = 0xffffffff;
	pixels[i + j - LE_WIN_WIDTH] = 0xffffffff;
	record_wall(doom->event.motion.x, doom->event.motion.y, doom->le);
	Mix_PlayChannel( -1, doom->mcPlop, 0 );
}

void		le_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->le->win);
}