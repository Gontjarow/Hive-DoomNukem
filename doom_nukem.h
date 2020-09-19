/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 19:13:53 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/19 06:27:26 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_NUKEM_H
# define DOOM_NUKEM_H

# include "libft/libft.h"
# include "mlx/mlx.h"

# ifndef __linux__

# include <SDL2/SDL.h>
# include <SDL2/SDL_mixer.h>
# include <SDL2/SDL_image.h>

# else

# include "libsdl/SDL.h"
# include "libsdl/SDL_image.h"
# include "libsdl/SDL_mixer.h"

# endif

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <time.h>

# define WIN_WIDTH 1024
# define WIN_HEIGHT 512
# define EDT_WIN_WIDTH 1024
# define EDT_WIN_HEIGHT 512
# define GAME_WIN_WIDTH 1024
# define GAME_WIN_HEIGHT 512
# define GAME_MID_X ((GAME_WIN_WIDTH) / 2)
# define GAME_MID_Y ((GAME_WIN_HEIGHT) / 2)

# define FPS 60
# define TICKS_PER_FRAME 1000.0 / FPS

# define EVT_KEY_DN 2
# define EVT_KEY_UP 3
# define EVT_MOUSE_DN 4
# define EVT_MOUSE_UP 5
# define EVT_MOUSE_MV 6
# define EVT_CLOSE_WIN 17

# define WAV_PLOP "wav/plop.wav"
# define WAV_STEAM0 "wav/steam/0.wav"
# define WAV_SWORD "wav/sword.wav"
# define WAV_THUNDER "wav/thunder.wav"
# define IMG_THUNDER0 "img/thunder/0.png"

# define RAD_TO_DEG 57.29577951308232087679815481410517033
# define DEG_TO_RAD 0.01745329251994329576923690768488612

/*
** *
** Basic structs
** *
*/

typedef struct	s_animation
{
	SDL_Surface **surfaces;
	int 		frames;
	int 		current;
}				t_animation;

typedef struct	s_image
{
	void	*ptr;
	int		*data;
	int		width;
	int		height;
	int		bpp;
	int		line;
	int		endian;
}				t_image;

typedef struct	s_xyz
{
	double x;
	double y;
	double z;
}				t_xyz;

typedef t_xyz	t_euler; // Euler angles (X, Y, Z)
typedef double	t_deg; // Degrees
typedef double	t_rad; // Radians

typedef struct	s_matrix
{
	double	m[4][4];
}				t_matrix;

/*
** *
** Bit and bob structs to construct the game data for the Model etc.
** *
*/


typedef struct 			s_point
{
	int 				x;
	int 				y;
}						t_point;

typedef struct 			s_health
{
	int 				max;
	int 				cur;
}						t_health;

typedef struct 			s_weapon
{
	int 				dmg;
	int 				ammo_cur;
	int 				ammo_max;
	int 				cooldown;
	int 				reload_time;
	struct Mix_Chunk 	*fire_sound;
	struct Mix_Chunk	*reload_sound;
}						t_weapon;

typedef struct 			s_player
{
	t_xyz				pos; // Player position
	t_euler				angle; // Player rotation (NOT normalized)
	int					x;
	int					y;
	int					rot;
	struct s_health		hp;
	struct s_weapon		wep;
}						t_player;

typedef struct 			s_enemy
{
	int 				id;
	int 				x;
	int 				y;
	int 				rot;
	struct s_point		tail;
	struct s_health		hp;
	struct s_weapon		wep;
	struct s_enemy		*next;
}						t_enemy;

typedef struct 			s_wall
{
	struct s_point		start;
	struct s_point		end;
	int 				id;
	struct s_wall		*next;
}						t_wall;

/*
** *
** Model struct (>Model<-View-Controller) - Game (Controller) modifies this with the input, logic code
 * and needs and utilizes this data during gameplay. So does Renderer (View) to create the graphic
 * representation of the game.
** *
*/

typedef struct			s_model
{
	struct s_doom		*parent;
	struct s_player		player;
	int 				current_portal;
	struct s_enemy		*enemies;
	struct s_wall		*walls;
	struct s_wall		*portals;
	struct s_enemy		*enemy_first;
	struct s_wall		*wall_first;
	struct s_wall		*portal_first;
	int 				wall_count;
	int 				portal_count;
	int 				enemy_count;
} 						t_model;


/*
** *
** Level Editor struct
** *
*/

typedef struct 			s_editor
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct s_wall		*walls;
	struct s_wall		*wall_begin;
	struct s_wall		*portals;
	struct s_wall		*portal_begin;
	int 				wall_count;
	int 				portal_count;
	int 				enemy_count;
	struct s_player		player;
	struct s_enemy		*enemies;
	struct s_enemy		*enemy_first;
	struct s_point		tail;
	struct s_point		last_enemy;
	int 				is_wall_start;
	char 				*map_string;
	char 				*portal_string;
	char 				*join_string;
	int 				polygon_start_x;
	int 				polygon_start_y;
	int 				polygon_binding;
	int 				portalization_binding;
	int 				portalization_ending;
	int					portal_x;
	int 				portal_y;
	int					new_portal_x;
	int					new_portal_y;
	int 				player_set;
	int 				enemy_set;
	struct s_wall		*portalization_a;
	struct s_wall		*portalization_b;
	struct s_wall		*new_portal;
	int 				write_maps;
	char 				*map_path;
	struct s_doom		*parent;
}						t_editor;

/*
** *
** Sound Effects struct
** *
*/

typedef struct 			s_sounds
{
	struct Mix_Chunk 	*mcThunder;
	struct Mix_Chunk 	*mcSteam;
	struct Mix_Chunk	*mcPlop;
	struct Mix_Chunk	*mcSword;
}						t_sounds;

/*
** *
** Main Menu struct
** *
*/

typedef struct			s_menu
{
	int 				selected;
	struct s_animation	ani_thunder;
	int 				esc_lock;
	struct SDL_Surface	*alphabet[128];
	int 				alphabet_scale;
	SDL_Surface			*thunder;
	struct s_doom		*parent;
}						t_menu;

/*
** *
** Game struct
** *
*/

typedef struct			s_game
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct s_model		*mdl;
	struct s_doom		*parent;
	char 				*map_path;
}						t_game;

/*
** *
** Main Parent Father God struct
** *
*/

typedef struct	s_doom
{
	int 				quit;
	int 				edt_quit;
	int 				game_quit;
	int 				menu_out_of_focus;
	const Uint8* 		keystates;
	uint32_t 			app_start;
	uint32_t 			frame_start;
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	union SDL_Event		event;
	struct s_sounds		*sounds;
	struct s_menu		*menu;
	struct s_editor		*edt;
	struct s_model		*mdl;
	struct s_game		*game;
	struct s_player		*player;
}						t_doom;

typedef struct 			s_line
{
	int					x1;
	int 				x2;
	int 				y1;
	int 				y2;
	int 				fx;
	int 				fy;
	int 				px;
	int 				py;
	uint32_t 			color;
	struct SDL_Surface 	*buff;
	struct s_doom		*doom;
}						t_line;

/*
** *
** Keys
** *
*/

# ifndef __linux__

enum			e_mouse
{
	LMB = 1,
	RMB = 2,
	MMB = 3,
	WhDn = 4,
	WhUp = 5
};
# else

enum			e_mouse
{
	LMB = 1,
	RMB = 3,
	MMB = 2,
	WhDn = 5,
	WhUp = 4
};
# endif

# ifndef __linux__

enum			e_key
{
	esc = 53,
	num_plus = 69,
	num_minus = 78,
	left = 123,
	right = 124,
	down = 125,
	up = 126,
	q = 12,
	e = 14
};
# else

enum			e_key
{
	esc = 65307,
	num_plus = 65451,
	num_minus = 65453,
	left = 65361,
	right = 65363,
	down = 65364,
	up = 65362,
	q = 113,
	e = 101
};
# endif

/*
** *
** Functions
** *
*/

int				window_close(t_doom *p);
int				keyboard_press(int key, t_doom *p);
int				keyboard_release(int key, t_doom *p);
int				movement(t_doom *p);
int				mouse_key(int key, int x, int y, t_doom *p);
int				mouse_move(int x, int y, t_doom *p);

void			ft_die(const char *error_message);

t_image			ft_new_image(int *mlx, int width, int height);
t_image			ft_new_xpm_image(int *mlx, char *filename);

Uint32			get_exact_pixel(SDL_Surface *surface, int x, int y);
SDL_Surface		*load_png(char *path);
SDL_Surface		*load_texture(t_doom *doom, char *path);

void 			load_alphabet(t_menu *menu);
void	 		destroy_alphabet(t_menu *menu);
void 			print_alphabet(const char *str, t_doom *doom, int x, int y);

void 			init_menu(t_doom *doom);
void 			destroy_menu(t_doom *doom);
void			render_animation(t_doom *doom);
void			load_animation(t_menu *menu);
void			main_menu_loop(t_doom *doom, int argc, char **argv);

void 			init_game(t_doom *doom, int argc, char **argv);
void 			destroy_game(t_doom *doom);
void			game_render(t_doom *doom);
void	 		game_loop(t_doom *doom);
void			game_mouse_motion(t_doom *doom);
void 			game_mouse_down(t_doom *doom);

void 			init_edt(t_doom *doom, int argc, char **argv);
void	 		destroy_edt(t_doom *doom);

void			edt_mouse_motion(t_doom *doom);
void 			edt_mouse_down(t_doom *doom);
void			edt_render(t_doom *doom);

void			modify_line_length(int len_mod, t_point *start, t_point *end, t_point *new_end);
void 			render_line(t_line *l);
void 			set_pixel(SDL_Surface *buff, int x, int y, uint32_t color);
void			ft_draw(unsigned int *pixel, t_xyz start, t_xyz end, int color);
int				write_mapfile(t_editor *le);

// Vector 3 stuff

t_xyz			vec3_zero();
t_xyz			vec3_add(t_xyz a, t_xyz b);
t_xyz			vec3_sub(t_xyz a, t_xyz b);
t_xyz			vec3_mul(t_xyz v, double scalar);
t_xyz			vec3_div(t_xyz v, double scalar);
double			vec3_mag(t_xyz v);
t_xyz			vec3_norm(t_xyz v);
double			vec3_dot(t_xyz a, t_xyz b);
t_xyz			vec3_cross(t_xyz a, t_xyz b);
double			vec3_dist(t_xyz a, t_xyz b);

// Matrix stuff

t_matrix		rotate_z(t_rad angle);
t_matrix		rotate_y(t_rad angle);
t_matrix		rotate_x(t_rad angle);
t_matrix		translation(double x, double y, double z);
t_matrix		scale(double x, double y, double z);
t_matrix		perspective(t_deg fov, double near, double far);
t_xyz			vec3_transform(t_xyz v, t_matrix m);
t_matrix		multiply_m(t_matrix a, t_matrix b);

#endif
