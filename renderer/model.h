#ifndef MODEL_H
# define MODEL_H

/*
** A vert has X Y Z components. ("vertex")
** A face is composed of at LEAST three verts. ("ngons" are possible)
** A face cannot have MORE THAN sixteen verts.
** A mesh is composed of at LEAST one face.
*/

typedef struct	s_face
{
	int		verts;
	t_vert	*vert;
}				t_face;

typedef struct	s_mesh
{
	int		faces;
	t_face	*face;
}				t_mesh;

# include "renderer.h"

t_mesh			load_mesh_obj(const char *file);

t_face			init_face(int n, ...);
t_mesh			init_mesh(int n, ...);
void			free_verts(t_face *face);
void			free_faces(t_mesh *mesh);

void			mesh_draw(unsigned int *surface, const t_mesh *mesh);
t_mesh			mesh_transform(t_matrix matrix, t_mesh mesh);

#endif
