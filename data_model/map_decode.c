#include "doom-nukem.h"

// 2x Pickup spec functions
static void			map_pickup_to_model(const int *fields, t_model *mdl)
{
	t_pickup *new_pickup;

	mdl->pickups->id = fields[0];
	mdl->pickups->loc.x = fields[1];
	mdl->pickups->loc.y = fields[2];
	mdl->pickups->flavor = fields[3];
	mdl->pickups->weapon_type_id = fields[4];
	mdl->pickup_count++;
	if (mdl->pickup_count == 1)
		mdl->pickup_first = mdl->pickups;
	new_pickup = (t_pickup*)malloc(sizeof(t_pickup));
	mdl->pickups->next = new_pickup;
	mdl->pickups = new_pickup;
		//ft_putendl("Created a pickup to model!");
}

static t_token		*pickup_spec(void)
{
	static t_token	*spec = NULL;
	int 			i;

	if (!spec)
	{
		i = 0;
		spec = (t_token *) malloc(sizeof(t_token));
		spec->expected = 5;
		spec->sur[0] = '[';
		spec->sur[1] = ']';
		spec->equ = '=';
		spec->sep = '|';
		spec->keys = (char**)malloc(sizeof(char*) * spec->expected);
		while (i < spec->expected)
			spec->keys[i++] = (char*)malloc(sizeof(char) * TOKEN_KEY_LIMIT);
		ft_strcpy(spec->keys[0],"id");
		ft_strcpy(spec->keys[1], "loc.x");
		ft_strcpy(spec->keys[2], "loc.y");
		ft_strcpy(spec->keys[3], "flavor");
		ft_strcpy(spec->keys[4], "weapon_type_id");
		spec->result_ptr = NULL;
		spec->map_function = map_pickup_to_model;
	}
	return (spec);
}

// 2x Player Spec functions
static void			map_player_to_model(const int *fields, t_model *mdl)
{
	mdl->player.x = (double)fields[0];
	mdl->player.y = (double)fields[1];
	mdl->player.rot = fields[2];
	update_player_tail(doom_ptr(), deg_to_rad(mdl->player.rot));
		//ft_putendl("Created a player to model!");
}

static t_token		*player_spec(void)
{
	static t_token	*spec = NULL;
	int 			i;

	if (!spec)
	{
		i = 0;
		spec = (t_token*)malloc(sizeof(t_token));
		spec->expected = 3;
		spec->sur[0] = '[';
		spec->sur[1] = ']';
		spec->equ = '=';
		spec->sep = '|';
		spec->keys = (char**)malloc(sizeof(char*) * spec->expected);
		while (i < spec->expected)
			spec->keys[i++] = (char*)malloc(sizeof(char) * TOKEN_KEY_LIMIT);
		ft_strcpy(spec->keys[0],"spawn.x");
		ft_strcpy(spec->keys[1], "spawn.y");
		ft_strcpy(spec->keys[2], "rot");
		spec->result_ptr = NULL;
		spec->map_function = map_player_to_model;
	}
	//"[Player] spawn.x = 100 | spawn.y = 100 |  rot = 90";
	return (spec);
}

// 2x Enemy Spec functions

static void			map_enemy_to_model(const int *fields, t_model *mdl)
{
	t_enemy *new_enemy;

	mdl->enemies->id = fields[0];
	mdl->enemies->x = fields[1];
	mdl->enemies->tail.x = fields[1];
	mdl->enemies->y = fields[2];
	mdl->enemies->tail.y = fields[2];
	mdl->enemies->rot = fields[3];
	mdl->enemies->hp.max = fields[4];
	mdl->enemies->hp.cur = fields[4];
	mdl->enemies->wep.type_id = fields[5];
	mdl->enemy_count++;
	if (mdl->enemy_count == 1)
		mdl->enemy_first = mdl->enemies;
	new_enemy = (t_enemy*)malloc(sizeof(t_enemy));
	mdl->enemies->next = new_enemy;
	mdl->enemies = new_enemy;
	new_enemy->next = NULL;
		//ft_putendl("Created an enemy to model!");
}

static t_token		*enemy_spec(void)
{
	static t_token	*spec = NULL;
	int 			i;

	if (!spec)
	{
		i = 0;
		spec = (t_token *) malloc(sizeof(t_token));
		spec->expected = 6;
		spec->sur[0] = '[';
		spec->sur[1] = ']';
		spec->equ = '=';
		spec->sep = '|';
		spec->keys = (char**)malloc(sizeof(char*) * spec->expected);
		while (i < spec->expected)
			spec->keys[i++] = (char*)malloc(sizeof(char) * TOKEN_KEY_LIMIT);
		ft_strcpy(spec->keys[0],"id");
		ft_strcpy(spec->keys[1], "x");
		ft_strcpy(spec->keys[2], "y");
		ft_strcpy(spec->keys[3], "rot");
		ft_strcpy(spec->keys[4], "hp");
		ft_strcpy(spec->keys[5], "wep.type_id");
		spec->result_ptr = NULL;
		spec->map_function = map_enemy_to_model;
	}
	return (spec);
}

// 2x Wall Spec Functions
static void			map_wall_to_model(const int *fields, t_model *mdl)
{
	t_wall *new_wall;

	mdl->walls->id = fields[0];
	mdl->walls->start.x = fields[1];
	mdl->walls->start.y = fields[2];
	mdl->walls->end.x = fields[3];
	mdl->walls->end.y = fields[4];
	mdl->wall_count++;
	if (mdl->wall_count == 1)
		mdl->wall_first = mdl->walls;
	new_wall = (t_wall*)malloc(sizeof(t_wall));
	mdl->walls->next = new_wall;
	mdl->walls = new_wall;
		//ft_putendl("Created a wall to model!");
}

static t_token		*wall_spec(void)
{
	static t_token	*spec = NULL;
	int 			i;

	if (!spec)
	{
		i = 0;
		spec = (t_token *) malloc(sizeof(t_token));
		spec->expected = 5;
		spec->sur[0] = '[';
		spec->sur[1] = ']';
		spec->equ = '=';
		spec->sep = '|';
		spec->keys = (char**)malloc(sizeof(char*) * spec->expected);
		while (i < spec->expected)
			spec->keys[i++] = (char*)malloc(sizeof(char) * TOKEN_KEY_LIMIT);
		ft_strcpy(spec->keys[0],"id");
		ft_strcpy(spec->keys[1], "start.x");
		ft_strcpy(spec->keys[2], "start.y");
		ft_strcpy(spec->keys[3], "end.x");
		ft_strcpy(spec->keys[4], "end.y");
		spec->result_ptr = NULL;
		spec->map_function = map_wall_to_model;
	}
	return (spec);
}

// 2x Portal Spec functions
static void			map_portal_to_model(const int *fields, t_model *mdl)
{
	t_wall *new_portal;

	mdl->portals->id = fields[0];
	mdl->portals->start.x = fields[1];
	mdl->portals->start.y = fields[2];
	mdl->portals->end.x = fields[3];
	mdl->portals->end.y = fields[4];
	mdl->portal_count++;
	if (mdl->portal_count == 1)
		mdl->portal_first = mdl->portals;
	new_portal = (t_wall*)malloc(sizeof(t_wall));
	mdl->portals->next = new_portal;
	mdl->portals = new_portal;
		//ft_putendl("Created a portal to model!");
}

static t_token		*portal_spec(void)
{
	static t_token	*spec = NULL;
	int 			i;

	if (!spec)
	{
		i = 0;
		spec = (t_token*)malloc(sizeof(t_token));
		spec->expected = 5;
		spec->sur[0] = '[';
		spec->sur[1] = ']';
		spec->equ = '=';
		spec->sep = '|';
		spec->keys = (char**)malloc(sizeof(char*) * spec->expected);
		while (i < spec->expected)
			spec->keys[i++] = (char*)malloc(sizeof(char) * TOKEN_KEY_LIMIT);
		ft_strcpy(spec->keys[0],"id");
		ft_strcpy(spec->keys[1], "start.x");
		ft_strcpy(spec->keys[2], "start.y");
		ft_strcpy(spec->keys[3], "end.x");
		ft_strcpy(spec->keys[4], "end.y");
		spec->result_ptr = NULL;
		spec->map_function = map_portal_to_model;
	}
	return (spec);
}

// 2x Room spec functions
static void			map_room_to_model(const int *fields, t_model *mdl)
{
	t_room *new_room;

	mdl->rooms->id = fields[0];
	mdl->rooms->first_wall_id = fields[1];
	mdl->rooms->wall_count = fields[2];
	mdl->rooms->floor_height = fields[3];
	mdl->rooms->roof_height = fields[4];
	mdl->rooms->visual.x = -1;
	mdl->rooms->visual.y = -1;
	mdl->room_count++;
	if (mdl->room_count == 1)
		mdl->room_first = mdl->rooms;
	new_room = (t_room*)malloc(sizeof(t_room));
	mdl->rooms->next = new_room;
	mdl->rooms = new_room;
		//ft_putendl("Created a room to model!");
}

static t_token		*room_spec(void)
{
	static t_token	*spec = NULL;
	int 			i;

	if (!spec)
	{
		i = 0;
		spec = (t_token*)malloc(sizeof(t_token));
		spec->expected = 5;
		spec->sur[0] = '[';
		spec->sur[1] = ']';
		spec->equ = '=';
		spec->sep = '|';
		spec->keys = (char**)malloc(sizeof(char*) * spec->expected);
		while (i < spec->expected)
			spec->keys[i++] = (char*)malloc(sizeof(char) * TOKEN_KEY_LIMIT);
		ft_strcpy(spec->keys[0],"id");
		ft_strcpy(spec->keys[1], "first_wall_id");
		ft_strcpy(spec->keys[2], "wall_count");
		ft_strcpy(spec->keys[3], "floor_height");
		ft_strcpy(spec->keys[4], "roof_height");
		spec->result_ptr = NULL;
		spec->map_function = map_room_to_model;
	}
	return (spec);
}

// 1st Spec specific Common function
static int 			spec_key_index(t_token *spec, char *key)
{
	int	i;

	i = 0;
	while (i < spec->expected)
	{
		if (ft_strequ(spec->keys[i], key))
		{
			//printf("spec_key_index matched: %s\n", spec->keys[i]);
			return (i);
		}
		i++;
	}
	ft_putendl("Warning: Invalid spec key encountered while converting map string to model variables");
	return (-1);
}

// 2nd Spec specific Common function
static void			next_phase(int *phase, int *i)
{
	(*phase)++;
	*i = 0;
}

// 3rd Spec specific Common function
static int			finalize_results(int success, int *success_flags, t_token *spec)
{
	int i;

	if (!success)
		return (0);
	i = 0;
	while (i < spec->expected)
	{
		if (success_flags[i++] == 0)
			return (0);
			//printf("success_flag[%d] was OK\n", i - 1);
	}
	return (1);
}

// 4th Spec specific Common function
static int			assign_result(int *res, int index, int result, int *success_flags)
{
	if (index == -1)
		return (0);
	res[index] = result;
	success_flags[index] = 1;
	return (1);
}

// 5th Spec specific Common function
static void			init_token_ints(int *collected, char *key_buffer, char *atoi_buffer, int *phase)
{
	int i;

	i = 0;
	while (i < TOKEN_FIELD_LIMIT - 1)
		collected[i++] = 0;
	i = 0;
	while (i < TOKEN_KEY_LIMIT - 1)
		key_buffer[i++] = '\0';
	i = 0;
	while (i < 10 - 1)
		atoi_buffer[i++] = '\0';
	*phase = 0;
}

// 6th Spec specific Common function
static void 		repeat_phase_two(int *phase, int *i_ptr, char *key_buffer, char *atoi_buffer)
{
	int i;

	*phase = 2;
	*i_ptr = 0;
	i = 0;
	while (i < TOKEN_KEY_LIMIT - 1)
		key_buffer[i++] = '\0';
	i = 0;
	while (i < 10 - 1)
		atoi_buffer[i++] = '\0';
		//ft_putendl("Repeated phase two");
}

// 7th Spec specific Common function
static int			token_ints(char *str, t_token *spec, int *results)
{
	int		collected[TOKEN_FIELD_LIMIT];
	char 	key_buffer[TOKEN_KEY_LIMIT];
	char 	atoi_buffer[10];
	int 	phase;
	int 	i;

	init_token_ints(&collected, &key_buffer, &atoi_buffer, &phase);
	while (*str != '\0')
	{
		if (ft_isspace(*str) && str++ != NULL)
			continue;
		else if ((*str == spec->sur[0] && phase == 0) || (*str == spec->sur[1] && phase == 1) || (phase == 2 && *str == spec->equ))
			next_phase(&phase, &i);
		else if (phase == 2)
			key_buffer[i++] = *str;
		else if (phase == 3 && ft_isdigit(*str))
			atoi_buffer[i++] = *str;
		else if (phase == 3 && (*str == spec->sep))
		{
			if (!(assign_result(results, spec_key_index(spec, &key_buffer), ft_atoi(&atoi_buffer), &collected)))
				return (0);
			repeat_phase_two(&phase, &i, &key_buffer, &atoi_buffer);
		}
		str++;
	}
	return (finalize_results(assign_result(results, spec_key_index(spec, &key_buffer), ft_atoi(&atoi_buffer), &collected), &collected, spec));
}

// 8th Spec specific Common function
static int			seek_and_feed(const char *str, char **seeker, unsigned int *i, t_token *spec)
{
	char 			*feeder;
	size_t			len;

	len = *seeker == NULL ? ft_strlen(str) - *i : *seeker - str - *i;
	if (len == 1)
		return (0);
	feeder = ft_strsub(str, *i, len);
		//printf("feeder's len: %d | mfeeder: %s\n", (int)len, feeder);
	if (!token_ints(feeder, spec, spec->result_ptr))
	{
		ft_putendl("Warning: Could not parse token ints at seek_and_feed");
		return (0);
	}
	free(feeder);
		//ft_putendl("Map_walls parsed token ints succesfully.");
		//printf("walls res [%d][%d][%d][%d][%d]\n", res[0], res[1], res[2], res[3], res[4]);
	if (*seeker != NULL)
	{
		(*seeker)++;
		*seeker = ft_strchr(*seeker, '\n');
	}
	*i += len;
	return (1);
}

// 9th Common Spec Func
static void			map_string(const char *str, t_model *mdl, t_token *spec)
{
	char 			*seeker;
	int 			*res;
	unsigned int	i;

	res = NULL;
	res = (int*)malloc(sizeof(int) * spec->expected);
	spec->result_ptr = res;
	i = 0;
	seeker = ft_strchr(str, '\n');
	while (seeker != NULL)
	{
		if (seek_and_feed(str, &seeker, &i, spec))
			spec->map_function(spec->result_ptr, mdl);
	}
	if (seek_and_feed(str, &seeker, &i, spec))
		spec->map_function(spec->result_ptr, mdl);
	free(res);
	spec->result_ptr = NULL;
}

// 10th Common Spec Func
void 				map_to_model(t_mapfile *map, t_model *mdl)
{
	if (!map->player_string)
		ft_die("Fatal error: map_to_model player data missing from mapfile.");
	else
		map_string(map->player_string, mdl, player_spec());

	//ft_putendl("Attempting to convert player from map to model at map_to_model");
	//debug_model_player();
	if (map->wall_string)
		map_string(map->wall_string, mdl, wall_spec());
	//ft_putendl("Attempting to convert walls from map to model at map_to_model");
	//debug_model_walls();
	if (map->room_string)
		map_string(map->room_string, mdl, room_spec());
	//ft_putendl("Attempting to convert rooms from map to model at map_to_model");
	//debug_model_rooms();
	if (map->portal_string)
		map_string(map->portal_string, mdl, portal_spec());
	//ft_putendl("Attempting to convert portals from map to model at map_to_model");
	//debug_model_portals();
	if (map->enemy_string)
		map_string(map->enemy_string, mdl, enemy_spec());
	//ft_putendl("Attempting to convert enemies from map to model at map_to_model");
	//debug_model_enemies();
	if (map->pickup_string)
		map_string(map->pickup_string, mdl, pickup_spec());
	if (!map->wall_string && !map->room_string && !map->portal_string
		&& !map->enemy_string && !map->player_string && !map->pickup_string)
	{
		ft_putendl("Warning: Empty map data strings at map_to_model");
		doom_ptr()->map->was_filled = 0;
	}
}

/* Unit tested version of map_to_model
void 				map_to_model(t_mapfile *map, t_model *mdl)
{
	char *tmp;
	char *unit_test;
	char *unit_0 = "[Player] spawn.x = 100 | spawn.y = 100 |  rot = 90";
	char *unit_1 = "[Wall] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 310\n"
				   "[Wall] id = 1 | start.x = 135 | start.y = 478 | end.x = 189 | end.y = 90\n"
				   "[Wall] id = 2 | start.x = 635 | start.y = 578 | end.x = 789 | end.y = 290\n"
				   "[Wall] id = 3 | start.x = 35 | start.y = 578 | end.x = 789 | end.y = 209\n"
				   "[Wall] id = 4 | start.x = 63 | start.y = 58 | end.x = 79 | end.y = 29\n"
				   "[Wall] id = 5 | start.x = 65 | start.y = 78 | end.x = 89 | end.y = 299";
	char *unit_2 = "[Room] id = 0 | first_wall_id = 0 | wall_count = 3 | floor_height = 1000 | roof_height = 1300\n"
				   "[Room] id = 1 | first_wall_id = 3 | wall_count = 3 | floor_height = 1000 | roof_height = 1300";
	char *unit_3 = "[Portal] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 310\n"
				   "[Portal] id = 1 | start.x = 135 | start.y = 478 | end.x = 189 | end.y = 90";
	char *unit_4 = "[Enemy] id = 0 | x = 335 | y = 278 | rot = 7 | hp = 100 | wep.type_id = 0\n"
				   "[Enemy] id = 1 | x = 158 | y = 421 | rot = 187 | hp = 100 | wep.type_id = 0";

	tmp = map->player_string;
	unit_test = ft_strdup(unit_0);
	//map->player_string = unit_test;
	if (!map->player_string)
		ft_die("Fatal error: map_to_model player data missing from mapfile.");
	else
	{
		//ft_putendl("Attempting to convert player from map to model at map_to_model");
		map_string(map->player_string, mdl, player_spec());
		debug_model_player();
	}
	free(unit_test);
	map->player_string = tmp;

	tmp = map->wall_string;
	unit_test = ft_strdup(unit_1);
	//map->wall_string = unit_test;
	if (map->wall_string)
	{
		//ft_putendl("Attempting to convert walls from map to model at map_to_model");
		map_string(map->wall_string, mdl, wall_spec());
		debug_model_walls();
	}
	free(unit_test);
	map->wall_string = tmp;

	tmp = map->room_string;
	unit_test = ft_strdup(unit_2);
	//map->room_string = unit_test;
	if (map->room_string)
	{
		//ft_putendl("Attempting to convert rooms from map to model at map_to_model");
		map_string(map->room_string, mdl, room_spec());
		debug_model_rooms();
	}
	free(unit_test);
	map->room_string = tmp;

	tmp = map->portal_string;
	unit_test = ft_strdup(unit_3);
	//map->portal_string = unit_test;
	if (map->portal_string)
	{
		//ft_putendl("Attempting to convert portals from map to model at map_to_model");
		map_string(map->portal_string, mdl, portal_spec());
		debug_model_portals();
	}
	free(unit_test);
	map->portal_string = tmp;

	tmp = map->enemy_string;
	unit_test = ft_strdup(unit_4);
	//map->enemy_string = unit_test;
	if (map->enemy_string)
	{
		//ft_putendl("Attempting to convert enemies from map to model at map_to_model");
		map_string(map->enemy_string, mdl, enemy_spec());
		debug_model_enemies();
	}
	free(unit_test);
	map->enemy_string = tmp;

	if (!map->wall_string && !map->room_string && !map->portal_string
			&& !map->enemy_string && !map->player_string)
	{
		ft_putendl("Warning: Empty map data strings at map_to_model");
		doom_ptr()->map->was_filled = 0;
		return ;
	}
}
*/

