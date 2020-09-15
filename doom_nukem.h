/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 19:13:53 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/11 14:01:44 by msuarez-         ###   ########.fr       */
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
# define LE_WIN_WIDTH 1024
# define LE_WIN_HEIGHT 512

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
** Main struct
** *
*/
typedef struct 			s_point
{
	int 				x;
	int 				y;
}						t_point;

typedef struct 			s_wall
{
	struct s_point		start;
	struct s_point		end;
	int 				id;
	struct s_wall		*next;
}						t_wall;

typedef struct 			s_le
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct s_wall		*walls;
	struct s_wall		*wall_begin;
	int 				wall_count;
	int 				is_wall_start;
}						t_le;

typedef struct	s_doom
{
	int 				quit;
	int 				le_quit;
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	union SDL_Event		event;
	struct Mix_Chunk 	*mcThunder;
	struct Mix_Chunk 	*mcSteam;
	struct Mix_Chunk	*mcPlop;
	struct Mix_Chunk	*mcSword;
	struct SDL_Surface	*alphabet[128];
	int 				alphabet_scale;
	struct s_animation	ani_thunder;
	int 				selected_le;
	int 				esc_lock;
	struct s_le			*le;
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

void 			load_alphabet(t_doom *doom);
void 			print_alphabet(const char *str, t_doom *doom, int x, int y);

void			render_animation(t_doom *doom);
void			load_animation(t_doom *doom);

void			le_mouse_motion(t_doom *doom);
void 			le_mouse_down(t_doom *doom);
void			le_render(t_doom *doom);

void 			render_line(t_line *l);
void 			set_pixel(SDL_Surface *buff, int x, int y, uint32_t color);
#endif
