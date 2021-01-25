#include "doom-nukem.h"

// TODO FOR EDITOR
//		WARN WHEN A POLYGON ROOM ENTERED IS NOT CONVEX // DONE
//		FLIP CLOCKWISENESS WHEN A POLYGON ROOM ENTERED WAS COUNTERCLOCKWISE // DONE
//		DENY RECORDING OF NON-CONVEX ROOMS IN EDTIOR // DONE

static double	angle_abc(t_point a, t_point b, t_point c)
{
	t_point ab = { b.x - a.x, b.y - a.y };
	t_point cb = { b.x - c.x, b.y - c.y };

	double dot = ((double)(ab.x * cb.x) + (double)(ab.y * cb.y));
	double cross = ((double)(ab.x * cb.y) - (double)(ab.y * cb.x));

	return (atan2(cross, dot));
}

static int		inner_checks(double deflection_angle, int *i, int *report_non_convex, int *compare_dir)
{
	int			dir;

	if (deflection_angle != (dir = 0))
		dir = deflection_angle > 0 ? 1 : -1;
	printf("abc angle %d: %f\n", (*i)++, (deflection_angle / M_PI) * 180.0f);
	if (*compare_dir != 0 && *compare_dir != dir)
		*report_non_convex = 1;
	*compare_dir = dir;
}

static void 	flip_wall(t_wall *wall)
{
	t_point		tmp;

	tmp.x = wall->start.x;
	tmp.y = wall->start.y;
	wall->start.x = wall->end.x;
	wall->start.y = wall->end.y;
	wall->end.x = tmp.x;
	wall->end.y = tmp.y;
}

void 			flip_room(t_room *room, t_model *mdl)
{
	t_wall 		**walls;
	t_wall 		*iter;
	int 		i;

	debug_model_walls();
	debug_model_rooms();
	printf("FLIPPING OPERATION BEGIN!\n\n");
	flip_wall(room->first_wall);
	walls = (t_wall**)malloc(sizeof(t_wall*) * room->wall_count);
	if (walls == NULL)
		ft_die("Fatal error: Could not malloc walls pointers at flip_room.");
	walls[0] = room->first_wall;
	iter = room->first_wall;
	i = room->wall_count;
	while (--i > 0)
	{
		walls[i] = iter->next;
		walls[i]->id = room->first_wall_id + i;
		flip_wall(walls[i]);
		iter = iter->next;
	}
	room->first_wall->next = walls[i];
	while (i < room->wall_count - 1)
	{
		walls[i]->next = walls[i + 1];
		printf("walls[%d]->next = walls[(%d + 1) = %d]\n", i, i, i + 1);
		i++;
	}
	walls[i]->next = mdl->walls;
	printf("walls[%d]->next = mdl->walls\n", i);
	printf("OPERATION COMPLETE!\n\n");
	debug_model_walls();
	debug_model_rooms();
	free(walls);
}

int				is_clockwise_convex_polygon(t_room *room)
{
	double	deflection_angle;
		double	sum;
		double 	running;
	int 	i;
	int		wc;
	int 	plus;
	int 	minus;
	int 	compare_dir;
	int 	report_non_convex;
	t_wall	*wall;

	sum = 0;
	running = 0;
	i = 0;
	compare_dir = 0;
	report_non_convex = 0;
	plus = 0;
	minus = 0;
	wc = room->wall_count;
	wall = room->first_wall;
	if (wc < 3)
		return (0);
	while (wc--)
	{
		if (wc != 0)
		{
			deflection_angle = (double) angle_abc(wall->start, wall->end, wall->next->end);
			inner_checks(deflection_angle, &i, &report_non_convex, &compare_dir);
				sum += deflection_angle;
				running += (M_PI - fabs(deflection_angle));
			(compare_dir == 1) ? plus++ : minus++;
		}
		if (wc == 0)
			break ;
		wall = wall->next;
	}
	deflection_angle = (double) angle_abc(wall->start, wall->end, room->first_wall->end);
	inner_checks(deflection_angle, &i, &report_non_convex, &compare_dir);
		sum += deflection_angle;
		running += (M_PI - fabs(deflection_angle));
	(compare_dir == 1) ? plus++ : minus++;
	printf("deflection angles sum for %d angles: %f\n", i, (sum / M_PI) * 180.0f);
	printf("deflection angles running sum for %d angles: %f\n", i, (running / M_PI) * 180.0f);
	if (wall->end.x != room->first_wall->start.x || wall->end.y != room->first_wall->start.y)
	{
		if (report_non_convex)
			ft_putendl("WARNING: NON-CONVEX polygon detected!");
		if (plus > minus)
			ft_putendl("WARNING: COUNTER-CLOCKWISE detected!");
		ft_putendl("WARNING: Rooms walls did not form a CLOSED room!");
		return (0);
	}
	if (report_non_convex && plus < minus)
	{
		ft_putendl("WARNING: NON-CONVEX clockwise polygon detected!");
		return (0);
	}
	else if (report_non_convex && minus < plus)
	{
		ft_putendl("WARNING: NON-CONVEX COUNTER-CLOCKWISE polygon detected!");
		return (0);
	}
	else if (report_non_convex)
	{
		ft_putendl("WARNING: NON-CONVEX polygon detected! Could be either clockwise or counter-clockwise?");
		return (0);
	}
	if (plus)
	{
		ft_putendl("WARNING: Convex polygon detected, but it was COUNTER-CLOCKWISE!");
		return (2);
	}
	ft_putendl("Room passed is_clockwise_convex_polygon checks!");
	return (1);
}

