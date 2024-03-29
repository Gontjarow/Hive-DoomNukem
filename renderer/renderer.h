#ifndef RENDERER_H
# define RENDERER_H

# include "doom-nukem.h"
# include <assert.h>

# define DEG_TO_RAD 0.0174532925
# define RAD_TO_DEG 57.2957795
# define PI_BY_TWO 1.57079632679

# define T_DIR 0.0
# define T_POS 1.0

# define NEAR_PLANE -0.01
# define EYE_HEIGHT 6
# define WORLD_SCALE 10
# define MAX_SECTOR_QUEUE 32

// For sector rendering
# define NO_NEIGHBOR -1

// For sprite drawing
# define SPRITE_SCALE 2.5
# define BYTE_TRANSPARENT 0xFF
# define COLOR_TRANSPARENT 0xFF00FF
# define DARK_MASK 0b111111001111110011111100
# define FUNC_SETPIXEL int (*fp)(SDL_Surface *, int, int, uint32_t)

// For wall effect drawing
# define POSTER_WIDTH 0.2
# define POSTER_HALF (POSTER_WIDTH / 2)

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

typedef struct	s_sector
{
	int			room_id;
	double		floor;
	double		ceil;
	unsigned	vertex_count;
	t_xy		*vertex;
	signed		*neighbors;
	int			has_ceiling;
	SDL_Surface	**textures;
}				t_sector;

typedef struct	s_section
{
	int id;
	int left;
	int right;
}				t_section;

typedef struct	s_camera
{
	signed		sector_id;
	t_xyz		position;
	t_xyz		velocity;
	double		angle;
	double		sin;
	double		cos;
	double		yaw;
	t_xy_line	*bounds;
}				t_camera;

typedef struct	s_world
{
	unsigned	sector_count;
	t_sector	*sectors;
	t_camera	player;
	int			*screen_y_top;
	int			*screen_y_bot;
}				t_world;

typedef struct	s_queue
{
	t_section	array[MAX_SECTOR_QUEUE];
	t_section	*front;
	t_section	*rear;
}				t_queue;

typedef struct	s_wdata
{
	signed		vertex;
	t_room		*room;
	SDL_Surface	*texture;
	t_xy_line	scale;
	t_xy_line	preclip;
	t_xy_line	render;
	t_xy_line	ceil;
	t_xy_line	floor;
	int			x1;
	int			x2;
	int			neighbor_id;
	double		ceil_angle;
	double		floor_angle;
	t_xy_line	start_clip;
	double		clipped_start;
	double		visible_ratio;
	double		x_step;
	t_sector	*connecting;
}				t_wdata;

typedef struct	s_stripe
{
	int		x;			// screen_x
	int		y;			// screen_y
	double	tx;			// texture_x
	double	ty;			// texture_x
	double	x_delta;	// texture X delta
	double	y_delta;	// texture Y delta
	double	depth;		// Z buffer
	int		y1;			// top pixel
	int		y2;			// bottom pixel
	int		top;		// absolute screen boundary
	int		bot;		// absolute screen boundary
	t_xy	render_width;
}				t_stripe;

double			*get_zbuffer();
t_world			*get_world();
SDL_Surface		*get_bricks_tex(t_doom *doom);
SDL_Surface		*get_border_tex(t_doom *doom);
SDL_Surface		*get_panorama_tex(t_doom *doom);
void			destroy_world(t_world *world);
t_world			*load_world(t_world *world);
void			link_wall_decorators();

void			draw(unsigned int *pixel, t_xy start, t_xy end, int color);
void			drawline(t_xy_line line, SDL_Surface *surface);
void			draw_box(t_xy center, int radius, int color, SDL_Surface *surface);
void			draw_sprite(t_wdata saved, t_stripe screen, FUNC_SETPIXEL);
void			vertical_line(int column, int start, int end, int color);
void			vertical_wall(int screen_x, double tex_x, t_xy range, SDL_Surface *tex, double depth, FUNC_SETPIXEL);
void			vertical_floor(int screen_x, t_xy floor_pos, t_xy range, SDL_Surface *tex, t_doom *doom);
void			vertical_sprite(SDL_Surface *active_sprite, int screen_x, int tex_x, t_xy range, double depth);
void			vertical_shade(int column, int start, int end, int color);

void			render_frame(t_doom *doom);
void			render_sky(t_doom *doom);
void			render_sector(t_sector *sector, t_section *section, t_doom *doom);
void			render_enemies(t_doom *doom);
void			render_sprites(t_doom *doom, int i);

// Somewhat generalized render functions
t_xy_line		calculate_horizontal_scale(t_xy_line segment, t_xy_line *out);
uint32_t		texture_pixel(SDL_Surface *tex, int x, int y);
int				zbuffer_ok(int index, double depth);
t_xy_line		viewer_facing_wall(t_xy location, t_world *world);

t_queue			*get_queue();
void			queue_add(int id, int left, int right);
void			queue_pop();
void			print_queue(t_queue *queue);

/*
** Math is fun, okay? ⤵️
*/

double			clamp(double n, double min, double max);
double			max(double a, double b);
double			min(double a, double b);
double			remap(double in, double old_min, double old_max, double new_min, double new_max);
double			value_changed(int initialize, double value);

void			vec2p(const char *name, t_xy v);
t_xy			vec2(double x, double y);
t_point			point(int x, int y);
t_xy			veci2(t_point v);
t_point			vec2i(t_xy v);
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
t_xy			vec2_lerp(t_xy start, t_xy stop, double t);

t_xy_line		line(double ax, double ay, double bx, double by);
t_xy_line		line_xy(t_xy start, t_xy stop, int color);
t_xy_line		line_relative(t_xy start, t_xy stop, t_xy origin);

void			linep(const char *name, t_xy_line line);
signed			line_is_zero(t_xy_line line);

t_xy_line		line_add_offset(t_xy_line line, t_xy offset);
t_xy_line		line_scale(t_xy_line line, t_xy scale);
t_xy_line		line_rot(t_xy_line line, double angle);
t_xy			line_norm(t_xy_line line);
double			line_mag(t_xy_line line);
void			clip_line(t_xy_line in, t_xy_line *out, t_xy_line plane);
t_xy			line_lerp(t_xy_line line, double t);

t_xy_line		*set_clip_bounds(t_xy topl, t_xy topr, t_xy botr, t_xy botl);
void			clip_to_bounds(t_xy_line in, t_xy_line *out, t_xy_line edge[4]);
void			clip_to_cone(t_xy_line in, t_xy_line *out);

t_xy_line		line_clamp(t_xy_line in, t_xy min, t_xy max);

#endif
