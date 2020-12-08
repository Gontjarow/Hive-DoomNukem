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

t_face_vert		*list2fvert(t_face_vert *list, int index)
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

t_face_vert		*list2face(t_actual_face *list, int index)
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

t_global_vert	*global_vert_add(t_global_vert *head, t_global_vert *node)
{
	t_global_vert	*last;

	if (head != NULL)
	{
		last = list2vert(head, LAST);
		last->next = node;
		node->prev = last;
	}
	else
	{
		head = node;
	}

	return (head);
}

t_face_vert		*face_vert_add(t_face_vert *head, t_face_vert *tail)
{
	t_face_vert	*last;

	if (head != NULL)
	{
		last = list2fvert(head, LAST);
		last->next = tail;
		tail->prev = last;
	}
	else
	{
		head = tail;
	}
	return (head);
}

t_actual_face	*face_list_add(t_actual_face *head, t_actual_face *tail)
{
	t_actual_face	*last;

	if (head != NULL)
	{
		last = list2face(head, LAST);
		last->next = tail;
		tail->prev = last;
	}
	else
	{
		head = tail;
	}

	return (head);
}

// Allocates new t_global_vert node (with actual vert data) and appends it to the a global list.
t_global_vert	*new_vert(t_global_vert *prev, t_vert v, t_global_vert *next)
{
	t_global_vert	*current;

	assert(current = malloc(sizeof(*current)));

	current->pos = v;
	current->prev = prev;
	current->next = next;
	g_verts = global_vert_add(g_verts, current);
	return (current);
}

// Allocates new t_face_vert node and assigns values to it.
t_face_vert		*new_fvert(t_face_vert *prev, t_global_vert *data, t_face_vert *next)
{
	t_face_vert	*current;

	assert(current = malloc(sizeof(*current)));

	current->data = data;
	current->prev = prev;
	current->next = next;
	return (current);
}

// Allocates new t_actual_face node and the appropriate sub-structs.
t_actual_face	*new_face(t_vert a, t_vert b, t_vert c)
{
	t_actual_face	*out;

	assert(out = malloc(sizeof(*out)));

	out->prev = NULL;
	out->vert             = new_fvert(NULL,            new_vert(NULL, a, NULL), NULL);
	out->vert->next       = new_fvert(out->vert,       new_vert(NULL, b, NULL), NULL);
	out->vert->next->next = new_fvert(out->vert->next, new_vert(NULL, c, NULL), NULL);
	out->next = NULL;
	return (out);
}

// - Builds a vertical wall from a 2D line as if viewed top-down.
// - Two verts will be at the same XY coordinate but different Z (height)
// - The third vert will be at the opposite end of the line.
// - A second triangle is made with the same method,
//	only with two points at the opposite end instead.
t_actual_face	*make_wall(t_wall *a, t_wall *b, int floor, int roof)
{
	t_vert			v[3];
	t_actual_face	*f[2];

	v[0] = vec4(a->start.x, a->start.y, roof, T_POS);
	v[1] = vec4(a->start.x, a->start.y, floor, T_POS);
	v[2] = vec4(b->start.x, b->start.y, roof, T_POS);
	f[0] = new_face(v[0], v[1], v[2]);

	v[0] = vec4(a->start.x, a->start.y, floor, T_POS);
	v[1] = vec4(b->start.x, b->start.y, floor, T_POS);
	v[2] = vec4(b->start.x, b->start.y, roof, T_POS);
	f[1] = new_face(v[0], v[1], v[2]);

	return (face_list_add(f[0], f[1]));
}

t_obj			mdl_to_usable_data(t_doom *doom)
{
	t_obj			room_object;

	int				wall_count;
	int				room_count;
	int				floor_height;
	int				roof_height;
	t_wall			*wall = NULL;
	t_room			*room = NULL;

	t_actual_face	*wall_tris = NULL;
	t_actual_face	*single_wall = NULL;

	room_object.v_count = 0;
	room_object.f_count = 0;

	room_count = doom->mdl->room_count;
	room       = doom->mdl->room_first;
	while (room_count--)
	{
		floor_height = room->floor_height;
		roof_height  = room->roof_height;
		wall_count   = room->wall_count;
		wall         = room->first_wall;
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
			wall_tris = face_list_add(wall_tris, single_wall);
			room_object.v_count += 6;
			room_object.f_count += 2;
			wall = wall->next;
		}
		room = room->next;
	}

	room_object.face = wall_tris;
	room_object.vert = g_verts;
	printf("FINAL ROOM STATS: %d verts, %d faces, v:%p, f:%p\n",
		room_object.v_count,
		room_object.f_count,
		room_object.vert,
		room_object.face);
	return (room_object);
}
