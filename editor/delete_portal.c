#include "doom-nukem.h"

void			delete_portals_by_room(t_room *room, t_model *mdl)
{
	// Add deletion of portals in a room;
	return ;
}

static void		update_remaining_portals(int id, t_model *mdl)
{
	t_wall	*portal;
	t_wall	*relink;
	int		pc;

	pc = mdl->portal_count;
	portal = mdl->portal_first;
	if (id == 0 && mdl->portal_count > 1)
		mdl->portal_first = mdl->portal_first->next;
	while (pc--)
	{
		if (portal->id == id - 1)
			relink = portal;
		if (portal->id == id + 1)
			relink->next = portal;
		portal = portal->next;
	}
}

void			delete_portal(t_wall *portal, t_model *mdl)
{
	update_remaining_portals(portal->id, mdl);
	mdl->portal_count--;
	free(portal);
}
