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

typedef struct	s_global_vert
{
	t_vert					v;
	struct s_global_vert	*prev;
	struct s_global_vert	*next;
}				t_global_vert;

typedef struct	s_face_vert
{
	t_global_vert		*p;
	struct s_face_vert	*prev;
	struct s_face_vert	*next;
}				t_face_vert;

typedef struct	s_obj
{
	int				v_count;
	int				f_count;
	t_global_vert	*v_list;
	t_face_vert		*f_list;
}				t_obj;

# include "renderer.h"

#endif
