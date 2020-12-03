#ifndef MODEL_LL_H
# define MODEL_LL_H

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
typedef struct	s_global_vert
{
	t_vert					pos;
	struct s_global_vert	*prev;
	struct s_global_vert	*next;
}				t_global_vert;

// list of vertices for the current face
typedef struct	s_face_vert
{
	t_global_vert		*vert;
	struct s_face_vert	*prev;
	struct s_face_vert	*next;
}				t_face_vert;

// list of faces for the current object
typedef struct	s_face_node
{
	t_face_vert			*face;
	struct s_face_node	*prev;
	struct s_face_node	*next;
}				t_face_node;

typedef struct	s_obj
{
	int				v_count;
	int				f_count;
	t_global_vert	*v_list;
	t_face_node		*f_list;
}				t_obj;

// obj object;
// t_vert f1v1 = object->f_list-> face-> vert.pos;
// t_vert f2v2 = object->f_list-> face->next-> vert->next.pos;

# include "renderer.h"

#endif
