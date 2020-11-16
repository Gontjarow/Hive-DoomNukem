#include "doom-nukem.h"

void 				wall_to_buffer(t_wall *wall, SDL_Surface *buff, uint32_t color)
{
	t_line			line;

	line.doom = singleton_doom_pointer(NULL);
	line.buff = buff;
	line.color = color;
	line.x1 = wall->start.x;
	line.y1 = wall->start.y;
	line.x2 = wall->end.x;
	line.y2 = wall->end.y;
	render_line(&line);
}

void				x_walls_to_buffer(int x, t_wall *wall, SDL_Surface *buff, uint32_t color)
{
	t_wall *iter;

	iter = wall;
	while (x--)
	{
		wall_to_buffer(iter, buff, color);
		iter = iter->next;
	}
}

void 				relink_model_walls(t_wall *relinking_wall)
{
	int				wc;
	t_wall			*wall;

	wc = get_model()->wall_count;
	if (wc == 0)
	{
		get_model()->walls = relinking_wall;
		return ;
	}
	wall = get_model()->wall_first;
	while (wc-- > 1)
		wall = wall->next;
	wall->next = relinking_wall;
	get_model()->walls = wall->next;
}

/*
int	wall_count_of_previous_rooms(t_editor *edt)
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
t_wall *wall_by_count(t_editor *edt, int count)
{
	t_wall *wall;

	wall = edt->wall_begin;
	while (count--)
	{
		wall = wall->next;
	}
	return (wall);
}

t_wall	*wall_by_id(t_editor *edt, int id)
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
*/