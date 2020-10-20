#include "doom_nukem.h"

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
