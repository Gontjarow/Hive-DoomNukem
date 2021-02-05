#ifndef DATA_MODEL_H
# define DATA_MODEL_H

#include "../doom-nukem.h"

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
# define				WAV_HEALTHPICKUP "wav/healthpickup.wav"
# define				WAV_AMMOPICKUP "wav/ammopickup.wav"
# define				WAV_WEAPONPICKUP "wav/weaponpickup.wav"

enum e_singleton_types { SINGLETON_MDL };
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
	struct s_enemy		*enemies;
	struct s_wall		*walls;
	struct s_room		*rooms;
	struct s_wall		*portals;
	struct s_effect		*effects;
	struct s_pickup		*pickups;
	struct s_enemy		*enemy_first;
	struct s_wall		*wall_first;
	struct s_room		*room_first;
	struct s_wall		*portal_first;
	struct s_effect		*effect_first;
	struct s_pickup		*pickup_first;
	int 				wall_count;
	int 				room_count;
	int 				portal_count;
	int 				enemy_count;
	int					effect_count;
	int 				pickup_count;
	char 				*chain;
} 						t_model;

typedef struct 			s_mapfile
{
	char 				*wall_string;
	char 				*room_string;
	char 				*portal_string;
	char 				*enemy_string;
	char 				*effect_string;
	char 				*pickup_string;
	char 				*player_string;
	char 				*chain_string;
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
 * from singleton_links.c
 * */

t_doom					*set_doom_singleton(t_doom *doom);
t_doom      		    *doom_ptr(void);
t_model					*get_model(void);
int 					singleton_manager(void **ptr, int type, int clear);

/*
 * from mapfile.c
 * */

int						stringify_mapfile(t_doom *doom, char *map_path);
t_mapfile				*init_mapfile(void);
void					destroy_mapfile(t_mapfile *map);
int						write_mapfile(char *map_path, t_mapfile *map);
int						overwrite_mapfile(char *map_path, t_mapfile *map);

/*
 * from map_encode.c
 * */

void					update_player_string(t_model *mdl, t_mapfile *map);
void					update_chain_string(t_model *mdl, t_mapfile *map);
void					add_enemy_to_string(t_enemy *enemy, t_mapfile *map);
void					add_wall_to_string(t_wall *wall, t_mapfile *map);
void					add_portal_to_string(t_wall *portal, t_mapfile *map);
void					add_room_to_string(t_room *room, t_mapfile *map);
void					add_effect_to_string(t_effect *effect, t_mapfile *map);
void					add_pickup_to_string(t_pickup *pickup, t_mapfile *map);

/*
 * from map_decode.c
 * */

void 					map_to_model(t_mapfile *map, t_model *mdl);

/*
 * from recalc.c
 * */

void			 		recalc_wall_ids(t_model *mdl);
void					recalc_room_ids(t_model *mdl);
void 					relink_model_walls(t_wall *relinking_wall);

/*
 * from get_by_id.c
 * */

t_room					*room_by_id(int id);
t_wall					*wall_by_id(int id);

#endif
