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

# include "doom-nukem.h"

typedef struct 			s_doom t_doom;
typedef struct			s_status t_status;
typedef struct          s_model t_model;
typedef struct          s_state t_state;
typedef struct          s_enemy t_enemy;
typedef struct 			s_wall t_wall;
typedef struct			s_mapfile t_mapfile;

typedef void            (*gui_event)(t_state *state);
typedef void 			(*gui_click)(int x, int y);
typedef void 			(*gui_motion)(int x, int y);
typedef void 			(*status_action)(t_status *status);
typedef void 			(*logic_xy)(int x, int y);
typedef void 			(*logic_void)(void);
typedef uint32_t 		(*logic_colors)(int type);

# define EDT_WIN_WIDTH			1600
# define EDT_WIN_HEIGHT			900
# define COLOR_LINE				0xffffffff
# define COLOR_PLAYER			0xff00ff00
# define COLOR_ENEMY			0xffffff00
# define COLOR_HEALTH_PICKUP	0xff00ff00
# define COLOR_AMMO_PICKUP		0xffffff00
# define COLOR_WEAPON_PICKUP	0xffffffff
# define PICKUP_RADIUS			16

typedef struct 			s_2d_layer
{
    struct SDL_Surface	*buff;
    int                 rendering_on;
}						t_2d_layer;

typedef struct 			s_linedraw
{
    int                 origin_id;
    int                 drawing_underway;
    int                 draw_from_x;
    int                 draw_from_y;
    int                 draw_to_x;
    int                 draw_to_y;
}						t_linedraw;

enum 					e_logic_type {PLAYER, ENEMY};

typedef struct 			s_logic
{
	int 				plant_type;
	logic_void			swap_type;
	logic_xy			plant;
	int 				planted_ticks;
	t_point				sweep[2];
	int 				sweep_counter;
	logic_colors		colors;
}						t_logic;

typedef struct 			s_status
{
	struct s_gui		*gui;
	int					phase;
	int					phase_count;
	status_action		*phases;
	status_action		reset;
	int 				click_x;
	int 				click_y;
	int                 motion_x;
	int                 motion_y;
	void                *data;
}						t_status;

typedef struct 			s_gui // Mode_Polydraw() .... infinite amount of Mode_Somethings() which all define their own
{							  // interactions etc. for example Mode_Placement()
    gui_event			activate;
    gui_event			deactivate;
    gui_event			change_zoom;
	gui_click  			left_click; // Function pointer to polydraw_leftclick
	gui_click			middle_click;
	gui_click			right_click;
	gui_motion			motion;
    int 				has_motion;
}						t_gui;

typedef struct 			s_state
{
	struct s_gui		*gui;
    int                 thread_permission;
    int                 thread_x;
    int                 thread_y;
    int                 thread_hit;
    int                 thread_target_id;
    uint32_t            thread_color;
    int                 job_running;
    int                 job_abort;
    int 				zoom_factor;
    int					scroll_x;
    int 				scroll_y;
    int 				confine_skip;
    int 				selected_weapon_type;
}						t_state;

typedef struct 			s_editor
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct s_doom		*parent;
	struct s_mapfile	*map;
	char 				*map_path;
	int 				map_supplied;
}						t_editor;

/*
 * from editor.c
 * */

void 					init_edt(t_doom *doom, int argc, char **argv);
void	 				destroy_edt(t_doom *doom);
void					edt_render(t_doom *doom);

/*
 * from editor_input.c
 * */

void					edt_mouse_motion(t_doom *doom);
void					edt_mouse_down(t_doom *doom);
void					edt_keystate_input(t_doom *doom);

/*
 * from draw_editor.c
 * */

void					redraw_editor_to_backbuffer(uint32_t color);
SDL_Surface				*zoom_xpm(int factor);
SDL_Surface				*mode_xpm(t_gui *mode);
SDL_Surface 			*mousehelp_xpm(void);
SDL_Surface 			*keyhelp_xpm(void);
void                	print_mode_info(t_gui *mode);

/*
 * from scrolling.c
 * */

void					draw_scroll_bars_to_backbuffer(t_state *state);
void					accelerate_scroll(t_state *state, SDL_Scancode direction);
int						confine_scroll(t_state *state);
void	 				handle_keyboard_scrolling(t_doom *doom);

/*
 * from walls.c
 * */

t_wall					*wall_by_id(int id);
void					wall_to_buffer(t_wall *wall, SDL_Surface *buff, uint32_t color);
void					x_walls_to_buffer(int x, t_wall *wall, SDL_Surface *buff, uint32_t color);
void					relink_model_walls(t_wall *relinking_wall);

/*
 * from editor_buffers.c
 * */

void            		wipe_editor_back_buffer(uint32_t color);
void            		wipe_editor_front_buffer(uint32_t color);
t_2d_layer      		*editor_back_buffer(void);
t_2d_layer      		*editor_front_buffer(void);
SDL_Surface     		*mixing_surface();

/*
 * from square_to_buffer.c
 * */

void					preserving_square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t mask);
void					unpreserving_square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void				 	preserving_cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void 					unpreserving_cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					digit_to_buffer(SDL_Surface *buff, t_point xy, int digit, uint32_t color);
void					digit_to_buffer_ptr(SDL_Surface *buff, t_point xy, int digit, uint32_t color, void (*render_fun)(t_line *line));

/*
 * from circle_to_buffer.c
 * */

void					preserving_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t mask);
void					unpreserving_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					unmasked_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color, uint32_t mask);
void					masked_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color, uint32_t avoid);
void            		circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);

/*
 * from linedraw.c
 * */

void					init_linedraw_data(void *data_ptr);
void					careful_linedraw_to_buffer(t_linedraw *data, SDL_Surface *buff, uint32_t color, uint32_t *avoid);
void					linedraw_to_buffer(t_linedraw *data, SDL_Surface *buff, uint32_t color);
t_wall					*linedraw_to_wall(t_linedraw *data);

/*
 * from magnets.c
 * */

void            		check_any_magnet_hits(int x, int y, t_model *mdl, t_state *state);
void            		magnet_test(void* argv);

/*
 * from mode_editor.c
 * */

t_gui					*mode_pickups(void);
t_gui					*mode_planting(void);
t_gui					*mode_polydraw(void);

/*
 * from state_editor.c
 * */

void 					trigger_protection(int clear);
void		 			edt_cycle_mode(t_state *state);
void					edt_outward_zoom(void);
void					edt_inward_zoom(void);
t_state					*get_state(void);

/*
 * from pickups.c
 * */

t_logic		 			*pickups_logic(void);
void 					pickups_swap_type(void);
void					pickups_refresh_preview(void);
void					pickups_plant_health(int x, int y);
void 					pickups_plant(int x, int y);

void					pickups_change_zoom(t_state *state);
void					pickups_activate(t_state *state);
void					pickups_deactivate(t_state *state);

void 					pickups_mouse_motion(int x, int y);
void 					pickups_left_click(int x, int y);
void 					pickups_right_click(int x, int y);
void 					pickups_middle_click(int x, int y);

/*
 * from positions.c
 * */

t_point					relative_position(int x, int y, t_state *state);
t_point					scrolled_position(int x, int y, t_state *state);
uint32_t				type_colors(int type);
uint32_t				pickup_color(int flavor);
int						valid_line(t_line *line);

/*
 * from draw_entities.c
 * */

void					draw_player(t_model *mdl, t_state *state);
void					draw_enemy(t_enemy *enemy, t_state *state);
void	 				draw_pickup(t_pickup *pickup, t_state *state);
void					update_tail_to_buffer(SDL_Surface *buff, void *obj_ptr, int obj_type);
void 					draw_plantings_to_backbuffer(t_model *mdl, t_state *state);

/*
 * from planting_logic, planting.c, planting_input.c
 * */

t_logic 				*planting_logic(void);
void 					planting_plant(int x, int y);
void 					planting_swap_type(void);

void					planting_activate(t_state *state);
void					planting_deactivate(t_state *state);
void					planting_change_zoom(t_state *state);

void 					planting_mouse_motion(int x, int y);
void 					planting_left_click(int x, int y);
void 					planting_right_click(int x, int y);
void 					planting_middle_click(int x, int y);

/*
 * from polydraw_logic.c, polydraw.c, polydraw_input.c
 * */

t_status				*polydraw_status(void);
void 					polydraw_start(t_status *status);
void 					polydraw_continue(t_status *status);
void 					polydraw_end(t_status *status);
void					polydraw_reset(t_status *status);

void					polydraw_activate(t_state *state);
void					polydraw_deactivate(t_state *state);
void 					polydraw_change_zoom(t_state *state);

void 					polydraw_mouse_motion(int x, int y);
void 					polydraw_left_click(int x, int y);
void 					polydraw_right_click(int x, int y);
void 					polydraw_middle_click(int x, int y);

/*
 * from record.c
 * */

void					show_editor_polymap(SDL_Surface *polymap, uint32_t *colors);
void					record_player(t_point location, t_point *tail, t_model *mdl);
t_enemy					*record_enemy(t_point location, t_point *tail, t_model *mdl);
void					create_strings_from_model(t_model *mdl, t_mapfile *map);

/*
 * from polymap.c
 * */

uint32_t				*get_debug_convs(void);
uint32_t				*get_conv_colors(void);
void        			add_room_polymap(t_room *room, SDL_Surface *polymap, uint32_t *conv_colors);

#endif
