/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:41:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/18 18:23:44 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void 		init_edt(t_doom *doom, int argc, char **argv)
{
	doom->edt = (t_editor*)malloc(sizeof(t_editor));
	if (!doom->edt)
		ft_die("Fatal error: Mallocing level editor struct failed at init_edt.");
	doom->edt->parent = doom;
	doom->edt->win = SDL_CreateWindow("DoomNukem Level Editor", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 0);
	doom->edt->buff = SDL_GetWindowSurface(doom->edt->win);
	doom->edt->walls = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->walls)
		ft_die("Fatal error: Mallocing walls struct failed at init_edt.");
	doom->edt->portals = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->portals)
		ft_die("Fatal error: Mallocing portals struct failed at init_edt.");
	doom->edt->enemies = (t_enemy*)malloc(sizeof(t_enemy));
	if (!doom->edt->enemies)
		ft_die("Fatal error: Mallocing enemies struct failed at init_edt.");
	doom->edt->wall_begin = NULL;
	doom->edt->enemy_first = NULL;
	doom->edt->map_string = NULL;
	doom->edt->join_string = NULL;
	doom->edt->map_path = NULL;
	doom->edt->portalization_a = NULL;
	doom->edt->portalization_b = NULL;
	doom->edt->new_portal = NULL;
	doom->edt->portal_begin = NULL;
	doom->edt->wall_count = 0;
	doom->edt->portal_count = 0;
	doom->edt->enemy_count = 0;
	doom->edt->player_set = 0;
	doom->edt->enemy_set = 0;
	doom->edt->polygon_binding = 0;
	doom->edt->portalization_binding = 0;
	doom->edt->portalization_ending = 0;
	doom->edt->write_maps = 0;
	doom->edt->player.x = -1;
	doom->edt->player.y = -1;
	doom->edt->player.rot = -1;
	doom->edt->is_wall_start = 1;
	doom->edt->polygon_start_x = -1;
	doom->edt->polygon_start_y = -1;
	doom->edt->portal_x = -1;
	doom->edt->portal_y = -1;
	doom->edt->new_portal_x = -1;
	doom->edt->new_portal_y = -1;
	if (argc == 2)
	{
		doom->edt->write_maps = 1;
		doom->edt->map_path = argv[1];
	}
}

void 		destroy_edt(t_doom *doom)
{
	if (doom->edt->write_maps && doom->edt->wall_count > 0)
		write_mapfile(doom->edt);
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	free(doom->edt->walls);
	doom->edt->walls = NULL;
	if (doom->edt->map_string)
		free(doom->edt->map_string);
	if (doom->edt->join_string)
		free(doom->edt->join_string);
	doom->edt->map_string = NULL;
	doom->edt->join_string = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

void 		edt_mouse_motion(t_doom *doom)
{
	if (doom->edt->polygon_start_x > 0 && doom->edt->polygon_start_y > 0)
	{
		if (abs(doom->event.motion.x - doom->edt->polygon_start_x) < 16)
		{
			if (abs(doom->event.motion.y - doom->edt->polygon_start_y) < 16)
				doom->edt->polygon_binding = 1;
		}
		else
			doom->edt->polygon_binding = 0;
	}
}

int			write_mapfile(t_editor *edt)
{
	int opened;
	char new_line[2];

	new_line[0] = '\n';
	new_line[1] = '\0';
	if (!edt->write_maps)
		return (0);
	opened = open(edt->map_path, O_WRONLY | O_CREAT, 0666);
	if (opened > 1)
	{
		write(opened, edt->map_string, ft_strlen(edt->map_string));
		write(opened, new_line, 1);
		if (edt->portal_count > 0)
			write(opened, edt->portal_string, ft_strlen(edt->portal_string));
		close(opened);
		return (1);
	}
	return (0);
}

static void print_characters(t_editor *edt)
{
	t_enemy *enemy;
	t_line	line;
	int 	ec;

	if (edt->player_set == 1)
	{
		line.x1 = edt->player.x;
		line.y1 = edt->player.y;
		line.x2 = edt->tail.x;
		line.y2 = edt->tail.y;
		line.color = 0xffffff00;
		line.buff = edt->buff;
		render_line(&line);
	}
	ec = edt->enemy_count;
	if (ec == 0)
		return ;
	enemy = edt->enemy_first;
	while (ec--)
	{
		line.x1 = enemy->x;
		line.y1 = enemy->y;
		line.x2 = enemy->tail.x;
		line.y2 = enemy->tail.y;
		line.color = 0xff00ff00;
		line.buff = edt->buff;
		render_line(&line);
		enemy = enemy->next;
	}
}

static void print_portals(t_editor *edt)
{
	int		pc;
	t_wall	*portal;
	t_line	line;

	pc = edt->portal_count;
	if (pc == 0)
		return ;
	portal = edt->portal_begin;
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
		line.buff = edt->buff;
		render_line(&line);
		portal = portal->next;
	}
}

static void print_walls(t_editor *edt)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = edt->wall_count;
	wall = edt->wall_begin;
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
		line.buff = edt->buff;
		render_line(&line);
		wall = wall->next;
	}
	print_characters(edt);
	print_portals(edt);
}

static void expand_map_string(t_editor *edt)
{
	if (edt->write_maps)
	{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Id: %d | start: %d, %d | end: %d, %d |\n",
				edt->walls->id, edt->walls->start.x, edt->walls->start.y, edt->walls->end.x, edt->walls->end.y);
		if (!edt->map_string)
			edt->map_string = ft_strnew(1);
		edt->map_string = ft_strjoin(edt->map_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
	}
}

static void set_portalization_xy(t_editor *edt)
{
	if (edt->portalization_binding)
	{
		if (edt->portalization_a->end.x == edt->portalization_b->start.x &&
			edt->portalization_a->end.y == edt->portalization_b->start.y)
		{
			edt->portal_x = edt->portalization_a->end.x;
			edt->portal_y = edt->portalization_a->end.y;
			//ft_putendl("Set Portalization(TM).");
		}
		else
		{
			ft_putendl("Warning: Portalization A / B comparison error.");
			edt->portal_x = -1;
			edt->portal_y = -1;
			edt->portalization_binding = 0;
		}
	}
	else
	{
		edt->portal_x = -1;
		edt->portal_y = -1;
	}
}

static void test_begin_portalization(int x, int y, t_editor *edt)
{
	int 	wc;
	t_wall	*wall;

	wc = edt->wall_count;
	wall = edt->wall_begin;
	edt->portalization_binding = 0;
	while (wc--)
	{
		if ((abs(x - wall->start.x) < 16) && (abs(y - wall->start.y) < 16))
		{
			edt->portalization_binding = 1;
			edt->portalization_b = wall;
		}
		else if ((abs(x - wall->end.x) < 16) && (abs(y - wall->end.y) < 16))
		{
			edt->portalization_binding = 1;
			edt->portalization_a = wall;
		}
		wall = wall->next;
	}
	set_portalization_xy(edt);
}

static void test_end_portalization(int x, int y, t_editor *edt)
{
	edt->new_portal = NULL;
	if ((abs(x - edt->portalization_a->start.x) < 16) && (abs(y - edt->portalization_a->start.y) < 16))
	{
		edt->new_portal = edt->portalization_a;
		edt->new_portal_x = edt->portalization_a->start.x;
		edt->new_portal_y = edt->portalization_a->start.y;
	}
	else if ((abs(x - edt->portalization_b->end.x) < 16) && (abs(y - edt->portalization_b->end.y) < 16))
	{
		edt->new_portal = edt->portalization_b;
		edt->new_portal_x = edt->portalization_b->end.x;
		edt->new_portal_y = edt->portalization_b->end.y;
	}
}

static void record_enemy(int x, int y, t_editor *edt)
{
	t_point		rot_point;
	t_point		enemy_point;
	t_enemy		*next_enemy;

	if (!edt->enemy_set)
	{
		edt->enemies->id = edt->enemy_count;
		edt->enemies->x = x;
		edt->enemies->y = y;
		edt->enemy_set = 1;
		return ;
	}
	rot_point.x = x;
	rot_point.y = y;
	enemy_point.x = edt->enemies->x;
	enemy_point.y = edt->enemies->y;
	modify_line_length(15, &enemy_point, &rot_point, &edt->enemies->tail);
	edt->last_enemy.x = edt->enemies->x;
	edt->last_enemy.y = edt->enemies->y;
	// UNDER CONSTRUCTION!!!
	edt->enemies->rot = 50;
	//expand_enemy_string(edt);
	next_enemy = (t_enemy*)malloc(sizeof(t_enemy));
	if (!next_enemy)
		ft_die("Fatal error: Could not malloc t_enemy at record_enemy.");
	if (edt->enemy_count == 0)
		edt->enemy_first = edt->enemies;
	edt->enemy_count++;
	edt->enemies->next = next_enemy;
	edt->enemies = next_enemy;
	edt->enemy_set = 0;
	print_walls(edt);
}

static void record_player(int x, int y, t_editor *edt)
{
	t_point	start;

	if (!edt->player_set)
	{
		edt->player.x = x;
		edt->player.y = y;
		edt->player_set = -1;
		//ft_putnbr(edt->player.x);
		//ft_putstr(" x | y ");
		//ft_putnbr(edt->player.y);
		//ft_putendl(" | Set player position.");
	}
	else if (edt->player_set == -1)
	{
		edt->tail.x = x;
		edt->tail.y = y;
		start.x = edt->player.x;
		start.y = edt->player.y;
		modify_line_length(15, &start, &edt->tail, &edt->tail);
		// UNDER CONSTRUCTION!!!!
		// edt->player.rot = 50;
		edt->player_set = 1;
		//ft_putnbr(edt->tail.x);
		//ft_putstr(" x | y ");
		//ft_putnbr(edt->tail.y);
		//ft_putendl(" | Set player tail.");
		//update_player_string(edt);
	}
	print_walls(edt);
}

static void expand_portal_string(t_editor *edt)
{
	if (edt->write_maps)
	{
		edt->join_string = ft_strnew(255);
		sprintf(edt->join_string, "Portal id: %d | start: %d, %d | end: %d, %d\n",
				edt->portals->id, edt->portals->start.x, edt->portals->start.y, edt->portals->end.x, edt->portals->end.y);
		if (!edt->portal_string)
			edt->portal_string = ft_strnew(1);
		edt->portal_string = ft_strjoin(edt->portal_string, edt->join_string);
		free(edt->join_string);
		edt->join_string = NULL;
	}
}

static void record_portal(t_editor *edt)
{
	t_wall *next_portal;

	edt->portals->id = edt->portal_count;
	edt->portals->start.x = edt->new_portal->start.x;
	edt->portals->start.y = edt->new_portal->start.y;
	edt->portals->end.x = edt->new_portal->end.x;
	edt->portals->end.y = edt->new_portal->end.y;
	expand_portal_string(edt);
	next_portal = (t_wall*)malloc(sizeof(t_wall));
	if (!next_portal)
		ft_die("Fatal error: Could not malloc t_wall at record_portal.");
	if (edt->portal_count == 0)
		edt->portal_begin = edt->portals;
	edt->portal_count++;
	edt->portals->next = next_portal;
	edt->portals = next_portal;
	print_walls(edt);
}

static void record_polygon(int x, int y, t_editor *edt)
{
	t_wall *next_wall;

	if (edt->is_wall_start)
	{
		//ft_putstr("Testing begin Portalization(TM): ");
		test_begin_portalization(x, y, edt);
		edt->walls->id = edt->wall_count;
		if (edt->portalization_binding)
		{
			edt->walls->start.x = edt->portal_x;
			edt->walls->start.y = edt->portal_y;
		}
		else
		{
			//ft_putendl("Negative.");
			edt->walls->start.x = x;
			edt->walls->start.y = y;
		}
		edt->polygon_start_x = x;
		edt->polygon_start_y = y;
		edt->is_wall_start = 0;
	}
	else if (edt->polygon_binding)
	{
		edt->walls->end.x = edt->polygon_start_x;
		edt->walls->end.y = edt->polygon_start_y;
		expand_map_string(edt);
		edt->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (edt->wall_count == 1)
			edt->wall_begin = edt->walls;
		edt->walls->next = next_wall;
		edt->walls = next_wall;
		edt->is_wall_start = 1;
		print_walls(edt);
	}
	else
	{
		edt->walls->end.x = x;
		edt->walls->end.y = y;
		if (edt->portalization_ending)
		{
			//ft_putstr("Testing end Portalization(TM): ");
			test_end_portalization(x, y, edt);
			if (edt->new_portal)
			{
				edt->walls->end.x = edt->new_portal_x;
				edt->walls->end.y = edt->new_portal_y;
				record_portal(edt);
				//ft_putendl("Portal created.");
				edt->portalization_ending = 0;
				edt->portalization_a = NULL;
				edt->portalization_b = NULL;
				edt->is_wall_start = 1;
			}
			else
			{
				//ft_putendl("Negative.");
			}
		}
		expand_map_string(edt);
		edt->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (edt->wall_count == 1)
			edt->wall_begin = edt->walls;
		edt->walls->next = next_wall;
		edt->walls = next_wall;
		edt->walls->id = edt->wall_count;
		edt->walls->start.x = x;
		edt->walls->start.y = y;
		print_walls(edt);
	}
}

static void edt_left_click(t_doom *doom) {
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	color = 0xffffffff;
	i = doom->event.motion.x;
	j = doom->event.motion.y * EDT_WIN_WIDTH;
	record_polygon(doom->event.motion.x, doom->event.motion.y, doom->edt);
	if (doom->edt->polygon_binding)
	{
		i = doom->edt->polygon_start_x;
		j = doom->edt->polygon_start_y * EDT_WIN_WIDTH;
		color = 0xff00ff00;
	}
	if (doom->edt->portalization_binding)
	{
		i = doom->edt->portal_x;
		j = doom->edt->portal_y * EDT_WIN_WIDTH;
		color = 0xffff0000;
		doom->edt->portalization_binding = 0;
		doom->edt->portalization_ending = 1;
	}
	if (doom->edt->new_portal_x >= 0)
	{
		i = doom->edt->new_portal_x;
		j = doom->edt->new_portal_y * EDT_WIN_WIDTH;
		color = 0xffff0000;
		doom->edt->new_portal_x = -1;
		doom->edt->new_portal_y = -1;
	}
	pixels = doom->edt->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + EDT_WIN_WIDTH] = color;
	pixels[i + j - EDT_WIN_WIDTH] = color;
	Mix_PlayChannel( -1, doom->sounds->mcPlop, 0 );
}

static void circle_player(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[doom->edt->player.x + x + ((doom->edt->player.y + y) * EDT_WIN_WIDTH)] = 0xffffff00;
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

	pixels = doom->edt->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[doom->edt->last_enemy.x + x + ((doom->edt->last_enemy.y + y) * EDT_WIN_WIDTH)] = 0xff00ff00;
			x++;
		}
		y++;
		x = -radius;
	}
}

static void edt_right_click(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	color = 0xff00ff00;
	if (doom->edt->player_set < 1)
		color = 0xffffff00;
	i = doom->event.motion.x;
	j = doom->event.motion.y * EDT_WIN_WIDTH;
	if (doom->edt->player_set < 1)
	{
		record_player(doom->event.motion.x, doom->event.motion.y, doom->edt);
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0);
		if (doom->edt->player_set == 1)
			return (circle_player(doom));
	}
	else
	{
		record_enemy(doom->event.motion.x, doom->event.motion.y, doom->edt);
		Mix_PlayChannel( -1, doom->sounds->mcSteam, 0);
		if (doom->edt->enemy_set == 0)
			return (circle_enemy(doom));
	}
	pixels = doom->edt->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + EDT_WIN_WIDTH] = color;
	pixels[i + j - EDT_WIN_WIDTH] = color;
}

void 		edt_mouse_down(t_doom *doom)
{
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
	{
		edt_right_click(doom);
	}
	else if (doom->event.button.button == SDL_BUTTON_LEFT)
	{
		edt_left_click(doom);
	}
}

void		edt_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->edt->win);
}
