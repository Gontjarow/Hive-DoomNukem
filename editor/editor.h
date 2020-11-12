/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 14:57:53 by krusthol          #+#    #+#             */
/*   Updated: 2020/11/06 14:59:44 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

#include "objects.h"
#include <stdint.h>

typedef struct 			s_doom t_doom;
typedef struct			s_status t_status;

typedef void 			(*gui_click)(int x, int y);
typedef void 			(*gui_motion)(int x, int y);
typedef void 			(*gui_action)(t_status *status);

typedef struct 			s_2d_layer
{
    struct SDL_Surface	*buff;
    int                 rendering_on;
}						t_2d_layer;

typedef struct 			s_linedraw
{
    int                 drawing_underway;
    int                 draw_from_x;
    int                 draw_from_y;
    int                 draw_to_x;
    int                 draw_to_y;
}						t_linedraw;

typedef struct 			s_status
{
	struct s_gui		*gui;
	int					phase;
	int					phase_count;
	gui_action			*phases;
	gui_action			reset;
	int 				click_x;
	int 				click_y;
	void                *data;
}						t_status;

typedef struct 			s_gui
{
	gui_click  			left_click;
	gui_click			middle_click;
	gui_click			right_click;
	int 				has_motion;
	gui_motion			motion;
}						t_gui;

typedef struct 			s_state
{
	struct s_gui		*gui;
}						t_state;

typedef struct 			s_editor
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct s_doom		*parent;
}						t_editor;

void 			init_edt(t_doom *doom);
void	 		destroy_edt(t_doom *doom);
void 			edt_mouse_motion(t_doom *doom);
void 			edt_mouse_down(t_doom *doom);
void			edt_render(t_doom *doom);

#if 0
/*
** *
** Level Editor struct
** *
*/

struct 					s_player;
struct 					s_point;
typedef struct 			SDL_Surface SDL_Surface;

typedef struct 			s_editor
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct SDL_Surface  *poly_map;
	struct SDL_Surface	*grid_buffer;
	struct SDL_Surface	*grid_temp;
	struct SDL_Surface  *back_buffer;
	struct SDL_Surface	*front_buffer;
	struct s_wall		*walls;
	struct s_wall		*wall_begin;
	struct s_room		*rooms;
	struct s_room		*room_first;
	struct s_wall		*portals;
	struct s_wall		*portal_begin;
	struct s_wall		*subselection_wall;
	int 				wall_count;
	int 				room_count;
	int 				portal_count;
	int 				enemy_count;
	struct s_player		player;
	struct s_enemy		*enemies;
	struct s_enemy		*enemy_first;
	struct s_point		tail;
	struct s_point		last_enemy;
	int 				is_wall_start;
	char 				*wall_string;
	char 				*portal_string;
	char 				*room_string;
	char 				*enemy_string;
	char 				*player_string;
	char 				*join_string;
	int 				hover_status;
	int 				hover_id;
	int 				selection_status;
	int 				selection_room_id;
	int					subselection_id;
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
	int 				load_map;
	int 				write_maps;
	int 				overwrite_map;
	char 				*map_path;
	struct s_doom		*parent;
	uint32_t            conversion_colors[512];
}						t_editor;

void			transfer_model_to_editor(t_doom *doom);
void			edt_mouse_motion(t_doom *doom);
void 			edt_mouse_down(t_doom *doom);
void			edt_render(t_doom *doom);
void            create_grid_buffer(t_editor *edt);

/* from walls.c */
int				wall_count_of_previous_rooms(t_editor *edt);
t_wall 			*wall_by_count(t_editor *edt, int count);
t_wall			*wall_by_id(t_editor *edt, int id);

/* from draw_editor.c */
void			print_portals(t_editor *edt);
void			print_characters(t_editor *edt);
void			print_walls(t_editor *edt);
void			circle_rooms(t_doom *doom);
void			circle_visual(SDL_Surface *buff, t_point *visual, uint32_t color);
void			circle_room(t_doom *doom, t_room *room);
void			circle_player(t_doom *doom);
void			circle_enemy(t_doom *doom);

/* from find_visual.c */
void			find_visual_xy(t_editor *edt, t_room *room);

/* from record.c */
void			create_strings_from_state(t_editor *edt);
void			record_room(t_editor *edt);
void			record_enemy(int x, int y, t_editor *edt);
void			record_player(int x, int y, t_editor *edt);
void 			record_portal(t_editor *edt);

/* from room_polygon_map.c */
int				room_id_from_pixel(SDL_Surface *buff, int x, int y);
void            create_room_polygon_map(t_editor *edt);
void	 		wipe_room_polygon_map(t_room *room, t_doom *doom);
void            expand_room_polygon_map(t_room *room, t_doom *doom, struct SDL_Surface *poly_map, uint32_t *conversion_colors);

#endif

#endif
