#ifndef MESH_MODEL_H
# define MESH_MODEL_H
# include <assert.h>
/*
** A vert has X Y Z components. ("vertex")
** A face is composed of at LEAST three verts. ("ngons" are possible)
** A face cannot have MORE THAN sixteen verts.
** A mesh is composed of at LEAST one face.
*/

typedef struct	s_xy
{
	double x;
	double y;
}				t_xy;

typedef struct	s_xyz
{
	double x;
	double y;
	double z;
}				t_xyz;

typedef struct	s_xyzw
{
	double x;
	double y;
	double z;
	double w;
}				t_xyzw;

typedef t_xyzw	t_vert;


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

# include "doom_nukem.h"

t_mesh			load_mesh_obj(const char *file);

t_face			init_face(int n, ...);
t_mesh			init_mesh(int n, ...);
void			free_verts(t_face *face);
void			free_faces(t_mesh *mesh);

void			mesh_draw(unsigned int *surface, const t_mesh *mesh);
// t_mesh			mesh_transform(t_matrix matrix, t_mesh mesh);

void			vec2p(t_xy v);
t_xy			vec2(double x, double y);
t_xyz			vec23(t_xy v, double z);
t_xy			vec2_add(t_xy a, t_xy b);
t_xy			vec2_sub(t_xy a, t_xy b);
t_xy			vec2_mul(t_xy v, double factor);
t_xy			vec2_div(t_xy v, double divisor);
double			vec2_cross(t_xy a, t_xy b);
double			vec2_slope(t_xy a, t_xy b);
double			vec2_invslope(t_xy a, t_xy b);
t_xy			vec2_clamp(t_xy v, double min, double max);


t_xyz			vec3(double x, double y, double z);
t_xyz			vec3_mul(t_xyz v, double scalar);
t_xyz			vec3_div(t_xyz v, double divisor);
double			vec3_mag(t_xyz v);
t_xyz			vec3_norm(t_xyz v);
double			vec3_dot(t_xyz a, t_xyz b);

void			vec4p(t_xyzw v);
t_xyzw			vec4(double x, double y, double z, double w);
t_xyz			vec43(t_xyzw v);
t_xy			vec42(t_xyzw v);
t_xyzw			vec4_sub(t_xyzw a, t_xyzw b);
t_xyz			vec4_cross(t_xyzw a, t_xyzw b);

#endif
