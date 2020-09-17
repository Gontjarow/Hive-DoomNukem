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

int			write_mapfile(t_le *le)
{
	int opened;
	char new_line[2];

	new_line[0] = '\n';
	new_line[1] = '\0';
	if (!le->write_maps)
		return (0);
	opened = open(le->map_path, O_WRONLY | O_CREAT, 0666);
	if (opened > 1)
	{
		write(opened, le->map_string, ft_strlen(le->map_string));
		write(opened, new_line, 1);
		if (le->portal_count > 0)
			write(opened, le->portal_string, ft_strlen(le->portal_string));
		close(opened);
		return (1);
	}
	return (0);
}

static void print_characters(t_le *le)
{
	t_enemy *enemy;
	t_line	line;
	int 	ec;

	if (le->player_set == 1)
	{
		line.x1 = le->player.x;
		line.y1 = le->player.y;
		line.x2 = le->tail.x;
		line.y2 = le->tail.y;
		line.color = 0xffffff00;
		line.buff = le->buff;
		render_line(&line);
	}
	ec = le->enemy_count;
	if (ec == 0)
		return ;
	enemy = le->enemy_first;
	while (ec--)
	{
		line.x1 = enemy->x;
		line.y1 = enemy->y;
		line.x2 = enemy->tail.x;
		line.y2 = enemy->tail.y;
		line.color = 0xff00ff00;
		line.buff = le->buff;
		render_line(&line);
		enemy = enemy->next;
	}
}

static void print_portals(t_le *le)
{
	int		pc;
	t_wall	*portal;
	t_line	line;

	pc = le->portal_count;
	if (pc == 0)
		return ;
	portal = le->portal_begin;
	//printf("Portals | portal_count %d\n------------------\n\n", pc);
	while (pc--)
	{
		//printf("Portal id: %d | start: %d, %d | end: %d, %d\n",
		//	   portal->id, portal->start.x, portal->start.y, portal->end.x, portal->end.y);
		line.x1 = portal->start.x;
		line.y1 = portal->start.y;
		line.x2 = portal->end.x;
		line.y2 = portal->end.y;
		line.color = 0xffff0000;
		line.buff = le->buff;
		render_line(&line);
		portal = portal->next;
	}
}

static void print_walls(t_le *le)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = le->wall_count;
	wall = le->wall_begin;
	//printf("Walls | wall_count %d\n------------------\n\n", wc);
	while (wc--)
	{
		//printf("Wall id: %d | start: %d, %d | end: %d, %d\n",
		//		wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		line.x1 = wall->start.x;
		line.y1 = wall->start.y;
		line.x2 = wall->end.x;
		line.y2 = wall->end.y;
		line.color = 0xffffffff;
		line.buff = le->buff;
		render_line(&line);
		wall = wall->next;
	}
	print_characters(le);
	print_portals(le);
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

static void set_portalization_xy(t_le *le)
{
	if (le->portalization_binding)
	{
		if (le->portalization_a->end.x == le->portalization_b->start.x &&
			le->portalization_a->end.y == le->portalization_b->start.y)
		{
			le->portal_x = le->portalization_a->end.x;
			le->portal_y = le->portalization_a->end.y;
			//ft_putendl("Set Portalization(TM).");
		}
		else
		{
			ft_putendl("Warning: Portalization A / B comparison error.");
			le->portal_x = -1;
			le->portal_y = -1;
			le->portalization_binding = 0;
		}
	}
	else
	{
		le->portal_x = -1;
		le->portal_y = -1;
	}
}

static void test_begin_portalization(int x, int y, t_le *le)
{
	int 	wc;
	t_wall	*wall;

	wc = le->wall_count;
	wall = le->wall_begin;
	le->portalization_binding = 0;
	while (wc--)
	{
		if ((abs(x - wall->start.x) < 16) && (abs(y - wall->start.y) < 16))
		{
			le->portalization_binding = 1;
			le->portalization_b = wall;
		}
		else if ((abs(x - wall->end.x) < 16) && (abs(y - wall->end.y) < 16))
		{
			le->portalization_binding = 1;
			le->portalization_a = wall;
		}
		wall = wall->next;
	}
	set_portalization_xy(le);
}

static void test_end_portalization(int x, int y, t_le *le)
{
	le->new_portal = NULL;
	if ((abs(x - le->portalization_a->start.x) < 16) && (abs(y - le->portalization_a->start.y) < 16))
	{
		le->new_portal = le->portalization_a;
		le->new_portal_x = le->portalization_a->start.x;
		le->new_portal_y = le->portalization_a->start.y;
	}
	else if ((abs(x - le->portalization_b->end.x) < 16) && (abs(y - le->portalization_b->end.y) < 16))
	{
		le->new_portal = le->portalization_b;
		le->new_portal_x = le->portalization_b->end.x;
		le->new_portal_y = le->portalization_b->end.y;
	}
}

static void record_enemy(int x, int y, t_le *le)
{
	t_point		rot_point;
	t_point		enemy_point;
	t_enemy		*next_enemy;

	if (!le->enemy_set)
	{
		le->enemies->id = le->enemy_count;
		le->enemies->x = x;
		le->enemies->y = y;
		le->enemy_set = 1;
		return ;
	}
	rot_point.x = x;
	rot_point.y = y;
	enemy_point.x = le->enemies->x;
	enemy_point.y = le->enemies->y;
	modify_line_length(15, &enemy_point, &rot_point, &le->enemies->tail);
	le->last_enemy.x = le->enemies->x;
	le->last_enemy.y = le->enemies->y;
	// UNDER CONSTRUCTION!!!
	le->enemies->rot = 50;
	//expand_enemy_string(le);
	next_enemy = (t_enemy*)malloc(sizeof(t_enemy));
	if (!next_enemy)
		ft_die("Fatal error: Could not malloc t_enemy at record_enemy.");
	if (le->enemy_count == 0)
		le->enemy_first = le->enemies;
	le->enemy_count++;
	le->enemies->next = next_enemy;
	le->enemies = next_enemy;
	le->enemy_set = 0;
	print_walls(le);
}

static void record_player(int x, int y, t_le *le)
{
	t_point	start;

	if (!le->player_set)
	{
		le->player.x = x;
		le->player.y = y;
		le->player_set = -1;
		//ft_putnbr(le->player.x);
		//ft_putstr(" x | y ");
		//ft_putnbr(le->player.y);
		//ft_putendl(" | Set player position.");
	}
	else if (le->player_set == -1)
	{
		le->tail.x = x;
		le->tail.y = y;
		start.x = le->player.x;
		start.y = le->player.y;
		modify_line_length(15, &start, &le->tail, &le->tail);
		// UNDER CONSTRUCTION!!!!
		// le->player.rot = 50;
		le->player_set = 1;
		//ft_putnbr(le->tail.x);
		//ft_putstr(" x | y ");
		//ft_putnbr(le->tail.y);
		//ft_putendl(" | Set player tail.");
		//update_player_string(le);
	}
	print_walls(le);
}

static void expand_portal_string(t_le *le)
{
	if (le->write_maps)
	{
		le->join_string = ft_strnew(255);
		sprintf(le->join_string, "Portal id: %d | start: %d, %d | end: %d, %d\n",
				le->portals->id, le->portals->start.x, le->portals->start.y, le->portals->end.x, le->portals->end.y);
		if (!le->portal_string)
			le->portal_string = ft_strnew(1);
		le->portal_string = ft_strjoin(le->portal_string, le->join_string);
		free(le->join_string);
		le->join_string = NULL;
	}
}

static void record_portal(t_le *le)
{
	t_wall *next_portal;

	le->portals->id = le->portal_count;
	le->portals->start.x = le->new_portal->start.x;
	le->portals->start.y = le->new_portal->start.y;
	le->portals->end.x = le->new_portal->end.x;
	le->portals->end.y = le->new_portal->end.y;
	expand_portal_string(le);
	next_portal = (t_wall*)malloc(sizeof(t_wall));
	if (!next_portal)
		ft_die("Fatal error: Could not malloc t_wall at record_portal.");
	if (le->portal_count == 0)
		le->portal_begin = le->portals;
	le->portal_count++;
	le->portals->next = next_portal;
	le->portals = next_portal;
	print_walls(le);
}

static void record_polygon(int x, int y, t_le *le)
{
	t_wall *next_wall;

	if (le->is_wall_start)
	{
		//ft_putstr("Testing begin Portalization(TM): ");
		test_begin_portalization(x, y, le);
		le->walls->id = le->wall_count;
		if (le->portalization_binding)
		{
			le->walls->start.x = le->portal_x;
			le->walls->start.y = le->portal_y;
		}
		else
		{
			//ft_putendl("Negative.");
			le->walls->start.x = x;
			le->walls->start.y = y;
		}
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
		if (le->portalization_ending)
		{
			//ft_putstr("Testing end Portalization(TM): ");
			test_end_portalization(x, y, le);
			if (le->new_portal)
			{
				le->walls->end.x = le->new_portal_x;
				le->walls->end.y = le->new_portal_y;
				record_portal(le);
				//ft_putendl("Portal created.");
				le->portalization_ending = 0;
				le->portalization_a = NULL;
				le->portalization_b = NULL;
				le->is_wall_start = 1;
			}
			else
			{
				//ft_putendl("Negative.");
			}
		}
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

static void le_left_click(t_doom *doom) {
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	color = 0xffffffff;
	i = doom->event.motion.x;
	j = doom->event.motion.y * LE_WIN_WIDTH;
	record_polygon(doom->event.motion.x, doom->event.motion.y, doom->le);
	if (doom->le->polygon_binding)
	{
		i = doom->le->polygon_start_x;
		j = doom->le->polygon_start_y * LE_WIN_WIDTH;
		color = 0xff00ff00;
	}
	if (doom->le->portalization_binding)
	{
		i = doom->le->portal_x;
		j = doom->le->portal_y * LE_WIN_WIDTH;
		color = 0xffff0000;
		doom->le->portalization_binding = 0;
		doom->le->portalization_ending = 1;
	}
	if (doom->le->new_portal_x >= 0)
	{
		i = doom->le->new_portal_x;
		j = doom->le->new_portal_y * LE_WIN_WIDTH;
		color = 0xffff0000;
		doom->le->new_portal_x = -1;
		doom->le->new_portal_y = -1;
	}
	pixels = doom->le->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + LE_WIN_WIDTH] = color;
	pixels[i + j - LE_WIN_WIDTH] = color;
	Mix_PlayChannel( -1, doom->mcPlop, 0 );
}

static void circle_player(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->le->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[doom->le->player.x + x + ((doom->le->player.y + y) * LE_WIN_WIDTH)] = 0xffffff00;
			x++;
		}
		y++;
		x = -radius;
	}
}

static void circle_enemy(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->le->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[doom->le->last_enemy.x + x + ((doom->le->last_enemy.y + y) * LE_WIN_WIDTH)] = 0xff00ff00;
			x++;
		}
		y++;
		x = -radius;
	}
}

static void le_right_click(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	color = 0xff00ff00;
	if (doom->le->player_set < 1)
		color = 0xffffff00;
	i = doom->event.motion.x;
	j = doom->event.motion.y * LE_WIN_WIDTH;
	if (doom->le->player_set < 1)
	{
		record_player(doom->event.motion.x, doom->event.motion.y, doom->le);
		Mix_PlayChannel( -1, doom->mcSword, 0);
		if (doom->le->player_set == 1)
			return (circle_player(doom));
	}
	else
	{
		record_enemy(doom->event.motion.x, doom->event.motion.y, doom->le);
		Mix_PlayChannel( -1, doom->mcSteam, 0);
		if (doom->le->enemy_set == 0)
			return (circle_enemy(doom));
	}
	pixels = doom->le->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + LE_WIN_WIDTH] = color;
	pixels[i + j - LE_WIN_WIDTH] = color;
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