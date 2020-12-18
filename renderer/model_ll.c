#include "renderer.h"

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

// Appends to, or creates a new list, then returns head.
// Note: Might want to avoid these when appending lots of things.
t_global_vert	*global_vert_add(t_global_vert **vlist, t_global_vert *tail)
{
	t_global_vert	*last;

	if (vlist != NULL)
	{
		if ((*vlist) != NULL)
		{
			last = list2vert(*vlist, LAST);
			last->next = tail;
			tail->prev = last;
		}
		else
		{
			*vlist = tail;
		}
		return (*vlist);
	}
	return (NULL);
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

t_actual_face	*face_list_add(t_actual_face **head, t_actual_face *tail)
{
	t_actual_face	*last;

	if (head != NULL)
	{
		if ((*head) != NULL)
		{
			last = list2face(*head, LAST);
			last->next = tail;
			tail->prev = last;
		}
		else
		{
			*head = tail;
		}
		return (*head);
	}
	return (NULL);
}

// Allocates new t_global_vert node (with actual vert data) and appends it to the a global list.
t_global_vert	*new_vert(t_global_vert **vlist, t_vert v)
{
	t_global_vert	*current;

	assert(current = malloc(sizeof(*current)));

	current->pos = v;
	current->prev = NULL;
	current->next = NULL;
	global_vert_add(vlist, current);
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
t_actual_face	*new_face(t_global_vert **vlist, t_vert a, t_vert b, t_vert c)
{
	t_actual_face	*out;

	assert(out = malloc(sizeof(*out)));

	out->prev = NULL;
	out->vert             = new_fvert(NULL,            new_vert(vlist, a), NULL);
	out->vert->next       = new_fvert(out->vert,       new_vert(vlist, b), NULL);
	out->vert->next->next = new_fvert(out->vert->next, new_vert(vlist, c), NULL);
	out->next = NULL;
	return (out);
}

// - Builds a vertical wall from a 2D line as if viewed top-down.
// - The wall consists of two counter-clockwise triangles.
// - Two verts will be at the same XY coordinate but different Z (height)
// - The third vert will be at the opposite end of the line.
// - A second triangle is made with the same method,
//	only with two points at the opposite end instead.
t_actual_face	*make_wall(t_global_vert **vlist, t_wall *a, t_wall *b, int floor, int roof)
{
	t_vert			v[4];
	t_actual_face	*f[2];

	v[0] = vec4_mul(vec4(a->start.x, a->start.y, roof,  T_POS), WORLD_SCALE_FACTOR);
	v[1] = vec4_mul(vec4(a->start.x, a->start.y, floor, T_POS), WORLD_SCALE_FACTOR);
	v[2] = vec4_mul(vec4(b->start.x, b->start.y, roof,  T_POS), WORLD_SCALE_FACTOR);
	v[3] = vec4_mul(vec4(b->start.x, b->start.y, floor, T_POS), WORLD_SCALE_FACTOR);

	f[0] = new_face(vlist, v[0], v[1], v[2]);
	f[1] = new_face(vlist, v[1], v[3], v[2]);

	return (face_list_add(&f[0], f[1]));
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

	t_actual_face	*single_wall = NULL;

	room_object.v_count = 0;
	room_object.f_count = 0;
	room_object.face = NULL;
	room_object.vert = NULL;

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
				single_wall = make_wall(&room_object.vert, wall, wall->next, floor_height, roof_height);
			else
				single_wall = make_wall(&room_object.vert, wall, room->first_wall, floor_height, roof_height);

			// join those triangles to some bigger list
			face_list_add(&room_object.face, single_wall);
			room_object.v_count += 6;
			room_object.f_count += 2;
			wall = wall->next;
		}
		room = room->next;
	}

	printf("FINAL ROOM STATS: %d verts, %d faces, v:%p, f:%p\n", room_object.v_count, room_object.f_count, room_object.vert, room_object.face);

	// Tests to make sure there are the correct amount of nodes:
	// int x = room_object.f_count, y = room_object.v_count;
	// while (--x) room_object.face = room_object.face->next; assert(room_object.face->next == NULL);
	// while (--y) room_object.vert = room_object.vert->next; assert(room_object.vert->next == NULL);

	return (room_object);
}

// Todo: Rewrite the previous model.c functions for the new structure.

void			recursive_vert_free(t_global_vert *vert)
{
	if (vert->next)
		recursive_vert_free(vert->next);
	free(vert);
}

void			recursive_fvert_free(t_face_vert *fvert)
{
	if (fvert->next)
		recursive_fvert_free(fvert->next);
	free(fvert);
}

void			recursive_face_free(t_actual_face *face)
{
	recursive_fvert_free(face->vert);
	if (face->next)
		recursive_face_free(face->next);
	free(face);
}

void			free_obj(t_obj obj)
{
	recursive_vert_free(obj.vert);
	recursive_face_free(obj.face);
}

// Allocates a new face (triangle) after applying the given matrix.
t_actual_face	*face_transform(t_global_vert **vlist, t_matrix m, t_actual_face *f)
{
	return (new_face(vlist,
		apply_m(m, f->vert->data->pos),
		apply_m(m, f->vert->next->data->pos),
		apply_m(m, f->vert->next->next->data->pos)
	));
}

/*
** \brief Applies the given matrix to all faces in the mesh.
** \return Newly allocated data, containing the transformed faces.
*/
t_obj			obj_transform(t_matrix m, t_obj obj)
{
	t_obj out;

	out = (t_obj){obj.v_count, obj.f_count, NULL, NULL};

	assert(obj.f_count >= 1);

	out.face = face_transform(&out.vert, m, obj.face);

	while (obj.face != NULL)
	{
		face_list_add(&out.face, face_transform(&out.vert, m, obj.face));
		obj.face = obj.face->next;
	}

	return (out);
}