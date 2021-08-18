#include "doom-nukem.h"

t_room				*room_by_id(int id)
{
	t_room			*room;
	int				rc;

	rc = get_model()->room_count;
	room = get_model()->room_first;
	while (rc--)
	{
		if (room->id == id)
			return (room);
		room = room->next;
	}
	return (NULL);
}

t_wall				*portal_by_id(int id)
{
	int		pc;
	t_wall 	*portal;

	if (id < 0)
	{
		ft_putendl("Warning: Portal_by_id was requested to return a portal with id less than 0. Returned NULL.");
		return (NULL);
	}
	if (get_model()->portal_count != 0)
		pc = get_model()->portal_count;
	if (pc <= id)
	{
		ft_putendl("Warning: Portal_by_id was requested to return a portal with id equal or less than portal_count of get_model(). Returned NULL.");
		return (NULL);
	}
	portal = get_model()->portal_first;
	while (pc--)
	{
		if (portal->id == id)
			return (portal);
		portal = portal->next;
	}
	ft_putendl("Warning: Portal_by_id could not find a portal with the requested id. Returned NULL.");
	return (NULL);
}

t_wall				*wall_by_id(int id)
{
	int		wc;
	t_wall 	*wall;

	if (id < 0)
	{
		ft_putendl("Warning: Wall_by_id was requested to return a wall with id less than 0. Returned NULL.");
		return (NULL);
	}
	if (get_model()->wall_count != 0)
		wc = get_model()->wall_count;
	if (wc <= id)
	{
		ft_putendl("Warning: Wall_by_id was requested to return a wall with id equal or less than wall_count of get_model(). Returned NULL.");
		return (NULL);
	}
	wall = get_model()->wall_first;
	while (wc--)
	{
		if (wall->id == id)
			return (wall);
		wall = wall->next;
	}
	ft_putendl("Warning: Wall_by_id could not find a wall with the requested id. Returned NULL.");
	return (NULL);
}

t_wall	*portal_by_wall(t_wall *wall)
{
	t_wall	*portal;
	int		pc;

	portal = get_model()->portal_first;
	pc = get_model()->portal_count;
	while (pc--)
	{
		if (matching_walls(wall, portal))
			return (portal);
		portal = portal->next;
	}
	return (NULL);
}
