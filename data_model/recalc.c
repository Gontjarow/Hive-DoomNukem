#include "doom-nukem.h"

void	 		recalc_wall_ids(t_model *mdl)
{
	t_wall		*wall;
	int 		wc;
	int 		i;

	puts("Recalcing wall_ids");
	i = 0;
	wall = mdl->wall_first;
	wc = mdl->wall_count;
	while (wc--)
	{
		wall->id = i++;
		wall = wall->next;
	}
	puts("Done recalcing wall_ids");
}

void			recalc_room_ids(t_model *mdl)
{
	t_room		*room;
	int 		rc;
	int 		i;

	puts("Recalcing room_ids");
	i = 0;
	room = mdl->room_first;
	rc = mdl->room_count;
	while (rc--)
	{
		room->id = i++;
		room = room->next;
	}
	puts("Done recalcing room_ids");
}

void 			relink_model_walls(t_wall *relinking_wall)
{
	int			wc;
	t_wall		*wall;

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

