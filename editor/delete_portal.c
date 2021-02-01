#include "doom-nukem.h"

static int		equal_points(t_point a, t_point b)
{
	if (a.x == b.x && a.y == b.y)
		return (1);
	return (0);
}

static int		matching_walls(t_wall *a, t_wall *b)
{
	if (equal_points(a->start, b->start) && equal_points(a->end, b->end))
		return (1);
	if (equal_points(a->start, b->end) && equal_points(a->end, b->start))
		return (1);
	return (0);
}

static int		portal_belongs_to_room(t_wall *portal, t_room *room)
{
	t_wall	*wall;
	int 	wc;

	wall = room->first_wall;
	wc = room->wall_count;
	while (wc--)
	{
		if (matching_walls(portal, wall))
			return (1);
		wall = wall->next;
	}
	return (0);
}

void			delete_portals_by_room(t_room *room, t_model *mdl)
{
	t_wall	*portal;
	int		pc;
		//int	debug_id;

		//debug_model_portals();
		//ft_putendl("\nDeleting portals by room\n");
	pc = mdl->portal_count;
	portal = mdl->portal_first;
		//debug_id = 0;
	while (pc--)
	{
		if (portal_belongs_to_room(portal, room))
		{
			delete_portal(portal, mdl);
				//printf("deleted portal id %d\n", debug_id);
		}
		portal = portal->next;
			//debug_id++;
	}
	debug_model_portals();
}

static void		update_remaining_portals(t_wall *delete, t_model *mdl)
{
	t_wall	*portal;
	t_wall	*relink;
	int		pc;

	pc = mdl->portal_count;
	portal = mdl->portal_first;
	if (mdl->portal_first == delete && mdl->portal_count > 1)
		mdl->portal_first = delete->next;
	while (pc--)
	{
		if (portal->id == delete->id - 1)
		{
			relink = portal;
			relink->next = NULL;
		}
		if (portal->id == delete->id + 1)
			relink->next = portal;
		portal->id--;
		portal = portal->next;
	}
	if (relink->next == NULL)
		relink->next = mdl->portals;
}

void			delete_portal(t_wall *portal, t_model *mdl)
{
	update_remaining_portals(portal, mdl);
	mdl->portal_count--;
	free(portal);
}
