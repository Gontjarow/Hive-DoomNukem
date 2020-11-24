#include "renderer.h"
#include "doom-nukem.h"

// https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order/1180256#1180256
// player radius: 10px
// player default move speed: 10px

// positive index: go forward
// negative index: go to the end then reverse
// index == -1   : return last element
t_global_vert	*list2vert(t_global_vert *list, int index)
{
	if (index > 0) //
	{
		while (index-- > 0)
		{
			if (list->next != NULL)
				list = list->next;
			else return (list);
		}
	}
	else if (index < 0)
	{
		while (list->next != NULL)
			list = list->next;
		while (++index < 0)
		{
			if (list->prev != NULL)
				list = list->prev;
			else return (list);
		}
	}
	return (list);
}

t_face_verts	*list2face(t_face_verts *list, int index)
{
	if (index > 0)
	{
		while (index-- > 0)
		{
			if (list->next != NULL)
				list = list->next;
			else return (list);
		}
	}
	else if (index < 0)
	{
		while (list->next != NULL)
			list = list->next;
		while (++index < 0)
		{
			if (list->prev != NULL)
				list = list->prev;
			else return (list);
		}
	}
	return (list);
}
}

t_obj	mdl_to_usable_data()
{
	t_obj obj;
	t_doom *doom;

	int	wall_count;
	int room_count;
	int floor_height;
	int roof_height;
	t_wall *wall;
	t_room *room;

	room_count = doom->mdl->room_count;
	room = doom->mdl->room_first;
	while (room_count--)
	{
		floor_height = room->floor_height;
		roof_height = room->roof_height;
		wall_count = room->wall_count;
		wall = room->first_wall;
		while (wall_count--)
		{
			// line from A -----> B
			// A floor, A roof, B floor
			// line from A <----- B
			// A roof, A floor, B floor
			// vertical up/down?
			t_vert a = vec4(wall->start.x, wall->start.y, roof_height, T_POS);
			t_vert b = vec4(wall->start.x, wall->start.y, floor_height, T_POS);
			t_vert c = vec4(wall->next->start.x, wall->next->start.y, roof_height, T_POS);

			t_vert a = vec4(wall->start.x, wall->start.y, floor_height, T_POS);
			t_vert b = vec4(wall->next->start.x, wall->next->start.y, floor_height, T_POS);
			t_vert c = vec4(wall->next->start.x, wall->next->start.y, roof_height, T_POS);
			wall = wall->next;
		}
		room = room->next;
	}
	// list2face(obj.f_list, 1);
}
