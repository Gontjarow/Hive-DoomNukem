#ifndef MODEL_LL_H
# define MODEL_LL_H

// 20 pixels = 1 meter; 1/20 = 0.05
# define WORLD_SCALE_FACTOR 0.05
# define LAST -1

#define CLIP_TRIVIAL_ACCEPT 1
#define CLIP_TRIVIAL_REJECT -1
#define CLIP_REQUIRED 0

#define OUTCODE_LEFT	0b000001
#define OUTCODE_RIGHT	0b000010
#define OUTCODE_UP		0b000100
#define OUTCODE_DOWN	0b001000
#define OUTCODE_NEAR	0b010000
#define OUTCODE_FAR		0b100000

/*
** Object
**	keeps count of its Vert and Face list
** Face list
**	double-linked list (for easy deletion)
**	each node has a list of (CCW) verts (ngons temporarily possible)
**	final faces for rendering should be triangles
** Vert list
**	double-linked list (for easy deletion)
**	each node simply contains t_vert
*/

// global list of vertices for the current object
// s_global_vert->next is the next VERTICE.
// note: next/prev are only used for ADDING/REMOVING vertices.
// note: When ACCESSING t_vert.pos, next/prev are meaningless.
typedef struct	s_global_vert
{
	t_vert					pos;
	struct s_global_vert	*prev;
	struct s_global_vert	*next;
}				t_global_vert;

// list of vertices for the current face
// s_face_vert->next is the next VERTICE OF THIS FACE.
// s_face_vert->data is a reference to the global object vert list.
typedef struct	s_face_vert
{
	t_global_vert		*data;
	struct s_face_vert	*prev;
	struct s_face_vert	*next;
}				t_face_vert;

// list of faces for the current object
// s_actual_face->next is the next FACE
typedef struct	s_actual_face
{
	t_face_vert				*vert;
	struct s_actual_face	*prev;
	struct s_actual_face	*next;
}				t_actual_face;

typedef struct	s_obj
{
	int				v_count;
	int				f_count;
	t_global_vert	*vert;
	t_actual_face	*face;
}				t_obj;

//       Todo: Is this really needed? | Memory efficiency?
//     What do we lose if we drop it? | Duplicate/redundant data?
//            Why can't it be t_vert? | Freeing the pointer might segfault...
//                                    |   * Correction: global verts (aka the memory being referred to) should not be deleted during the frame.
//                                    |   * Extra verts should be discarded at the end of the frame when nothing is needed anymore.
//                                    | Isn't this already redundant?
//                                    | Removing this would free at least 24 (50%) bytes.
//                                    | Would the global list be redundant too then?
//                                    |   * It can't be just t_vert because then t_actual_face could only have one t_vert.
//                                    |   * It can't be a fixed size array because we can't determine how many new verts we'll need.
//                                    |     * Worst case can be over 10 total verts. Too much memory to reserve for all faces.
//                                    |   * Therefore it must be a linked list or realloced with a count?
//                                    |   * Reallocing might be complicated if verts get removed from the middle of the block.
//                                    |   * A linked list with count? Should've done that in the first place, but it's not a definite improvement.
//                                    |     * Linked list with count requires every link to have their counts updated...
//                                    |     * Unless a special "head" struct is added to hold some metadata... More stuff to manage...
//                                    |
// t_obj object;         ↓ face       ↓ vert-ref   ↓ vert-data
// t_vert f1v1 = object->face->       vert->       data.pos;
// t_vert f2v2 = object->face->next-> vert->next-> data.pos;

typedef struct	s_doom t_doom;

# include "renderer.h"

t_global_vert	*new_vert(t_global_vert **list, t_vert v);
t_face_vert		*new_fvert(t_face_vert *prev, t_global_vert *data, t_face_vert *next);
t_actual_face	*new_face(t_global_vert **vlist, t_vert a, t_vert b, t_vert c);

t_global_vert	*list2vert(t_global_vert *list, int index);
t_face_vert		*list2fvert(t_face_vert *list, int index);
t_actual_face	*list2face(t_actual_face *list, int index);

t_global_vert	*global_vert_add(t_global_vert **head, t_global_vert *node);
t_face_vert		*face_vert_add(t_face_vert *head, t_face_vert *tail);
t_actual_face	*face_list_add(t_actual_face **head, t_actual_face *tail);

t_actual_face	*make_wall(t_global_vert **vlist, t_wall *a, t_wall *b, int floor, int roof);

t_obj			mdl_to_usable_data(t_doom *doom);

t_actual_face	*face_transform(t_global_vert **vlist, t_matrix m, t_actual_face *f);
t_obj			obj_transform(t_matrix m, t_obj obj);
t_obj			obj_clip(t_obj obj);

t_vert			lerp_vert(t_vert a, t_vert b);
t_actual_face	*faceclipper(t_actual_face *face, t_global_vert **vlist);
int				get_outcode(t_vert v);
int				get_clip_type(t_actual_face *face);
t_actual_face	*clip_face(t_actual_face *face, t_global_vert **vlist, int clip_type);
t_obj			obj_clip(t_obj obj);

#endif
