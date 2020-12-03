#include "renderer.h"
#include "doom-nukem.h"

static t_global_vert	*g_verts = NULL;

// https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order/1180256#1180256
// player radius: 10px
// player default move speed: 10px

// positive index: go forward
// negative index: go to the end then reverse
// index == -1   : return last element
t_global_vert	*list2vert(t_global_vert *list, int index)
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

t_face_vert		*list2face(t_face_vert *list, int index)
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

void			global_vert_add(t_global_vert *dest, t_global_vert *node)
{
	t_global_vert *last;

	last = list2vert(dest, -1);
	last->next = node;
	node->prev = last;
}

void			face_vert_add(t_face_vert *dest, t_global_vert *node)
{
	t_face_vert *last;

	last = list2face(dest, -1);
	last->next = node;
	node->prev = last;
}

void			join_face_list(t_face_vert *a, t_face_vert *b)
{
	t_face_vert	*last;

	last = list2face(a, -1);
	last->next = b;
	b->prev = last;
}

// adds given verts to the global list
// creates 3 face nodes which contain pointers to verts in global list
t_face_vert	*new_face(t_vert a, t_vert b, t_vert c)
{
	t_global_vert	*current;
	t_global_vert	*last_vert;
	t_face_vert		*list_out;
	t_face_vert		*node;

	last_vert = list2vert(g_verts, -1);
	last_vert->next = new_vert(last_vert, a, NULL);
	assert(node = malloc(sizeof(*node)));
	node->prev = NULL;
	node->p = last_vert->next;

	// Ignore this mess:

	// assert(current = malloc(sizeof(*current)));
	// last->next = current;
	// 	current->v = a;
	// 	current->prev = last;
	// 	current->next = NULL;
	// assert(face = malloc(sizeof(*face)));
	// face->prev = NULL;
	// face->p = current;
	// last = current;

	// assert(current = malloc(sizeof(*current)));
	// face->next = current;
	// last->next = current;
	// 	current->v = b;
	// 	current->prev = last;
	// 	current->next = NULL;
	// assert(face = malloc(sizeof(*face)));
	// face->p = current;
	// face->prev = last;
	// last = current;

	// assert(current = malloc(sizeof(*current)));
	// face->next = current;
	// last->next = current;
	// 	current->v = c;
	// 	current->prev = last;
	// 	current->next = NULL;
	// assert(face = malloc(sizeof(*face)));
	// face->p = current;
	// face->prev = last;
	// last = current;

	return (list_out);
}

t_global_vert	*new_vert(t_global_vert *prev, t_vert v, t_global_vert *next)
{
	t_global_vert	*current;

	assert(current = malloc(sizeof(*current)));

	current->v = v;
	current->prev = prev;
	current->next = next;
	// face->p = current;
	// face->prev = (*last);
	// (*last)->next = current;
	// (*last) = current;
	// face->next = current;
	return (current);
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

	t_face_vert	*wall_tris;
	t_face_vert	*single_wall;

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
			if (wall->next)
				single_wall = make_wall(wall, wall->next, floor_height, roof_height);
			else
				single_wall = make_wall(wall, room->first_wall, floor_height, roof_height);
			// join those triangles to some bigger list
			join_face_list(wall_tris, single_wall);
			wall = wall->next;
		}
		room = room->next;
	}
	obj.v_count
}

t_face_vert	*make_wall(t_wall *a, t_wall *b, int floor, int roof)
{
	t_vert		v[3];
	t_face_vert	*f[2];

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


