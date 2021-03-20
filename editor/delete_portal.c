/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_portal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:18:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 17:20:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void			delete_portals_by_room(t_room *room, t_model *mdl)
{
	t_wall	*portal;
	int		pc;

	pc = mdl->portal_count;
	portal = mdl->portal_first;
	while (pc--)
	{
		if (portal_belongs_to_room(portal, room))
			delete_portal(portal, mdl);
		portal = portal->next;
	}
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
