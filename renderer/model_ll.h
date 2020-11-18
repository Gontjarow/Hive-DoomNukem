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

typedef struct	s_verts
{
	t_vert			v;
	struct s_verts	*prev;
	struct s_verts	*next;
}				t_verts;

typedef struct	s_faces
{
	t_verts			list;
	struct s_faces	*prev;
	struct s_faces	*next;
}				t_faces;

typedef struct	s_obj
{
	int		v_count;
	int		f_count;
	t_verts	v_list;
	t_faces	f_list;
}				t_obj;

# include "renderer.h"

#endif
