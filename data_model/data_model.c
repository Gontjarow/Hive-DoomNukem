#include "doom-nukem.h"

void		init_model(t_doom *doom)
{
	//puts("MODEL BORN!!!");
	// Mallocing model data type itself and linking parent pointer
	doom->mdl = (t_model*)malloc(sizeof(t_model));
	if (!doom->mdl)
		ft_die("Fatal error: Mallocing model struct failed at init_model.");
	get_model();
	doom->mdl->parent = doom;
	// Mallocing lists of objects
	doom->mdl->walls = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->mdl->walls)
		ft_die("Fatal error: Mallocing walls struct failed at init_model.");
	doom->mdl->rooms = (t_room*)malloc(sizeof(t_room));
	if (!doom->mdl->rooms)
		ft_die("Fatal error: Mallocing rooms struct failed at init_model.");
	doom->mdl->portals = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->mdl->portals)
		ft_die("Fatal error: Mallocing portals struct failed at init_model.");
	doom->mdl->enemies = (t_enemy*)malloc(sizeof(t_enemy));
	if (!doom->mdl->enemies)
		ft_die("Fatal error: Mallocing enemies struct failed at init_model.");
	doom->mdl->effects = (t_effect*)malloc(sizeof(t_effect));
	if (!doom->mdl->effects)
		ft_die("Fatal error: Mallocing effects struct failed at init_model.");
	doom->mdl->pickups = (t_pickup*)malloc(sizeof(t_pickup));
	if (!doom->mdl->pickups)
		ft_die("Fatal error: Mallocing pickups struct failed at init_model.");
	doom->mdl->rsrcs = NULL;

	// Mallocing the underlying polygon 2D map that holds room data and is used to tell
	// and keep track of which room the player or any other point is belonging to instantly
	doom->mdl->poly_map = SDL_CreateRGBSurfaceWithFormat(0, GAME_POLYMAP_WIDTH, GAME_POLYMAP_HEIGHT, 32, SDL_GetWindowPixelFormat(doom->win));
	if (!doom->mdl->poly_map)
		ft_die("Fatal error: SDL_CreateRGBSurfaceWithFormat failed to create poly_map for doom->mdl struct at init_model.");
	flood_buffer(doom->mdl->poly_map, 0xffffffff);

	// Initializing values to pointers and members
	doom->mdl->chain = NULL;
	doom->mdl->wall_first = NULL;
	doom->mdl->room_first = NULL;
	doom->mdl->portal_first = NULL;
	doom->mdl->enemy_first = NULL;
	doom->mdl->pickup_first = NULL;
	doom->mdl->effect_first = NULL;
	doom->mdl->rsrc_first = NULL;
	doom->mdl->wall_count = 0;
	doom->mdl->room_count = 0;
	doom->mdl->portal_count = 0;
	doom->mdl->enemy_count = 0;
	doom->mdl->pickup_count = 0;
	doom->mdl->effect_count = 0;
	doom->mdl->rsrc_count = 0;

	// Initializing the player variables. Strongly related to the ../game/* functions!
	doom->mdl->player.x = -1;
	doom->mdl->player.y = -1;
	doom->mdl->player.z = -1;
	doom->mdl->player.z_velocity = 0.0;
	doom->mdl->player.yaw = 0.0;
	doom->mdl->player.rot_horizontal = 0;
	doom->mdl->player.rot_vertical = 0;
	doom->mdl->player.is_jumping = 0;
	doom->mdl->player.is_crouching = 0;
	doom->mdl->player.is_running = 0;
	doom->mdl->player.height = STAND_HEIGHT;
	doom->mdl->player.min_speed = 300;	// crouched
	doom->mdl->player.mov_speed = 500;	// walking
	doom->mdl->player.max_speed = 800;	// running
	doom->mdl->player.rot_speed = 5;
	doom->mdl->player.run_lock = 0;
	doom->mdl->player.crouch_lock = 0;
	doom->mdl->player.weap_id = PISTOL;
	doom->mdl->player.reload_time = 0;
	doom->mdl->player.shoot_cd = 0;
	doom->mdl->player.bullet_pos.x = 0.0;
	doom->mdl->player.bullet_pos.y = 0.0;
	doom->mdl->player.hp.cur = 100;
	doom->mdl->player.hp.max = 100;
	doom->mdl->player.shooting = 0;
	doom->mdl->player.invis = 0;
	doom->mdl->player.room_id = -1;
	doom->mdl->player.room = NULL;
		//puts("PLAYER ROOM INITED TO NULL AND ID TO -1");
	init_player_weapon(doom);
}

void 		destroy_model(t_doom *doom)
{
	// TODO ITERATE THROUGH LINKED LISTS AND DESTROY ALL NODES

	free(doom->mdl->enemies);
	free(doom->mdl->portals);
	free(doom->mdl->walls);
	free(doom->mdl->rooms);
	free(doom->mdl->effects);
	free(doom->mdl->pickups);
	if (doom->mdl->rsrcs != NULL)
		free(doom->mdl->rsrcs);
	if (doom->mdl->chain != NULL)
		free(doom->mdl->chain);
	doom->mdl->chain = NULL;
	doom->mdl->enemy_first = NULL;
	doom->mdl->portal_first = NULL;
	doom->mdl->wall_first = NULL;
	doom->mdl->room_first = NULL;
	doom->mdl->effect_first = NULL;
	doom->mdl->pickup_first = NULL;
	doom->mdl->rsrc_first = NULL;
	doom->mdl->parent = NULL;
	free(doom->mdl);
	doom->mdl = NULL;
	singleton_manager(NULL, SINGLETON_MDL, 1);
	//puts("MODEL EVAPORATED!!!");
}
