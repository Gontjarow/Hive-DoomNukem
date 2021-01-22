/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom-nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 19:13:53 by ngontjar          #+#    #+#             */
/*   Updated: 2021/01/19 20:27:43 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_NUKEM_H
# define DOOM_NUKEM_H

# include "SDL.h"
# include "SDL_image.h"
# include "SDL_mixer.h"

# include "libft.h"
# include "objects.h"
# include "data_model.h"
# include "editor.h"
# include "game.h"
# include "minimap.h"
# include "renderer.h"
# include "wireframe.h"

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <time.h>
# include <math.h>
# include <pthread.h>

# define WIN_WIDTH 1024
# define WIN_HEIGHT 512

# define UINT_ERROR_CONSTANT 32202
# define EPS 1e-14

# define FPS 60
# define TICKS_PER_FRAME 1000.0 / FPS

# define DEBUG 1

typedef struct			s_doom
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
	struct s_sprites	*sprites;
	struct s_menu		*menu;
	struct s_minimap    *minimap;
	struct s_editor		*edt;
	struct s_model		*mdl;
	struct s_game		*game;
}						t_doom;

void			ft_die(const char *error_message);
void 			load_alphabet(t_menu *menu);
void			load_numbers(t_doom *doom);
void	 		destroy_alphabet(t_menu *menu);
void			destroy_numbers(t_doom *doom);
void 			print_alphabet(const char *str, t_doom *doom, int x, int y);

void			load_sounds(t_doom *doom);
void			destroy_sounds(t_doom *doom);

int 			load_sprites(t_doom *doom);
int 			destroy_sprites(t_doom *doom);

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
void 			game_mouse_updown(t_doom *doom);
void			game_key_down(t_doom *doom);

#endif
