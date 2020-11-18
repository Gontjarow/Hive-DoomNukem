#ifndef RENDERER_H
# define RENDERER_H

# include "doom-nukem.h"
# include <assert.h>

# define T_DIR 0.0
# define T_POS 1.0

typedef double	t_deg;
typedef double	t_rad;

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

typedef t_xyzw	t_dir;
typedef t_xyzw	t_pos;
typedef t_xyzw	t_vert;

typedef struct	s_matrix
{
	double m[4][4];
}				t_matrix;

#define Xx m[0][0]
#define Xy m[1][0]
#define Xz m[2][0]
#define Xw m[3][0]

#define Yx m[0][1]
#define Yy m[1][1]
#define Yz m[2][1]
#define Yw m[3][1]

#define Zx m[0][2]
#define Zy m[1][2]
#define Zz m[2][2]
#define Zw m[3][2]

#define Tx m[0][3]
#define Ty m[1][3]
#define Tz m[2][3]
#define Tw m[3][3]

typedef struct	s_cam
{
	t_xyz	pos;
	t_xyz	rot;
}				t_cam;

# include "model.h"
# include "model_ll.h"

double			*get_zbuffer();

void			sort_tri(t_face *face);
void			swap_xyzw(t_xyzw *a, t_xyzw *b);
t_xy			bb_min(t_face face);
t_xy			bb_max(t_face face);
double			edge(t_xy p, t_xy a, t_xy b);
int				inside(t_xy p, t_face face);
t_xyz			bary(t_xy p, t_face face);

void			draw(unsigned int *pixel, t_xy start, t_xy end, int color);
void			draw_tri(unsigned int *pixel, t_face face, int color);
void			draw_tri_color(unsigned int *pixel, t_face face);

void			render_frame(t_doom *doom);

/*
** Math is fun, okay? ⤵️
*/

double			clamp(double n, double min, double max);

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

void			vec3p(t_xyz v);
t_xyz			vec3(double x, double y, double z);
t_xy			vec32(t_xyz v);
t_xyzw			vec34(t_xyz v, double w);
t_xyz			vec3_add(t_xyz a, t_xyz b);
t_xyz			vec3_sub(t_xyz a, t_xyz b);
t_xyz			vec3_mul(t_xyz v, double scalar);
t_xyz			vec3_div(t_xyz v, double divisor);
double			vec3_mag(t_xyz v);
t_xyz			vec3_norm(t_xyz v);
double			vec3_dot(t_xyz a, t_xyz b);
t_xyz			vec3_cross(t_xyz a, t_xyz b);
double			vec3_dist(t_xyz a, t_xyz b);

void			vec4p(t_xyzw v);
t_xyzw			vec4(double x, double y, double z, double w);
t_xyz			vec43(t_xyzw v);
t_xy			vec42(t_xyzw v);
t_xyzw			vec4_add(t_xyzw a, t_xyzw b);
t_xyzw			vec4_sub(t_xyzw a, t_xyzw b);
t_xyzw			vec4_mul(t_xyzw v, double factor);
t_xyzw			vec4_div(t_xyzw v, double divisor);
double			vec4_mag(t_xyzw v);
t_xyzw			vec4_norm(t_xyzw v);
double			vec4_dot(t_xyzw a, t_xyzw b);
t_xyz			vec4_cross(t_xyzw a, t_xyzw b);

void			mat4p(t_matrix m);
t_matrix		identity_m();
t_matrix		scale_m(double x, double y, double z);
t_matrix		translate_m(double x, double y, double z);
t_matrix		rotate_x(t_rad angle);
t_matrix		rotate_y(t_rad angle);
t_matrix		rotate_z(t_rad angle);
t_matrix		multiply_m(t_matrix a, t_matrix b);
t_xyzw			apply_m(t_matrix m, t_xyzw v);
t_mesh			mesh_normalize(t_mesh mesh);

t_matrix		project_pure_m();
t_matrix		project_m(t_deg fov, double aspect, double znear, double zfar);
t_matrix		lookat_m(t_xyz eye, t_xyz at, t_xyz up);
t_matrix		window_m(double znear, double zfar);

#endif
