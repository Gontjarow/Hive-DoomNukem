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

void 		init_edt(t_doom *doom, int argc, char **argv)
{
	int opened;

	doom->edt = (t_editor*)malloc(sizeof(t_editor));
	if (!doom->edt)
		ft_die("Fatal error: Mallocing level editor struct failed at init_edt.");
	doom->edt->parent = doom;
	doom->edt->win = SDL_CreateWindow("DoomNukem Level Editor", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 0);
	doom->edt->buff = SDL_GetWindowSurface(doom->edt->win);
	flood_window(doom->edt->buff, 0xff000000);
	doom->edt->walls = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->walls)
		ft_die("Fatal error: Mallocing walls struct failed at init_edt.");
	doom->edt->rooms = (t_room*)malloc(sizeof(t_room));
	if (!doom->edt->rooms)
		ft_die("Fatal error: Mallocing rooms struct failed at init_edt.");
	doom->edt->portals = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->portals)
		ft_die("Fatal error: Mallocing portals struct failed at init_edt.");
	doom->edt->enemies = (t_enemy*)malloc(sizeof(t_enemy));
	if (!doom->edt->enemies)
		ft_die("Fatal error: Mallocing enemies struct failed at init_edt.");
	doom->edt->wall_begin = NULL;
	doom->edt->portal_begin = NULL;
	doom->edt->enemy_first = NULL;
	doom->edt->room_first = NULL;
	doom->edt->wall_string = NULL;
	doom->edt->room_string = NULL;
	doom->edt->portal_string = NULL;
	doom->edt->join_string = NULL;
	doom->edt->player_string = NULL;
	doom->edt->enemy_string = NULL;
	doom->edt->map_path = NULL;
	doom->edt->portalization_a = NULL;
	doom->edt->portalization_b = NULL;
	doom->edt->new_portal = NULL;
	doom->edt->wall_count = 0;
	doom->edt->room_count = 0;
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
	doom->edt->load_map = 0;
	doom->edt->write_maps = 0;
	doom->edt->overwrite_map = 0;
	if (argc == 2)
	{
		opened = open(argv[1], O_RDONLY);
		if (opened > 1)
		{
			ft_putendl("Hive-DoomNukem: Loading map enabled. Map file detected.");
			doom->edt->load_map = 1;
			doom->edt->overwrite_map = 1;
			close(opened);
		}
		else
		{
			ft_putendl("Hive-DoomNukem: Saving maps enabled. Map file will be written, when closing Level Editor.");
			doom->edt->write_maps = 1;
		}
		doom->edt->map_path = argv[1];
	}
}

void 		destroy_edt(t_doom *doom)
{
	// INCOMPLETE!!! MUST DESTRYOY LINKED LIST STRUCTURES FROM BEGINNING TO END
	// - krusthol
	if (doom->edt->write_maps && doom->edt->wall_count > 0)
		write_mapfile(doom->edt);
	if (doom->edt->overwrite_map)
		overwrite_mapfile(doom->edt);
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	//ft_putendl("Window SDL destructed");
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	if (!doom->mdl)
	{
		free(doom->edt->walls);
		doom->edt->walls = NULL;
		doom->edt->wall_begin = NULL;
		free(doom->edt->rooms);
		doom->edt->rooms = NULL;
		doom->edt->room_first = NULL;
		free(doom->edt->portals);
		doom->edt->portals = NULL;
		doom->edt->portal_begin = NULL;
		free(doom->edt->enemies);
		doom->edt->enemies = NULL;
		doom->edt->enemy_first = NULL;
	}
	if (doom->edt->wall_string)
		free(doom->edt->wall_string);
	if (doom->edt->room_string)
		free(doom->edt->room_string);
	if (doom->edt->portal_string)
		free(doom->edt->portal_string);
	if (doom->edt->enemy_string)
		free(doom->edt->enemy_string);
	if (doom->edt->player_string)
		free(doom->edt->player_string);
	if (doom->edt->join_string)
		free(doom->edt->join_string);
	doom->edt->wall_string = NULL;
	doom->edt->room_string = NULL;
	doom->edt->portal_string = NULL;
	doom->edt->enemy_string = NULL;
	doom->edt->player_string = NULL;
	doom->edt->join_string = NULL;
	doom->edt->portalization_a = NULL;
	doom->edt->portalization_b = NULL;
	doom->edt->new_portal = NULL;
	doom->edt->map_path = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

void 		edt_mouse_motion(t_doom *doom)
{
	doom->edt->polygon_binding = 0;
	if (doom->edt->polygon_start_x > 0 && doom->edt->polygon_start_y > 0)
	{
		if (abs(doom->event.motion.x - doom->edt->polygon_start_x) < 16)
		{
			if (abs(doom->event.motion.y - doom->edt->polygon_start_y) < 16)
				doom->edt->polygon_binding = 1;
		}
	}
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
		line.x2 = edt->player.tail.x;
		line.y2 = edt->player.tail.y;
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
			//ft_putendl("Portalization_binding due to start.x, start.y.");
		}
		else if ((abs(x - wall->end.x) < 16) && (abs(y - wall->end.y) < 16))
		{
			edt->portalization_binding = 1;
			edt->portalization_a = wall;
			//ft_putendl("Portalization_binding due to end.x, end.y.");
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
	//ft_putendl("Tested end_portalization");
}

static int 	degree_rot(int x, int y, t_point *tail)
{
	double result;

	x = tail->x - x;
	y = tail->y - y;
	result = atan2(y, x) * 180.0 / M_PI;;
	result += 180.0;
	return ((int)result);
}

static void circle_visual(t_doom *doom, t_point *visual)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 3;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[visual->x + x + ((visual->y + y) * EDT_WIN_WIDTH)] = 0xffff00ff;
			x++;
		}
		y++;
		x = -radius;
	}
}

static void circle_room(t_doom *doom, t_room *room)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 3;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[room->visual.x + x + ((room->visual.y + y) * EDT_WIN_WIDTH)] = 0xffffffff;
			x++;
		}
		y++;
		x = -radius;
	}
}

static int	wall_count_of_previous_rooms(t_editor *edt)
{
	int 	rc;
	int		wallcountofpreviousrooms;
	t_room	*rooms;

	rc = edt->room_count;
	rooms = edt->room_first;
	wallcountofpreviousrooms = 0;
	while (rc--)
	{
		wallcountofpreviousrooms += rooms->wall_count;
		//printf("WCOPR added %d\n", rooms->wall_count);
		rooms = rooms->next;
	}
	//printf("WCOPR: %d\n", wallcountofpreviousrooms);
	return (wallcountofpreviousrooms);
}

static t_wall *wall_by_count(t_editor *edt, int count)
{
	t_wall *wall;

	wall = edt->wall_begin;
	while (count--)
	{
		wall = wall->next;
	}
	return (wall);
}

static t_tri_sides room_first_triangle(t_room *room)
{
	t_point		*s;
	t_point		*e;
	double 		distance_a;
	double		distance_b;
	double		distance_c;

	s = &room->first_wall->start;
	e = &room->first_wall->end;
	//printf("Calculating between points X1Y1[%d, %d] and X2Y2[%d, %d]\n", s->x, s->y, e->x, e->y);
	distance_a = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_a = sqrt(distance_a);
	s = &room->first_wall->end;
	e = &room->first_wall->next->end;
	//printf("Calculating between points X1Y1[%d, %d] and X2Y2[%d, %d]\n", s->x, s->y, e->x, e->y);
	distance_b = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_b = sqrt(distance_b);
	s = &room->first_wall->next->end;
	e = &room->first_wall->start;
	//printf("Calculating between points X1Y1[%d, %d] and X2Y2[%d, %d]\n", s->x, s->y, e->x, e->y);
	distance_c = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_c = sqrt(distance_c);
	return ((t_tri_sides){distance_a, distance_b, distance_c});
}

static t_tri_sides room_second_triangle(t_room *room)
{
	t_point		*s;
	t_point		*e;
	double 		distance_a;
	double		distance_b;
	double		distance_c;

	s = &room->first_wall->start;
	e = &room->first_wall->next->end;
	//printf("Calculating between points X1Y1[%d, %d] and X2Y2[%d, %d]\n", s->x, s->y, e->x, e->y);
	distance_a = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_a = sqrt(distance_a);
	s = &room->first_wall->next->end;
	e = &room->first_wall->next->next->end;
	//printf("Calculating between points X1Y1[%d, %d] and X2Y2[%d, %d]\n", s->x, s->y, e->x, e->y);
	distance_b = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_b = sqrt(distance_b);
	s = &room->first_wall->next->next->end;
	e = &room->first_wall->start;
	//printf("Calculating between points X1Y1[%d, %d] and X2Y2[%d, %d]\n", s->x, s->y, e->x, e->y);
	distance_c = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_c = sqrt(distance_c);
	return ((t_tri_sides){distance_a, distance_b, distance_c});
}

static void debug_triangle_line(t_line *line, t_point *s, t_point *e, t_editor *edt)
{
	// Disable or enable debugging visuals by enabling return.
	return ;
	line->x1 = s->x;
	line->y1 = s->y;
	line->x2 = e->x;
	line->y2 = e->y;
	line->color = 0xffff00ff;
	line->buff = edt->buff;
	render_line(line);
}

static t_tri_sides room_n_triangle(t_room *room, int n, t_editor *edt)
{
	t_line		line;
	t_wall 		*n_wall;
	t_point		*s;
	t_point		*e;
	double 		distance_abc[2];

	n_wall = room->first_wall;
	while (n--)
		n_wall = n_wall->next;
	s = &room->first_wall->start;
	e = &n_wall->end;
	debug_triangle_line(&line, s, e, edt);
	distance_abc[0] = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_abc[0] = sqrt(distance_abc[0]);
	s = &n_wall->end;
	e = &n_wall->next->end;
	debug_triangle_line(&line, s, e, edt);
	distance_abc[1] = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_abc[1] = sqrt(distance_abc[1]);
	s = &n_wall->next->end;
	e = &room->first_wall->start;
	debug_triangle_line(&line, s, e, edt);
	distance_abc[2] = ((e->x - s->x) * (e->x - s->x)) + ((e->y - s->y) * (e->y - s->y));
	distance_abc[2] = sqrt(distance_abc[2]);
	return ((t_tri_sides){distance_abc[0], distance_abc[1], distance_abc[2]});
}

static double	triangle_area(t_tri_sides tri)
{
	double	s;

	//printf("Triangle_area a: %f | b: %f | c: %f\n", tri.a, tri.b, tri.c);
	if (tri.a < 0 || tri.b < 0 || tri.c < 0 ||
		(tri.a + tri.b <= tri.c) || tri.a + tri.c <= tri.b || tri.b + tri.c <= tri.a)
	{
		ft_putendl("Warning: Invalid triangle at triangle_area");

		return (0);
	}
	s = (tri.a + tri.b + tri.c) / 2;
	return (sqrt(s * (s - tri.a) * (s - tri.b) * (s - tri.c)));

}

static t_point barycentric_xy(t_editor *edt, t_room *room)
{
	t_point		v1;
	t_point 	v2;
	t_point 	v3;
	t_wall		*second_wall;

	v1.x = room->first_wall->start.x;
	v1.y = room->first_wall->start.y;
	v2.x = room->first_wall->end.x;
	v2.y = room->first_wall->end.y;
	second_wall = room->first_wall->next;
	v3.x = second_wall->end.x;
	v3.y = second_wall->end.y;
	return ((t_point){(v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3});
}

static t_point second_barycentric_xy(t_editor *edt, t_room *room)
{
	t_point		v1;
	t_point 	v2;
	t_point 	v3;

	v1.x = room->first_wall->start.x;
	v1.y = room->first_wall->start.y;
	v2.x = room->first_wall->next->end.x;
	v2.y = room->first_wall->next->end.y;
	v3.x = room->first_wall->next->next->end.x;
	v3.y = room->first_wall->next->next->end.y;
	return ((t_point){(v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3});
}

static t_point n_visual_xy(t_room *room, int n)
{
	t_point		v1;
	t_point 	v2;
	t_point 	v3;
	t_wall		*n_wall;

	n_wall = room->first_wall;
	while (n--)
		n_wall = n_wall->next;
	v1.x = room->first_wall->start.x;
	v1.y = room->first_wall->start.y;
	v2.x = n_wall->end.x;
	v2.y = n_wall->end.y;
	v3.x = n_wall->next->end.x;
	v3.y = n_wall->next->end.y;
	return ((t_point){(v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3});
}

static t_point split_barycentric_xy(t_editor *edt, t_room *room)
{
	double 		area1;
	double 		area2;
	t_point		visual1;
	t_point		visual2;
	t_point 	result;

	area1 = 0;
	area2 = 0;
	area1 = triangle_area(room_first_triangle(room));
	area2 = triangle_area(room_second_triangle(room));
	visual1 = barycentric_xy(edt, room);
	visual2 = second_barycentric_xy(edt, room);
	result.x = ((visual1.x * (int)area1) + (visual2.x * (int)area2)) / (int)(area1 + area2);
	result.y = ((visual1.y * (int)area1) + (visual2.y * (int)area2)) / (int)(area1 + area2);
	//printf("Calculated triangle areas: [%f | %f]\n", area1, area2);
	//printf("Calculated barycentric center points: [%d, %d] | [%d, %d]\n",
	//	visual1.x, visual1.y, visual2.x, visual2.y);
	return (result);
}

static t_point n_barycentric_xy(t_editor *edt, t_room *room, int n)
{
	double	*areas;
	t_point	*visuals;
	t_point	weighted_result;
	int 	weight;
	int		i;

	i = 0;
	areas = (double*)malloc(sizeof(double) * n);
	visuals = (t_point*)malloc(sizeof(t_point) * n);
	if (!areas || !visuals)
		ft_die("Fatal error: Mallocing areas, visuals failed at n_barycentric_xy.");
	areas[i] = triangle_area(room_first_triangle(room)) / 100;
	visuals[i] = barycentric_xy(edt, room);
	//circle_visual(edt->parent, &visuals[i]);
	weighted_result.x = ((int)areas[i] * visuals[i].x);
	weighted_result.y = ((int)areas[i] * visuals[i].y);
	weight = (int)areas[i];
	//printf("Weighted result x = %d, y = %d, total_weight = %d\n", weighted_result.x, weighted_result.y, weight);
	while(++i < n - 2)
	{
		areas[i] = triangle_area(room_n_triangle(room, i, edt)) / 100;
		weight += (int)areas[i];
		visuals[i] = n_visual_xy(room, i);
		//circle_visual(edt->parent, &visuals[i]);
		weighted_result.x += ((int)areas[i] * visuals[i].x);
		weighted_result.y += ((int)areas[i] * visuals[i].y);
		//printf("Weighted result x = %d, y = %d, total_weight = %d\n", weighted_result.x, weighted_result.y, weight);
	}
	weighted_result.x = weighted_result.x / weight;
	weighted_result.y = weighted_result.y / weight;
	//printf("Weighted result divided final values x = %d, y = %d, division total_weight was %d\n", weighted_result.x, weighted_result.y, weight);
	//ft_putendl("N_Barycentric done iterating room_n_triangles, n_visual_xys.");
	free(areas);
	free(visuals);
	return (weighted_result);
}

static int room_portals(t_editor *edt, t_room *room)
{
	t_wall *wall;
	t_wall *portal;
	int pc;
	int wc;
	int included_portals_in_room;
	int flagged_for_inclusion;

	pc = edt->portal_count;
	if (pc == 0)
	{
		//ft_putendl("Skipping room_portals, portal_count was zero.");
		return (0);
	}
	portal = edt->portal_begin;
	included_portals_in_room = 0;
	while (pc--)
	{
		flagged_for_inclusion = 0;
		//ft_putendl("Room_portals checking against a portal for hits.");
		wc = room->wall_count;
		wall = room->first_wall;
		while (wc--)
		{
			if ((wall->start.x == portal->end.x && wall->start.y == portal->end.y) ||
					(wall->start.x == portal->start.x && wall->start.y == portal->start.y) ||
					(wall->end.x == portal->end.x && wall->end.y == portal->end.y) ||
					(wall->end.x == portal->start.x && wall->end.y == portal->start.y))
			{
				flagged_for_inclusion++;
				//ft_putendl("Found a portal for including into a room in room_portals.");
			}
			wall = wall->next;
		}
		if (flagged_for_inclusion)
			included_portals_in_room++;
		portal = portal->next;
	}
	return (included_portals_in_room);
}

static void find_visual_xy(t_editor *edt, t_room *room)
{
	t_point	visual_xy;

	//ft_putendl("Finding weighted centroid of convex polygon.");
	if (room->wall_count == 3)
	{
		//ft_putendl("Barycentric visual can be found from the triangle.");
		visual_xy = barycentric_xy(edt, room);
		//printf("Visual located to: %d, %d\n", visual_xy.x, visual_xy.y);
		room->visual = visual_xy;
		circle_room(edt->parent, room);
	}
	else if (room->wall_count == 4)
	{
		visual_xy = split_barycentric_xy(edt, room);
		//printf("Visual located to: %d, %d\n", visual_xy.x, visual_xy.y);
		room->visual = visual_xy;
		circle_room(edt->parent, room);
	}
	else if (room->wall_count > 4)
	{
		visual_xy = n_barycentric_xy(edt, room, room->wall_count + room_portals(edt, room));
		room->visual = visual_xy;
		circle_room(edt->parent, room);
		//ft_putendl("Polygon can be triangulated into ");
		//ft_putnbr(room->wall_count - 2);
		//ft_putendl(" triangles, which can be measured for area, and calculated for their barycentric center.");
	}
}

static void circle_rooms(t_doom *doom)
{
	int		rc;
	t_room	*room;

	if (doom->edt->room_count == 0)
		return;
	rc = doom->edt->room_count;
	room = doom->edt->room_first;
	while (rc--)
	{
		room->visual.x = 0;
		room->visual.y = 0;
		find_visual_xy(doom->edt, room);
		if (room->visual.x > 0 && room->visual.y > 0)
			circle_room(doom, room);
		room = room->next;
	}
}


static void	record_room(t_editor *edt)
{
	t_room	*next_room;
	int 	wallcountofpreviousrooms;

	wallcountofpreviousrooms = wall_count_of_previous_rooms(edt);
	edt->rooms->id = edt->room_count;
	edt->rooms->floor_height = 1000;
	edt->rooms->roof_height = 1300;
	edt->room_count++;
	next_room = (t_room*)malloc(sizeof(t_room));
	if (!next_room)
		ft_die("Fatal error: Could not malloc t_room at record_room.");
	if (edt->room_count == 1)
	{
		edt->rooms->first_wall = edt->wall_begin;
		edt->rooms->wall_count = edt->wall_count;
		edt->room_first = edt->rooms;
	}
	else
	{
		edt->rooms->wall_count = edt->wall_count - wallcountofpreviousrooms;
		edt->rooms->first_wall = wall_by_count(edt, wallcountofpreviousrooms);
	}
	edt->rooms->first_wall_id = edt->rooms->first_wall->id;
	printf("Room id: %d | first_wall_id: %d | wall_count: %d | floor_height: %d | roof_height: %d\n",
			edt->rooms->id, edt->rooms->first_wall_id, edt->rooms->wall_count, edt->rooms->floor_height, edt->rooms->roof_height);
	expand_room_string(edt);
	find_visual_xy(edt, edt->rooms);
	edt->rooms->next = next_room;
	edt->rooms = next_room;
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
	edt->enemies->wep.type_id = 0;
	edt->enemies->hp.max = 100;
	edt->enemies->rot = degree_rot(edt->enemies->x, edt->enemies->y, &edt->enemies->tail);
	expand_enemy_string(edt);
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
	}
	else if (edt->player_set == -1)
	{
		edt->player.tail.x = x;
		edt->player.tail.y = y;
		start.x = edt->player.x;
		start.y = edt->player.y;
		modify_line_length(15, &start, &edt->player.tail, &edt->player.tail);
		edt->player.rot = degree_rot(edt->player.x, edt->player.y, &edt->player.tail);
		edt->player_set = 1;
		update_player_string(edt);
	}
	print_walls(edt);
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
			//ft_putendl("Portalization started.");
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
		//ft_putendl("Polygon binding.");
		edt->walls->end.x = edt->polygon_start_x;
		edt->walls->end.y = edt->polygon_start_y;
		expand_wall_string(edt);
		edt->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (edt->wall_count == 1)
			edt->wall_begin = edt->walls;
		edt->walls->next = next_wall;
		edt->walls = next_wall;
		edt->is_wall_start = 1;
		record_room(edt);
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
				edt->portalization_ending = 0;
				edt->portalization_a = NULL;
				edt->portalization_b = NULL;
				edt->is_wall_start = 1;
				//ft_putendl("Portal created.");
			}
			else
			{
				//ft_putendl("Negative.");
			}
		}
		expand_wall_string(edt);
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
		if (edt->is_wall_start)
			record_room(edt);
		print_walls(edt);
	}
}

static void edt_left_click(t_doom *doom)
{
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

static t_wall	*wall_by_id(t_editor *edt, int id)
{
	t_wall *wall;
	int 	wc;

	wc = edt->wall_count;
	wall = edt->wall_begin;
	while (wc--)
	{
		if (wall->id == id)
			return (wall);
		wall = wall->next;
	}
	ft_die("Fatal error: Could not find a wall by its ID in wall_by_id.");
	return (NULL);
}

static void		link_rooms(t_editor *edt)
{
	t_room *room;
	int 	rc;

	rc = edt->room_count;
	room = edt->room_first;
	while (rc--)
	{
		room->first_wall = wall_by_id(edt, room->first_wall_id);
		room = room->next;
	}
}

void			transfer_model_to_editor(t_doom *doom)
{
	int	ec;

	ft_putendl("Transferring model to editor.");
	doom->edt->walls = doom->mdl->walls;
	doom->edt->rooms = doom->mdl->rooms;
	doom->edt->enemies = doom->mdl->enemies;
	doom->edt->portals = doom->mdl->portals;
	doom->edt->enemy_first = doom->mdl->enemy_first;
	doom->edt->portal_begin = doom->mdl->portal_first;
	doom->edt->wall_begin = doom->mdl->wall_first;
	doom->edt->room_first = doom->mdl->room_first;
	doom->edt->wall_count = doom->mdl->wall_count;
	doom->edt->room_count = doom->mdl->room_count;
	doom->edt->enemy_count = doom->mdl->enemy_count;
	doom->edt->portal_count = doom->mdl->portal_count;
	doom->edt->player = doom->mdl->player;
	doom->edt->player_set = 1;
	circle_player(doom);
	link_rooms(doom->edt);
	if (doom->map->player_string)
		doom->edt->player_string = ft_strdup(doom->map->player_string);
	if (doom->map->wall_string)
		doom->edt->wall_string = ft_strdup(doom->map->wall_string);
	if (doom->map->room_string)
		doom->edt->room_string = ft_strdup(doom->map->room_string);
	if (doom->map->portal_string)
		doom->edt->portal_string = ft_strdup(doom->map->portal_string);
	if (doom->map->enemy_string)
		doom->edt->enemy_string = ft_strdup(doom->map->enemy_string);
	print_walls(doom->edt);
	circle_rooms(doom);
	ec = doom->edt->enemy_count;
	if (ec == 0)
		return ;
	doom->edt->enemies = doom->edt->enemy_first;
	doom->edt->last_enemy.x = doom->edt->enemies->x;
	doom->edt->last_enemy.y = doom->edt->enemies->y;
	while (ec--)
	{
		circle_enemy(doom);
		doom->edt->enemies = doom->edt->enemies->next;
		doom->edt->last_enemy.x = doom->edt->enemies->x;
		doom->edt->last_enemy.y = doom->edt->enemies->y;
	}
	doom->edt->enemies = doom->mdl->enemies;
}