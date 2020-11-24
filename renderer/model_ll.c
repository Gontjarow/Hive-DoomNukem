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

t_face_verts	*new_face(t_vert a, t_vert b, t_vert c)
{
	t_global_vert *v[3];
	t_face_verts *out;

	assert(v[0] = malloc(sizeof(*v[0])));
	assert(v[1] = malloc(sizeof(*v[1])));
	assert(v[2] = malloc(sizeof(*v[2])));
	assert(out = malloc(sizeof(*out)));
	v[0]->v = a;
	v[0]->prev = NULL;
	v[0]->next = v[1];
	v[1]->v = b;
	v[1]->prev = v[0];
	v[1]->next = v[2];
	v[2]->v = c;
	v[2]->prev = v[1];
	v[2]->next = NULL;
	out->p = v[0];
	out->prev = NULL;
	out->next = NULL;
	return (out);
}

t_obj	mdl_to_usable_data()
{
	t_obj			obj;
	t_doom			*doom;

	int				wall_count;
	int				room_count;
	int				floor_height;
	int				roof_height;
	t_wall			*wall;
	t_room			*room;

	t_face_verts	*wall_tris;
	t_face_verts	*single_wall;

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
			// solution: clockwise-sorted room corners, CCW triangles
			// note: CCW corners should work too

			// build linked list with 2 triangles
			single_wall = make_wall(wall, wall->next, floor_height, roof_height);
			// join those triangles to some bigger list
			join_face_list(wall_tris, single_wall);
			wall = wall->next;
		}
		room = room->next;
	}
	// list2face(obj.f_list, 1);
}

t_face_verts	*make_wall(t_wall *a, t_wall *b, int floor, int roof)
{
	t_vert			v[3];
	t_face_verts	*f[2];

	v[0] = vec4(a->start.x, a->start.y, roof, T_POS);
	v[1] = vec4(a->start.x, a->start.y, floor, T_POS);
	v[2] = vec4(b->start.x, b->start.y, roof, T_POS);
	f[0] = new_face(v[0], v[1], v[2]);
	v[0] = vec4(a->start.x, a->start.y, floor, T_POS);
	v[1] = vec4(b->start.x, b->start.y, floor, T_POS);
	v[2] = vec4(b->start.x, b->start.y, roof, T_POS);
	f[0] = new_face(v[0], v[1], v[2]);
	join_face_list(f[0], f[1]);
	return (f[0]);
}

void			join_face_list(t_face_verts *a, t_face_verts *b)
{
	list2face(a, -1)->next = b; // lol
}
