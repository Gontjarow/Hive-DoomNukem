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
	if (doom->le->polygon_start_x > 0 && doom->le->polygon_start_y > 0)
	{
		if (abs(doom->event.motion.x - doom->le->polygon_start_x) < 16)
		{
			if (abs(doom->event.motion.y - doom->le->polygon_start_y) < 16)
				doom->le->polygon_binding = 1;
		}
		else
			doom->le->polygon_binding = 0;
	}
}


static int	write_mapfile(t_le *le)
{
	int opened;

	if (!le->write_maps)
		return (0);
	opened = open(le->map_path, O_WRONLY | O_CREAT);
	if (opened > 1)
	{
		write(opened, le->map_string, ft_strlen(le->map_string));
		return (1);
	}
	return (0);
}

static void print_walls(t_le *le)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = le->wall_count;
	wall = le->wall_begin;
	printf("Walls | wall_count %d\n------------------\n\n", wc);
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
	write_mapfile(le);
}

static void expand_map_string(t_le *le)
{
	if (le->write_maps)
	{
		le->join_string = ft_strnew(255);
		sprintf(le->join_string, "Wall id: %d | start: %d, %d | end: %d, %d\n",
				le->walls->id, le->walls->start.x, le->walls->start.y, le->walls->end.x, le->walls->end.y);
		if (!le->map_string)
			le->map_string = ft_strnew(1);
		le->map_string = ft_strjoin(le->map_string, le->join_string);
		free(le->join_string);
		le->join_string = NULL;
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
		expand_map_string(le);
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

static void record_polygon(int x, int y, t_le *le)
{
	t_wall *next_wall;

	if (le->is_wall_start)
	{
		le->walls->id = le->wall_count;
		le->walls->start.x = x;
		le->walls->start.y = y;
		le->polygon_start_x = x;
		le->polygon_start_y = y;
		le->is_wall_start = 0;
	}
	else if (le->polygon_binding)
	{
		le->walls->end.x = le->polygon_start_x;
		le->walls->end.y = le->polygon_start_y;
		expand_map_string(le);
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
	else
	{
		le->walls->end.x = x;
		le->walls->end.y = y;
		expand_map_string(le);
		le->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (le->wall_count == 1)
			le->wall_begin = le->walls;
		le->walls->next = next_wall;
		le->walls = next_wall;
		le->walls->id = le->wall_count;
		le->walls->start.x = x;
		le->walls->start.y = y;
		print_walls(le);
	}
}

static void le_left_click(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;

	i = doom->event.motion.x;
	j = doom->event.motion.y * LE_WIN_WIDTH;
	if (doom->le->polygon_binding)
	{
		i = doom->le->polygon_start_x;
		j = doom->le->polygon_start_y * LE_WIN_WIDTH;
	}
	pixels = doom->le->buff->pixels;
	pixels[i + j] = 0xffffffff;
	pixels[i + j - 1] = 0xffffffff;
	pixels[i + j + 1] = 0xffffffff;
	pixels[i + j + LE_WIN_WIDTH] = 0xffffffff;
	pixels[i + j - LE_WIN_WIDTH] = 0xffffffff;
	record_polygon(doom->event.motion.x, doom->event.motion.y, doom->le);
	Mix_PlayChannel( -1, doom->mcPlop, 0 );
}

static void le_right_click(t_doom *doom)
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

void 		le_mouse_down(t_doom *doom)
{
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
	{
		le_right_click(doom);
	}
	else if (doom->event.button.button == SDL_BUTTON_LEFT)
	{
		le_left_click(doom);
	}
}

void		le_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->le->win);
}