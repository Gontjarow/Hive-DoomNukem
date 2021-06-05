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
# define COLOR_PORTAL			0xff00ffff
# define COLOR_SELECTION_LINE	0xffffff00
# define COLOR_SELECTION_WALL	0xff0000ff
# define COLOR_SELECTION_VECTOR 0xffff0000
# define COLOR_GRID_LINE		0xff888888
# define COLOR_PLAYER			0xff00ff00
# define COLOR_ENEMY			0xffffff00
# define COLOR_HEALTH_PICKUP	0xff00ff00
# define COLOR_AMMO_PICKUP		0xffffff00
# define COLOR_WEAPON_PICKUP	0xffffffff
# define COLOR_EFFECT_EXIT		0xff00ff00
# define COLOR_EFFECT_POSTER	0xffffff00
# define COLOR_EFFECT_KEY		0xffffff00
# define COLOR_EFFECT_LIGHT		0xffffffff
# define EDT_TRIANGLE_SIZE		16
# define PICKUP_RADIUS			16
# define HEIGHT_STEPPING		10
# define FLOOR_MIN				0
# define FLOOR_DEFAULT			100
# define FLOOR_MAX				750
# define NEXT_WALL              1
# define PREV_WALL              -1
# define ROOF_MIN				0
# define ROOF_DEFAULT			200
# define ROOF_MAX				800
# define FLOOR_ROOF_DIFF_LIMIT	50
# define GRID_START_SIZE		32
# define DOUBLE_CLICK_COOLDOWN	24
# define STRING_ENTER_MAPFILE	"input mapfile name with a..z and ."
# define STRING_VALID_CHAR_INFO	"save and confirm with enter"
# define STRING_CONFIRM_SAVING	"do you want to save changes to map"
# define WARNING_NON_CONVEX		"WARNING: NON-CONVEX polygon detected!"
# define WARNING_NON_CONVEX_CW	"WARNING: NON-CONVEX clockwise polygon detected!"
# define WARNING_NON_CONVEX_CCW	"WARNING: NON-CONVEX COUNTER-CLOCKWISE polygon detected!"
# define WARNING_NON_CONVEX_Q	"WARNING: NON-CONVEX polygon detected! Could be either clockwise or counter-clockwise?"
# define WARNING_CCW			"WARNING: COUNTER-CLOCKWISE detected!"
# define WARNING_NON_CLOSED		"WARNING: Rooms walls did not form a CLOSED room!"
# define XPM_SINGLY				"img/edt/single_zoom.xpm"
# define XPM_DOUBLY				"img/edt/double_zoom.xpm"
# define XPM_TRIPLY				"img/edt/triple_zoom.xpm"
# define XPM_QUADLY				"img/edt/quad_zoom.xpm"
# define XPM_POLYDRAW			"img/edt/polydraw.xpm"
# define XPM_SELECT				"img/edt/select.xpm"
# define XPM_EFFECT				"img/edt/effect.xpm"
# define XPM_PLANTING			"img/edt/planting.xpm"
# define XPM_PICKUPS			"img/edt/pickups.xpm"

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
    int 				portalizing;
    int 				portalizing_to_a;
	int 				portalizing_to_b;
    int 				portal_option_a;
    int 				portal_option_b;
    t_point				portal_a_loc;
    t_point				portal_b_loc;
}						t_linedraw;

enum 					e_logic_type { PLAYER, ENEMY };
enum 					e_logic_plant_dirs { UPWARD, DOWNWARD };

typedef struct 			s_logic
{
	int 				plant_type;
	int					plant_dir;
	logic_void			swap_type;
	logic_xy			plant;
	int 				planted_ticks;
	t_point				sweep[2];
	int 				sweep_counter;
	logic_colors		colors;
    int                 last_plant_id;
}						t_logic;

typedef struct 			s_select
{
	int 				selected_room_id;
	int 				last_floor;
	int 				last_roof;
    int                 selected_wall_id;
    int                 adding_slope;
    t_wall              virtual_slope_wall;
}						t_select;

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

typedef struct 			s_gui
{
    gui_event			activate;
    gui_event			deactivate;
    gui_event			change_zoom;
	gui_click  			left_click;
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
    int 				selected_ai_type;
    int 				selected_sprite_id;
    int 				grid_on;
    int 				grid_size;
    int 				cooldown;
    int 				saving_choice;
    int                 give_ceiling_to_rooms;
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
void					edt_gridify(void);
void					edt_render(t_doom *doom);

/*
 * from editor_input.c
 * */

void					edt_mouse_motion(t_doom *doom);
void					edt_mouse_down(t_doom *doom);
void					edt_keystate_input(t_doom *doom);

/*
 * from editor_take_input.c
 * */

void					draw_confirmation(char *input, t_doom *doom);
void					draw_input(t_doom *doom);
void					decide_table(int x, char *arr, int *i, t_doom *doom);
void					keystate_input(char *arr, int *i, t_doom *doom);

/*
 * from draw_editor.c
 * */

void					redraw_editor_to_backbuffer(uint32_t color);
void                	print_mode_info(t_gui *mode);

/*
 * from editor_xpm.c
 * */

SDL_Surface				*zoom_xpm(int zoom_factor);
SDL_Surface				*mode_xpm(t_gui *mode);
SDL_Surface 			*mousehelp_xpm(void);
SDL_Surface 			*keyhelp_xpm(void);

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

void					wall_to_buffer(t_wall *wall, SDL_Surface *buff, uint32_t color);
void 					room_walls_to_buffer(t_room *room, SDL_Surface *buff, uint32_t color);
void					x_walls_to_buffer(int x, t_wall *wall, SDL_Surface *buff, uint32_t color);

/*
 * from editor_buffers.c
 * */

void            		wipe_editor_back_buffer(uint32_t color);
void            		wipe_editor_front_buffer(uint32_t color);
t_2d_layer      		*editor_back_buffer(void);
t_2d_layer      		*editor_front_buffer(void);
SDL_Surface     		*mixing_surface();

/*
 * from square_to_buffer.c, circle_to_buffer.c, triangle_to_buffer.c
 * */

void					preserving_square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t mask);
void					unpreserving_square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void				 	preserving_cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void 					unpreserving_cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					cross_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					number_to_buffer(SDL_Surface *buff, t_point loc, int number, uint32_t color);
void					digit_to_buffer(SDL_Surface *buff, t_point xy, int digit, uint32_t color);
void					digit_to_buffer_ptr(SDL_Surface *buff, t_point xy, int digit, uint32_t color, void (*render_fun)(t_line *line));

void					preserving_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t mask);
void					unpreserving_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);
void					unmasked_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color, uint32_t mask);
void					masked_circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color, uint32_t *avoid);
void            		circle_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color);

void					unpreserving_triangle_to_buffer(SDL_Surface *buff, t_point xy, int dir, uint32_t color);
void					preserving_triangle_to_buffer(SDL_Surface *buff, t_point xy, int dir, uint32_t color);

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
t_point			 		detect_wall_point(int x, int y, t_model *mdl, t_state *state);
void            		check_any_magnet_hits(int x, int y, t_model *mdl, t_state *state);
void            		magnet_test(void* argv);

/*
 * from mode_editor.c
 * */

t_gui					*mode_select(void);
t_gui					*mode_effect(void);
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
 * from select.c
 * */

t_select 				*select_logic(void);
void		 			select_roof(int dir);
void 					select_floor(int dir);
void		 			select_delete_room(void);
void					select_activate(t_state *state);
void					select_deactivate(t_state *state);
void					select_change_zoom(t_state *state);
void		 			select_mouse_motion(int x, int y);
void 					select_left_click(int x, int y);
void 					select_right_click(int x, int y);
void 					select_middle_click(int x, int y);

/*
 * from effect.c
 * */

int						effect_dirs(int type);
uint32_t				effect_colors(int type);
void 					effect_swap_type(void);
void					effect_plant(int x, int y);
t_logic 				*effect_logic(void);
void			        effect_adjust(int x_or_y, int change);
void					effect_activate(t_state *state);
void					effect_deactivate(t_state *state);
void					effect_change_zoom(t_state *state);
void 					effect_mouse_motion(int x, int y);
void 					effect_left_click(int x, int y);
void 					effect_right_click(int x, int y);
void 					effect_middle_click(int x, int y);

/*
 * from positions.c
 * */

t_point					relative_position(int x, int y, t_state *state);
t_point                 scroll_adjusted(t_point point);
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
void		 			draw_selection_to_backbuffer(t_state *state);
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
void		 			record_portal(t_model *mdl, t_wall *wall);
int						record_room(t_model *mdl, t_wall *room_first_wall, int prev_rooms_wall_count);
void					create_strings_from_model(t_model *mdl, t_mapfile *map);

/*
 * from polymap.c
 * */

int						room_id_from_polymap(SDL_Surface *polymap, int x, int y);
uint32_t				*get_debug_convs(void);
uint32_t				*get_conv_colors(void);
void        			add_room_polymap(t_room *room, SDL_Surface *polymap, uint32_t *conv_colors);
void 					repaint_polymap(t_model *mdl);

/*
 * from find_visual.c
 * */

void					find_visual_xy(t_room *room);

/*
 * from delete_room.c
 * */

void					delete_room(t_room *room, int del_count, t_model *mdl);

enum					e_clockwise_return_code { NEEDS_FLIPPING = 2 };

/*
 * from clockwiseness.c
 * */

void		 			flip_room(t_room *room, t_model *mdl);
int						is_clockwise_convex_polygon(t_room *room, t_wall *wall, int wc);

/*
 * from delete_portal.c
 * */

void					delete_portals_by_room(t_room *room, t_model *mdl);
void					delete_portal(t_wall *portal, t_model *mdl);

/*
 * from editor_handle_save.c
 * */

char					*ask_to_save(t_doom *doom);
int 					edt_handle_confirm_save(t_doom *doom);

/*
 * from room.c
 * */

void					flip_room(t_room *room, t_model *mdl);
t_room					*room_by_wall_id(int id, t_model *mdl);

/*
 * from portal.c
 * */

int						portal_belongs_to_room(t_wall *portal, t_room *room);

#endif
