/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:17:53 by krusthol          #+#    #+#             */
/*   Updated: 2021/08/14 21:35:25 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

#include "doom-nukem.h"

# define CROUCH_HEIGHT 10
# define STAND_HEIGHT 20
# define KNEE_HEIGHT 11

enum	e_enemy_aggro { INACTIVE, ACTIVE };
enum	e_enemy_type { RANGED, MELEE, BOSS };
enum 	e_sprite_orient { FRONT, LEFT, RIGHT, BACK };
enum	e_sprite_state { DEATH, IDLE, MOVE, ATTACK, HURT };
enum    e_weapons { PISTOL, SHOTGUN, ASSAULT_RIFLE, JETPACK };
enum	e_enemy_points { FRONTLEFT, FRONTRIGHT, BACKLEFT, BACKRIGHT };

typedef struct			s_xy t_xy;
typedef struct 			s_doom t_doom;
typedef struct 			s_model t_model;

typedef struct	t_argb
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
}				t_argb;

typedef struct 			s_point
{
	int 				x;
	int 				y;
}						t_point;

typedef struct			s_coord
{
	double				x;
	double				y;
}						t_coord;

typedef struct 			s_tri_sides
{
	double				a;
	double 				b;
	double				c;
}						t_tri_sides;

typedef struct 			s_health
{
	int 				max;
	int 				cur;
}						t_health;

typedef struct 			s_weapon
{
	int 				type_id;
	int 				dmg;
	int 				ammo_cur;
	int					ammo_res;
	int 				ammo_max;
	int					do_own;
	double 				cooldown;
	double 				reload_time;
	struct SDL_Surface	*weap_img;
	struct Mix_Chunk 	*fire_sound;
	struct Mix_Chunk	*reload_sound;
}						t_weapon;

typedef struct 			s_player
{
	double				rot_horizontal;
	double				rot_vertical;
	double 				x;
	double 				y;
	double				z;
	double				z_velocity;
	double 				yaw;
	double				shoot_cd;
	double				reload_time;
	int					room_id;
	int 				rot;
	int					height;
	int					is_jumping;
	int					is_crouching;
	int					is_running;
	int					is_flying;
	int					has_fired;
	int					has_key;
	int					eff_pressed;
	int					mov_speed;
	int					max_speed;
	int					min_speed;
	int					run_lock;
	int					crouch_lock;
	int					rot_speed;
	int					weap_id;
	int					shooting;
	int					invis;
	struct s_room		*room;
	struct SDL_Surface	*active_health_bar;
	struct SDL_Surface	*active_fuel_bar;
	struct SDL_Surface	*hud_num[10];
	struct s_coord		bullet_pos;
	struct s_weapon		weap_arr[10];
	struct s_point		tail;
	struct s_health		hp;
	struct s_weapon		wep;
}						t_player;

typedef struct          s_ai
{
	int                 type_id;
	int                 min_dis;
	int                 max_dis;
	int                 aggro;
	int                 mov_speed;
	int                 dmg;
	struct Mix_Chunk	*mcEnemyDeath;
	struct Mix_Chunk 	*mcEnemyShoot;
}                       t_ai;

typedef struct			s_animation
{
	// SDL_Surface 		**surfaces;
	// int 				frames;
	// int 				current;
	int					dead;
	int					count;
	int					orient;
	int					done;
}						t_animation;

typedef struct			s_menu_anim
{
	SDL_Surface 		**surfaces;
	int 				frames;
	int 				current;
}						t_menu_anim;

typedef struct 			s_enemy
{
	int 				id;
	int 				x;
	int 				y;
	int 				rot;
	int					did_shoot;
	int					who_shot;
	double				shoot_cd;
	int					anim_phase;
	double				stun_time;
	double				stun_cd;
	uint32_t			ray_color;
	struct s_ai			ai;
	struct s_coord		bullet_pos;
	struct s_point		tail;
	struct s_health		hp;
	struct s_weapon		wep;
	struct s_enemy		*next;
	struct s_animation	anim;
	struct SDL_Surface	*active_sprite;
}						t_enemy;

#define MAX_EFFECT_COUNT 4
typedef struct 			s_wall
{
	struct s_point		start;
	struct s_point		end;
	int 				id;
	int					texture_id;
	SDL_Surface			*active_sprite;
	struct s_wall		*next;
	int					can_portal;
	struct s_effect		*effects[MAX_EFFECT_COUNT];
	int					effect_count;
	int					open;
	int					portal_type;
}						t_wall;

typedef struct 			s_room
{
	struct s_wall		*first_wall;
	int					lit;
	int 				first_wall_id;
	int 				id;
	int 				wall_count;
	int 				floor_height;
	int 				roof_height;
	int					has_ceiling;
	int					is_hallway;
	int					slope_floor;
	int					slope_roof;
	int					adjusting_opposite;
	t_point 			visual;
	struct s_room		*next;
}						t_room;

enum 	e_effect_types { EFFECT_EXIT, EFFECT_POSTER, EFFECT_KEYPANEL, EFFECT_LIGHTKNOB, EFFECT_TARGET, EFFECT_LEVER };

typedef struct			s_effect
{
	int					id;
	int 				type_id;
	int					activated;
	struct s_point		loc;
	struct s_point		target;
	int 				target_id;
	SDL_Surface			*active_sprite;
	struct s_effect		*next;
}						t_effect;

enum	e_pickup_flavors { PICKUP_HEALTH, PICKUP_AMMO, PICKUP_WEAPON, PICKUP_KEY };
# define PICKING_RADIUS 10

typedef struct 			s_pickup
{
	int 				id;
	struct s_point		loc;
	int 				flavor;
	int 				weapon_type_id;
	SDL_Surface			*active_sprite;
	struct s_pickup		*next;
}						t_pickup;

typedef struct 			s_sounds
{
	struct Mix_Chunk 	*mcThunder;
	struct Mix_Chunk 	*mcSteam;
	struct Mix_Chunk	*mcPlop;
	struct Mix_Chunk	*mcSword;
	struct Mix_Chunk	*mcPistolRld;
	struct Mix_Chunk	*mcAssaultRld;
	struct Mix_Chunk	*mcShotgunRld;
	struct Mix_Chunk	*mcShotgunShot;
	struct Mix_Chunk	*mcPistolShot;
	struct Mix_Chunk	*mcAssaultShot;
	struct Mix_Chunk	*mcWalking;
	struct Mix_Chunk	*mcRunning;
	struct Mix_Chunk	*mcCrouching;
	struct Mix_Chunk	*mcEnemyDeath;
	struct Mix_Chunk	*mcHealthPickup;
	struct Mix_Chunk	*mcAmmoPickup;
	struct Mix_Chunk	*mcWeaponPickup;
	struct Mix_Chunk	*mcBackground;
	struct Mix_Chunk	*mcBoss;
	struct Mix_Chunk	*mcLoading;
	struct Mix_Chunk	*mcJetpack;
	struct Mix_Chunk	*mcLaserShot;
	struct Mix_Chunk	*mcDogShot;
	struct Mix_Chunk	*mcDogDying;
	struct Mix_Chunk	*mcOutAmmo;
	int					is_flying;
	double				jetpack_delay;
	double				footstep_delay;
}						t_sounds;

typedef struct 			s_rsrc
{
	struct SDL_Surface	*texture;
	int					id;
	struct s_rsrc		*next;
}						t_rsrc;

typedef struct 			s_sprites
{
	// Wall and Floor Sprites
	struct SDL_Surface **txt_wall;
	struct SDL_Surface **txt_floor;
	struct SDL_Surface *txt_door;
	struct SDL_Surface *txt_transparent;

	// HUD Sprites
	struct SDL_Surface	**txt_health_bar;
	struct SDL_Surface	**txt_fuel_bar;
	struct SDL_Surface	*txt_rifle_bar;
	struct SDL_Surface	*txt_shotgun_bar;
	struct SDL_Surface	*txt_pistol_bar;
	struct SDL_Surface	*txt_jetpack_ammo_pickup;
	struct SDL_Surface	*txt_clip_bar;
	struct SDL_Surface	*txt_key_hud;

	// Pickup Sprites
	struct SDL_Surface	*txt_health_pickup;
	struct SDL_Surface	*txt_shotgun_ammo_pickup;
	struct SDL_Surface	*txt_assault_ammo_pickup;

	// Effector Sprites
	struct SDL_Surface	**txt_portal;
	struct SDL_Surface	*txt_switch_off;
	struct SDL_Surface	*txt_switch_on;
	struct SDL_Surface	*txt_light_off;
	struct SDL_Surface	*txt_light_on;
	struct SDL_Surface	*txt_poster_off;
	struct SDL_Surface	*txt_poster_on;
	struct SDL_Surface	*txt_panel_off;
	struct SDL_Surface	*txt_panel_on;
	struct SDL_Surface	*txt_lever_off;
	struct SDL_Surface	*txt_lever_on;
	struct SDL_Surface	*active_portal;
	int					portal_phase;

	// Loading Screen Sprites
	struct SDL_Surface	*txt_loading_none;
	struct SDL_Surface	*txt_loading_0;
	struct SDL_Surface	*txt_loading_1;
	struct SDL_Surface	*txt_loading_2;
	struct SDL_Surface	*txt_loading_won;
	struct SDL_Surfacce *txt_loading_lost;

	// Underlying data holders, invididually named for readability
	struct SDL_Surface	*txt_ranged_front_attack;
	struct SDL_Surface	*txt_ranged_front_idle;
	struct SDL_Surface	**txt_ranged_front_walk;
	struct SDL_Surface	*txt_ranged_left_attack;
	struct SDL_Surface	*txt_ranged_right_attack;
	struct SDL_Surface	*txt_ranged_left_idle;
	struct SDL_Surface	*txt_ranged_right_idle;
	struct SDL_Surface	**txt_ranged_left_walk;
	struct SDL_Surface	**txt_ranged_right_walk;
	struct SDL_Surface	**txt_ranged_back_walk;
	struct SDL_Surface	*txt_ranged_back_idle;
	struct SDL_Surface	**txt_ranged_death;

	struct SDL_Surface	**txt_melee_front_attack;
	struct SDL_Surface	**txt_melee_front_walk;
	struct SDL_Surface	**txt_melee_left_attack;
	struct SDL_Surface	**txt_melee_right_attack;
	struct SDL_Surface	**txt_melee_left_walk;
	struct SDL_Surface	**txt_melee_right_walk;
	struct SDL_Surface	*txt_melee_front_idle;
	struct SDL_Surface	*txt_melee_left_idle;
	struct SDL_Surface	*txt_melee_right_idle;
	struct SDL_Surface	**txt_melee_back_walk;
	struct SDL_Surface	*txt_melee_back_idle;
	struct SDL_Surface	**txt_melee_death;

	struct SDL_Surface	**txt_boss_front_attack;
	struct SDL_Surface	**txt_boss_front_walk;
	struct SDL_Surface	**txt_boss_left_attack;
	struct SDL_Surface	**txt_boss_right_attack;
	struct SDL_Surface	**txt_boss_left_walk;
	struct SDL_Surface	**txt_boss_right_walk;
	struct SDL_Surface	*txt_boss_front_idle;
	struct SDL_Surface	*txt_boss_left_idle;
	struct SDL_Surface	*txt_boss_right_idle;
	struct SDL_Surface	**txt_boss_back_walk;
	struct SDL_Surface	*txt_boss_back_idle;
	struct SDL_Surface	**txt_boss_death;
	struct SDL_Surface	*txt_boss_back_walk_atlas;
}						t_sprites;

typedef struct			s_menu
{
	int 				selected;
	int                 mousing_at;
	struct s_menu_anim	ani_thunder;
	int 				esc_lock;
	struct SDL_Surface	*alphabet[128];
	int 				alphabet_scale;
	int                 update_argc_argv;
	char                *added_arg;
	SDL_Surface			*thunder;
	struct s_doom		*parent;
}						t_menu;

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
	uint32_t 			avoid[4];
	struct SDL_Surface 	*buff;
	struct s_doom		*doom;
}						t_line;

typedef struct			s_image
{
	void				*ptr;
	int					*data;
	int					width;
	int					height;
	int					bpp;
	int					line;
	int					endian;
}						t_image;

# define 				TABLE_SIZE	28
enum 					e_alphabet_types { SCANCODES, CHARS };

/*
 * from alphabet.c and alphabet_table.c
 * */

void 					print_glyph_str(const char *str, SDL_Surface *buff, int x, int y);
void 					print_glyph_str_dymo(const char *str, SDL_Surface *buff, int x, int y);
int						propose_character(char c, char *buff, int *i, int table_index);
void					*get_alphabet_tables(int type);
int						table_index(char c);

/*
 * from debug_console.c
 * */

void 					debug_model_player(void);
void		 			debug_model_enemies(void);
void 					debug_model_walls(void);
void					debug_model_rooms(void);
void					debug_model_portals(void);
void					debug_model_effects(void);
void					debug_model_pickups(void);
void					debug_model_chain(void);


/*
 * from texture.c
 * */

SDL_Surface 			*flip_horizontal(SDL_Surface *surf);
SDL_Surface				*load_xpm(char *path);
SDL_Surface				*xpm2surface(char *path);
SDL_Surface				*load_texture(t_doom *doom, char *path);

/*
 * from line_intersect.c
 * */

int						imin(int a, int b);
int						imax(int a, int b);
int						on_segment(t_point p, t_point q, t_point r);
int						point_orientation(t_point p, t_point q, t_point r);
int						do_intersect(t_point p1, t_point q1, t_point p2, t_point q2);

/*
 * from line.c and line_safe.c
 * */

void					render_line_safe(t_line *l);
void 					render_line(t_line *l);
void					careful_render_line(t_line *l);
void					preserve_render_line(t_line *l);
void					unpreserve_render_line(t_line *l);
void					render_line_simple(t_doom *doom, t_xy a, t_xy b, int c);

/*
 * from render_2d.c
 * */

void					draw_rect_color(int x[2], int y[2], unsigned int color, SDL_Surface *buff);
void					draw_ignore_alpha(int x, int y, SDL_Surface *surf, SDL_Surface *buff);
void					render_winning_screen(t_doom *doom, int reset);
void					render_losing_screen(t_doom *doom, int reset);
void					render_loading_screen(t_doom *doom, const char *label, SDL_Surface *txt_screen, int reset);

/*
 * from pixel.c
 * */

void					draw_visible(int x, int y, SDL_Surface *surf, SDL_Surface *buff);
void					draw_surface(int x, int y, SDL_Surface *surf, SDL_Surface *buff);
void					flood_buffer(SDL_Surface *buff, uint32_t color);
void					set_protected_color(uint32_t color);
int						set_pixel_safe(SDL_Surface *buff, int x, int y, uint32_t color);
uint32_t				get_pixel(SDL_Surface *surface, int x, int y);
void					set_pixel(SDL_Surface *buff, int x, int y, uint32_t color);
void					set_pixel_dark(SDL_Surface *buff, int x, int y, uint32_t color);

/*
 * from rotation_math.c
 */

int						check_sprite_facing(t_enemy *enemy, t_model *mdl);
double					angle_abc(t_point a, t_point b, t_point c);
int			 			tail_degree_rot(t_point location, t_point *tail);

/*
 * from blend_color.c
 * */

uint32_t				blend_argb_ratio(uint32_t foreground, uint32_t background, double ratio);
uint32_t				invert_color(uint32_t color);

/*
 * from resources.c
 * */

void					load_resources(t_model *mdl);
int						load_appended(t_doom *doom);

/*
 * from destroy_sprites.c
 * */

void					destroy_walls_floors(t_doom *doom);
void					destroy_enemies(t_doom *doom);
void					destroy_pickups(t_doom *doom);
void					destroy_single_sprites(t_doom *doom);

#endif
