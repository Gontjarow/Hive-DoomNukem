/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 19:13:53 by ngontjar          #+#    #+#             */
/*   Updated: 2020/10/05 14:24:28 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_NUKEM_H
# define DOOM_NUKEM_H

# include "libft.h"
# include "objects.h"
# include "editor.h"
# include "minimap.h"
# include "renderer.h"

# include "SDL.h"
# include "SDL_image.h"
# include "SDL_mixer.h"

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <time.h>
# include <math.h>

# define WIN_WIDTH 1024
# define WIN_HEIGHT 512
# define EDT_WIN_WIDTH 1024
# define EDT_WIN_HEIGHT 512
# define GAME_WIN_WIDTH 1024
# define GAME_WIN_HEIGHT 512
# define GAME_MIDWIDTH (GAME_WIN_WIDTH / 2)
# define GAME_MIDHEIGHT (GAME_WIN_HEIGHT / 2)

# define FPS 60
# define TICKS_PER_FRAME 1000.0 / FPS

# define EVT_KEY_DN 2
# define EVT_KEY_UP 3
# define EVT_MOUSE_DN 4
# define EVT_MOUSE_UP 5
# define EVT_MOUSE_MV 6
# define EVT_CLOSE_WIN 17

# define GAME_KEY_W 26
# define GAME_KEY_A 4
# define GAME_KEY_S 22
# define GAME_KEY_D 7
# define GAME_KEY_LSHIFT 225
# define GAME_KEY_SPACEBAR 44
# define GAME_KEY_E_ACTION 8
# define GAME_KEY_ESC 41

# define WAV_PLOP "wav/plop.wav"
# define WAV_STEAM0 "wav/steam/0.wav"
# define WAV_SWORD "wav/sword.wav"
# define WAV_THUNDER "wav/thunder.wav"
# define IMG_THUNDER0 "img/thunder/0.png"

# define DEBUG 1

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
	struct SDL_Surface  *poly_map;
	struct s_player		player;
	int 				current_portal;
	struct s_enemy		*enemies;
	struct s_wall		*walls;
	struct s_room		*rooms;
	struct s_wall		*portals;
	struct s_enemy		*enemy_first;
	struct s_wall		*wall_first;
	struct s_room		*room_first;
	struct s_wall		*portal_first;
	int 				wall_count;
	int 				room_count;
	int 				portal_count;
	int 				enemy_count;
    uint32_t            conversion_colors[512];
} 						t_model;

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
	int 				map_supplied;
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
	int					minimap_quit;
	int 				menu_out_of_focus;
	int 				map_data_initialized;
	const Uint8* 		keystates;
	uint32_t 			app_start;
	uint32_t 			frame_start;
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	union SDL_Event		event;
	struct s_mapfile	*map;
	struct s_sounds		*sounds;
	struct s_menu		*menu;
	struct s_minimap    *minimap;
	struct s_editor		*edt;
	struct s_model		*mdl;
	struct s_game		*game;
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

void			load_sounds(t_doom *doom);
void			destroy_sounds(t_doom *doom);

void 			init_menu(t_doom *doom);
void 			destroy_menu(t_doom *doom);
void			render_animation(t_doom *doom);
void			load_animation(t_menu *menu);
void			window_and_menu_events(t_doom *doom);
void			main_menu_loop(t_doom *doom, int argc, char **argv);

void 			init_game(t_doom *doom, int argc, char **argv);
void 			destroy_game(t_doom *doom);
void			game_render(t_doom *doom);
void	 		game_loop(t_doom *doom);
void			game_mouse_motion(t_doom *doom);
void 			game_mouse_down(t_doom *doom);
void			game_key_down(t_doom *doom);

void			modify_line_length(int len_mod, t_point *start, t_point *end, t_point *new_end);
void 			render_line(t_line *l);

void			flood_window(SDL_Surface *buff, uint32_t color);
void 			set_pixel(SDL_Surface *buff, int x, int y, uint32_t color);

void			add_wall_to_string(t_editor *edt, t_wall *wall);
void			expand_wall_string(t_editor *edt);
void			add_room_to_string(t_editor *edt, t_room *room);
void 			expand_room_string(t_editor *edt);
void			add_portal_to_string(t_editor *edt, t_wall *portal);
void			expand_portal_string(t_editor *edt);
void			expand_enemy_string(t_editor *edt);
void			update_player_string(t_editor *edt);
int				write_mapfile(t_editor *le);
int				overwrite_mapfile(t_editor *edt);

void	 		destroy_mapdata(t_doom *doom);
int 			load_model(t_doom *doom);
void	 		destroy_model(t_doom *doom);
#endif
