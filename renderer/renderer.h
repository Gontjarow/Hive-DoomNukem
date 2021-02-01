#ifndef RENDERER_H
# define RENDERER_H

# include "doom-nukem.h"
# include <assert.h>

# define DEG_TO_RAD 0.0174532925
# define RAD_TO_DEG 57.2957795
# define PI_BY_TWO 1.57079632679

# define T_DIR 0.0
# define T_POS 1.0

# define NEAR_PLANE -1.01
# define EYE_HEIGHT 6
# define WORLD_SCALE 10
# define MAX_SECTOR_QUEUE 32

// For sprite drawing
# define SPRITE_SCALE 10
# define BYTE_TRANSPARENT 0xFF
# define COLOR_TRANSPARENT 0xFF00FF

// For line clipping
# define LINE_SIDES_CROSS -1
# define LINE_SIDES_EQUAL  1
# define LINE_SIDES_BOUND  0

# define LINE_SIDE_RIGHT   1
# define LINE_SIDE_IN      1

# define LINE_SIDE_LEFT   -1
# define LINE_SIDE_OUT    -1

# define LINE_SIDE_NONE    0

typedef double	t_deg;
typedef double	t_rad;

typedef struct	s_xy
{
	double	x;
	double	y;
}				t_xy;

typedef struct	s_xyz
{
	double	x;
	double	y;
	double	z;
}				t_xyz;

typedef struct	s_xyzw
{
	double	x;
	double	y;
	double	z;
	double	w;
}				t_xyzw;

typedef struct	s_xy_line
{
	t_xy	start;
	t_xy	stop;
	signed	color;
}				t_xy_line;

typedef t_xyzw	t_dir;
typedef t_xyzw	t_pos;
typedef t_xyzw	t_vert;

typedef struct	s_matrix
{
	double m[4][4];
}				t_matrix;

# define Xx m[0][0]
# define Xy m[1][0]
# define Xz m[2][0]
# define Xw m[3][0]

# define Yx m[0][1]
# define Yy m[1][1]
# define Yz m[2][1]
# define Yw m[3][1]

# define Zx m[0][2]
# define Zy m[1][2]
# define Zz m[2][2]
# define Zw m[3][2]

# define Tx m[0][3]
# define Ty m[1][3]
# define Tz m[2][3]
# define Tw m[3][3]

typedef struct	s_cam
{
	t_xyz	pos;
	t_xyz	rot;
}				t_cam;

typedef struct	s_sector
{
	double		floor;
	double		ceil;
	unsigned	vertex_count;
	t_xy		*vertex;
	signed		*neighbors;
}				t_sector;

typedef struct	s_section
{
	int id;
	int left;
	int right;
}				t_section;

typedef struct	s_camera
{
	signed	sector_id;
	t_xyz	position;
	t_xyz	velocity;
	double	angle;
	double	sin;
	double	cos;
	double	yaw;
}				t_camera;

typedef struct	s_world
{
	unsigned	sector_count;
	t_sector	*sectors;
	t_camera	player;
}				t_world;

double			*get_zbuffer();
t_world			*get_world();
t_world			*load_world(t_world *world);

void			draw(unsigned int *pixel, t_xy start, t_xy end, int color);
void			drawline(t_xy_line line, SDL_Surface *surface);
void			draw_box(t_xy center, int radius, int color, SDL_Surface *surface);
void			vertical_line(int column, int start, int end, int color);

void			render_frame(t_doom *doom);
void			render_sector(t_sector *sector, t_section *section, t_doom *doom, int *y_top, int *y_bot);

/*
** Math is fun, okay? ⤵️
*/

double			clamp(double n, double min, double max);

void			vec2p(const char *name, t_xy v);
t_xy			vec2(double x, double y);
t_xyz			vec23(t_xy v, double z);
t_xy			vec2_add(t_xy a, t_xy b);
t_xy			vec2_sub(t_xy a, t_xy b);
t_xy			vec2_mul(t_xy v, double factor);
t_xy			vec2_div(t_xy v, double divisor);
double			vec2_mag(t_xy v);
t_xy			vec2_norm(t_xy v);
double			vec2_dot(t_xy a, t_xy b);
double			vec2_cross(t_xy a, t_xy b);
double			vec2_slope(t_xy a, t_xy b);
double			vec2_invslope(t_xy a, t_xy b);
t_xy			vec2_clamp(t_xy v, double min, double max);
t_xy			vec2_rot(t_xy v, double angle);

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

// Glorious vector/line functions
double			vec2_projected_length(t_xy vec, t_xy normal);
double			vec2_project_to_hypotenuse(t_xy adjescent, t_xy hypotenuse);
double			vec2_point_line_distance(t_xy point, t_xy pos, t_xy dir);
t_xy			vec2_point_to_line(t_xy point, t_xy line, t_xy dir);
signed			vec2_point_side(t_xy point, t_xy start, t_xy end);

t_xy_line		line(double ax, double ay, double bx, double by);
t_xy_line		line_xy(t_xy start, t_xy stop, int color);
t_xy_line		line_relative(t_xy start, t_xy stop, t_xy origin);

void			linep(const char *name, t_xy_line line);
signed			line_is_zero(t_xy_line line);

t_xy_line		line_add_offset(t_xy_line line, t_xy offset);
t_xy_line		line_sub_offset(t_xy_line line, t_xy offset);
t_xy_line		line_rot(t_xy_line line, double angle);
t_xy			line_norm(t_xy_line line);
double			line_mag(t_xy_line line);
void			clip_line(t_xy_line in, t_xy_line *out, t_xy_line plane);

t_xy_line		*set_clip_bounds(t_xy topl, t_xy topr, t_xy botr, t_xy botl);
void			clip_to_bounds(t_xy_line in, t_xy_line *out, t_xy_line edge[4]);

t_xy_line		line_clamp(t_xy_line in, t_xy min, t_xy max);
void			vec2_clip_line(t_xy_line in, t_xy_line *out, t_xy_line plane);

#endif
