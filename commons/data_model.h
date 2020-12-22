#ifndef DATA_MODEL_H
# define DATA_MODEL_H

#include "doom-nukem.h"

typedef	struct 			s_doom t_doom;
typedef struct 			SDL_Surface SDL_Surface;

# define 				TOKEN_FIELD_LIMIT 16
# define				TOKEN_KEY_LIMIT 16
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
	struct s_pickup		*pickups;
	struct s_enemy		*enemy_first;
	struct s_wall		*wall_first;
	struct s_room		*room_first;
	struct s_wall		*portal_first;
	struct s_pickup		*pickup_first;
	int 				wall_count;
	int 				room_count;
	int 				portal_count;
	int 				enemy_count;
	int 				pickup_count;
} 						t_model;

typedef struct 			s_mapfile
{
	char 				*wall_string;
	char 				*room_string;
	char 				*portal_string;
	char 				*enemy_string;
	char 				*player_string;
	char 				*join_string;
	int 				was_filled;
}						t_mapfile;

typedef void			(*map_func)(const int *fields, t_model *mdl);

typedef struct			s_token
{
	int 				expected;
	char 				sur[2];
	char 				equ;
	char 				sep;
	char 				**keys;
	int 				*result_ptr;
	map_func			map_function;
}						t_token;

typedef struct			s_animation
{
	SDL_Surface 		**surfaces;
	int 				frames;
	int 				current;
}						t_animation;

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
	int					footstep_delay;
}						t_sounds;

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
	uint32_t 			avoid[3];
	struct SDL_Surface 	*buff;
	struct s_doom		*doom;
}						t_line;

/*
 * from data_model.c
 * */

void					init_model(t_doom *doom);
void	 				destroy_model(t_doom *doom);

/*
 * from load_model.c
 * */

void	 				destroy_mapdata(t_doom *doom);
int 					load_model(t_doom *doom);

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
 * from singleton_links.c
 * */

t_doom					*set_doom_singleton(t_doom *doom);
t_doom      		    *doom_ptr(void);
t_model					*get_model(void);

/*
 * from mapfile.c
 * */

t_mapfile				*init_mapfile(void);
void					destroy_mapfile(t_mapfile *map);
int						write_mapfile(char *map_path, t_mapfile *map);
int						overwrite_mapfile(char *map_path, t_mapfile *map);

/*
 * from map_encode.c
 * */

void					update_player_string(t_model *mdl, t_mapfile *map);
void					add_enemy_to_string(t_enemy *enemy, t_mapfile *map);
void					add_wall_to_string(t_wall *wall, t_mapfile *map);
void					add_portal_to_string(t_wall *portal, t_mapfile *map);
void					add_room_to_string(t_room *room, t_mapfile *map);

/*
 * from map_decode.c
 */

void 					map_to_model(t_mapfile *map, t_model *mdl);

/*
 * from texture.c
 * */

uint32_t 				get_exact_pixel(SDL_Surface *surface, int x, int y);
SDL_Surface				*xpm2surface(char *path);
SDL_Surface				*load_png(char *path);
SDL_Surface				*load_texture(t_doom *doom, char *path);

/*
 * from line.c and line_safe.c
 * */

void					render_line_safe(t_line *l);
void 					render_line(t_line *l);
void					careful_render_line(t_line *l);

/*
 * from pixel.c
 * */

void					flood_buffer(SDL_Surface *buff, uint32_t color);
void					set_protected_color(uint32_t color);
int 					set_pixel_safe(SDL_Surface *buff, int x, int y, uint32_t color);
void 					set_pixel(SDL_Surface *buff, int x, int y, uint32_t color);

#endif
