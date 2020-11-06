#include "doom-nukem.h"

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

void	find_visual_xy(t_editor *edt, t_room *room)
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
		visual_xy = n_barycentric_xy(edt, room, room->wall_count);
		room->visual = visual_xy;
		circle_room(edt->parent, room);
		//ft_putendl("Polygon can be triangulated into ");
		//ft_putnbr(room->wall_count - 2);
		//ft_putendl(" triangles, which can be measured for area, and calculated for their barycentric center.");
	}
}

