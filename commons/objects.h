/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:17:53 by krusthol          #+#    #+#             */
/*   Updated: 2021/01/11 15:26:57 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

#include "doom-nukem.h"

# define 				WAV_PLOP "wav/plop.wav"
# define 				WAV_STEAM0 "wav/steam/0.wav"
# define 				WAV_SWORD "wav/sword.wav"
# define				WAV_THUNDER "wav/thunder.wav"
# define 				IMG_THUNDER0 "img/thunder/0.png"
# define				WAV_PISTOLRLD "wav/pistolreload.wav"
# define				WAV_ASSAULTRLD "wav/assaultreload.wav"
# define				WAV_SMGRLD "wav/smgreload.wav"
# define				WAV_PISTOLSHOT "wav/pistolshot.wav"
# define				WAV_WALKING "wav/walking.wav"
# define				WAV_RUNNING "wav/running.wav"
# define				WAV_CROUCHING "wav/crouching.wav"
# define				WAV_ENEMYDEATH "wav/enemy_dead.wav"
# define				WAV_ASSAULTSHOT "wav/assaultshot.wav"
# define				WAV_SMGSHOT "wav/smgshot.wav"
# define				WAV_HEALTHPICKUP "wav/healthpickup.wav"
# define				WAV_AMMOPICKUP "wav/ammopickup.wav"

typedef struct 			s_doom t_doom;

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
	int 				cooldown;
	int 				reload_time;
	struct Mix_Chunk 	*fire_sound;
	struct Mix_Chunk	*reload_sound;
}						t_weapon;

typedef struct 			s_player
{
	double				rot_horizontal;
	double				rot_vertical;
	double 				x;
	double 				y;
	int 				rot;
	int					height;
	int					is_jumping;
	int					is_crouching;
	int					is_running;
	int					mov_speed;
	int					max_speed;
	int					min_speed;
	int					run_lock;
	int					crouch_lock;
	int					rot_speed;
	int					shoot_cd;
	int					reload_time;
	int					weap_id;
	struct s_coord		bullet_pos;
	struct s_weapon		weap_arr[3];
	struct s_point		tail;
	struct s_health		hp;
	struct s_weapon		wep;
}						t_player;

typedef struct 			s_enemy
{
	int 				id;
	int 				x;
	int 				y;
	int 				rot;
	int					did_shoot;
	int					who_shot;
	int					shoot_cd;
	uint32_t			ray_color;
	struct s_coord		bullet_pos;
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

typedef struct 			s_room
{
	struct s_wall		*first_wall;
	int 				first_wall_id;
	int 				id;
	int 				wall_count;
	int 				floor_height;
	int 				roof_height;
	t_point 			visual;
	struct s_room		*next;
}						t_room;

enum	e_pickup_flavors { PICKUP_HEALTH, PICKUP_AMMO, PICKUP_WEAPON };
# define PICKING_RADIUS 10

typedef struct 			s_pickup
{
	int 				id;
	struct s_point		loc;
	int 				flavor;
	int 				weapon_type_id;
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
	struct Mix_Chunk	*mcSmgRld;
	struct Mix_Chunk	*mcSmgShot;
	struct Mix_Chunk	*mcPistolShot;
	struct Mix_Chunk	*mcAssaultShot;
	struct Mix_Chunk	*mcWalking;
	struct Mix_Chunk	*mcRunning;
	struct Mix_Chunk	*mcCrouching;
	struct Mix_Chunk	*mcEnemyDeath;
	struct Mix_Chunk	*mcHealthPickup;
	struct Mix_Chunk	*mcAmmoPickup;
	int					footstep_delay;
}						t_sounds;

typedef struct			s_animation
{
	SDL_Surface 		**surfaces;
	int 				frames;
	int 				current;
}						t_animation;

typedef struct			s_menu
{
	int 				selected;
	struct s_animation	ani_thunder;
	int 				esc_lock;
	struct SDL_Surface	*alphabet[128];
	struct SDL_Surface	*hud_num[9];
	int 				alphabet_scale;
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


/*
 * from alphabet.c
 * */

void 					print_glyph_str(const char *str, SDL_Surface *buff, int x, int y);

/*
 * from debug_console.c
 * */

void 					debug_model_player(void);
void		 			debug_model_enemies(void);
void 					debug_model_walls(void);
void					debug_model_rooms(void);
void					debug_model_portals(void);
void					debug_model_pickups(void);

/*
 * from texture.c
 * */

uint32_t 				get_exact_pixel(SDL_Surface *surface, int x, int y);
SDL_Surface				*xpm2surface(char *path);
SDL_Surface				*load_texture(t_doom *doom, char *path);

/*
 * from line.c and line_safe.c
 * */

void					render_line_safe(t_line *l);
void 					render_line(t_line *l);
void					careful_render_line(t_line *l);
void					preserve_render_line(t_line *l);
void					unpreserve_render_line(t_line *l);

/*
 * from pixel.c
 * */

void					flood_buffer(SDL_Surface *buff, uint32_t color);
void					set_protected_color(uint32_t color);
int 					set_pixel_safe(SDL_Surface *buff, int x, int y, uint32_t color);
void 					set_pixel(SDL_Surface *buff, int x, int y, uint32_t color);

/*
 * from rotation_math.c
 */

int			 			tail_degree_rot(t_point location, t_point *tail);

#endif
