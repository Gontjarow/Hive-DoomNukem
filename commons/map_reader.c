#include "doom-nukem.h"

// Generalized struct, save where mapfile struct is saved at
typedef struct		s_token
{
	int 			expected;
	char 			sur[2];
	char 			equ;
	char 			sep;
	char 			**keys;
	int 			*result_ptr;
}					t_token;

// 1st Wall spec function
t_token				*wall_spec(void)
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
		spec->result_ptr == NULL;
	}
		//ft_putendl("Returned map specification for wall_spec succesfully!");
		//printf("key[%d] = %s\n", 0, spec->keys[0]);
		//printf("key[%d] = %s\n", 1, spec->keys[1]);
		//printf("key[%d] = %s\n", 2, spec->keys[2]);
		//printf("key[%d] = %s\n", 3, spec->keys[3]);
		//printf("key[%d] = %s\n", 4, spec->keys[4]);
	return (spec);
}

//[Room] id = 0 | first_wall_id = 335 | wall_count = 278 | floor_height = 389 | roof_height = 190

t_token				*room_spec(void)
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
	}
		//ft_putendl("Returned map specification for room_spec succesfully!");
		//printf("key[%d] = %s\n", 0, spec->keys[0]);
		//printf("key[%d] = %s\n", 1, spec->keys[1]);
		//printf("key[%d] = %s\n", 2, spec->keys[2]);
		//printf("key[%d] = %s\n", 3, spec->keys[3]);
		//printf("key[%d] = %s\n", 4, spec->keys[4]);
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

// [Wall] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 190
// is the same as
// [Wall]id=0|start.x=335|start.y=278|end.x=389|end.y=190

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

// [Wall] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 190

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

static void			map_rooms(t_mapfile *map, t_model *mdl)
{
	t_token			*spec;
	int 			res[5];

	spec = room_spec();
	if (!token_ints(map->room_string, spec, &res))
		ft_putendl("Warning: Could not parse token ints at map_rooms");
	else
		ft_putendl("Map_rooms parsed token ints succesfully.");
	printf("rooms res [%d][%d][%d][%d][%d]\n", res[0], res[1], res[2], res[3], res[4]);
}

// Common, generalized helper (8th Spec specific Common function)
static int			seek_and_feed(const char *str, char **seeker, unsigned int *i, t_token *spec)
{
	char 			*feeder;
	size_t			len;

	len = *seeker == NULL ? ft_strlen(str) - *i : *seeker - str - *i;
	feeder = ft_strsub(str, *i, len);
	printf("feeder: %s\n", feeder);
	if (!token_ints(feeder, spec, spec->result_ptr))
	{
		ft_putendl("Warning: Could not parse token ints at map_walls");
		return (0);
	}
		//ft_putendl("Map_walls parsed token ints succesfully.");
	free(feeder);
	//printf("walls res [%d][%d][%d][%d][%d]\n", res[0], res[1], res[2], res[3], res[4]);
	if (*seeker != NULL)
	{
		(*seeker)++;
		*seeker = ft_strchr(*seeker, '\n');
	}
	*i += len;
	return (1);
}

// 2nd Wall Func

static void			map_wall_to_model(int *field, t_model *mdl)
{
	t_wall *new_wall;

	mdl->walls->id = field[0];
	mdl->walls->start.x = field[1];
	mdl->walls->start.y = field[2];
	mdl->walls->end.x = field[3];
	mdl->walls->end.y = field[4];
	mdl->wall_count++;
	if (mdl->wall_count == 1)
		mdl->wall_first = mdl->walls;
	new_wall = (t_wall*)malloc(sizeof(t_wall));
	mdl->walls->next = new_wall;
	mdl->walls = new_wall;
	ft_putendl("Created wall to model!");
}

// 3rd Wall Func

static void			map_walls(t_mapfile *map, t_model *mdl, t_token *spec)
{
	char 			*seeker;
	int 			res[5];
	unsigned int	i;

	spec->result_ptr = &res;
	i = 0;
	seeker = ft_strchr(map->wall_string, '\n');
	while (seeker != NULL)
	{
		if (seek_and_feed(map->wall_string, &seeker, &i, spec))
			map_wall_to_model(spec->result_ptr, mdl);
	}
	if (seek_and_feed(map->wall_string, &seeker, &i, spec))
		map_wall_to_model(spec->result_ptr, mdl);
	debug_model_walls();
	ft_die("Model manipulated.");
}

void 				map_to_model(t_mapfile *map, t_model *mdl) {
	/*
	if (!map->player_string)
		ft_die("Fatal error: map_to_model mapfile player data missing from mapfile.");
	else
		map_player(map, mdl);*/
	char *unit_test;
	char *unit_0 = "[Wall] id = 0 | start.x = 335 | start.y = 278 | end.x = 389 | end.y = 310\n"
				   "[Wall] id = 1 | start.x = 135 | start.y = 478 | end.x = 189 | end.y = 90\n"
				   "[Wall] id = 2 | start.x = 635 | start.y = 578 | end.x = 789 | end.y = 290";
	//char 			*unit_1 = "[Room] id = 0 | first_wall_id = 0 | wall_count = 6 | floor_height = 1000 | roof_height = 1300";
	//Room id: 0 | first_wall_id: 0 | wall_count: 6 | floor_height: 1000 | roof_height: 1300
	char *tmp;

	tmp = map->wall_string;
	unit_test = (char *) malloc(sizeof(char) * ft_strlen(unit_0) + 1);
	ft_strcpy(unit_test, unit_0);
	map->wall_string = unit_test;
	//printf("map->wall_string:\n%s\n", map->wall_string);
	if (map->wall_string) {
		//ft_putendl("Attempting to convert walls from map to model at map_to_model");
		map_walls(map, mdl, wall_spec());
	}
	free(unit_test);
	map->wall_string = tmp;
}
	//printf("reverted map->wall_string back to:\n%s\n", map->wall_string);

	/*
	map->room_string = unit_test;
	printf("unit_test swapped map->room_string:\n%s\n", map->room_string);
	if (map->room_string) {
		map_rooms(map, mdl);
		ft_putendl("Attempting to convert rooms from map to model at map_to_model");
	}
	free(unit_test);
	map->room_string = tmp;

	if (map->portal_string) {
		map_portals(map, mdl);
		ft_putendl("Converted walls from map to model at map_to_model");
	}
	if (map->enemy_string) {
		map_enemies(map, mdl);
		ft_putendl("Converted walls from map to model at map_to_model");
	}
	//printf("%s\n%s\n%s\n%s\n[BIG DEBUG DATA]\n", doom->map->wall_string, doom->map->enemy_string, doom->map->portal_string, doom->map->player_string);
	if (!map->wall_string && !map->room_string && !map->portal_string
			&& !map->enemy_string && !map->player_string)
	{
		ft_putendl("Warning: Map data strings empty at parse_mapfile.");
		doom_ptr()->map->was_filled = 0;
		return ;
	}
}

// Potential replacement for read_mapfile

int				stringify_mapfile(t_doom *doom, char *map_path)
{
	int		opened;
	char	*line;
	char 	*join;

	init_mapdata(doom);
	opened = open(map_path, O_RDONLY);
	line = NULL;
	if (opened > 1)
	{
		while (get_next_line(opened, &line))
		{
			if (ft_strlen(line) < 3) {
				free(line);
				continue;
			}
			if (ft_strnstr(line, "[Wall]", 6))
			{
				if (doom->map->wall_string == NULL)
					doom->map->wall_string = ft_strnew(1);
				join = doom->map->wall_string;
				doom->map->wall_string = ft_strjoin(doom->map->wall_string, line);
				free(join);
				join = doom->map->wall_string;
				doom->map->wall_string = ft_strjoin(doom->map->wall_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Room]", 6)) {
				if (doom->map->room_string == NULL)
					doom->map->room_string = ft_strnew(1);
				join = doom->map->room_string;
				doom->map->room_string = ft_strjoin(doom->map->room_string, line);
				free(join);
				join = doom->map->room_string;
				doom->map->room_string = ft_strjoin(doom->map->room_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Portal]", 8)) {
				if (doom->map->portal_string == NULL)
					doom->map->portal_string = ft_strnew(1);
				join = doom->map->portal_string;
				doom->map->portal_string = ft_strjoin(doom->map->portal_string, line);
				free(join);
				join = doom->map->portal_string;
				doom->map->portal_string = ft_strjoin(doom->map->portal_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Enemy]", 7)) {
				if (doom->map->enemy_string == NULL)
					doom->map->enemy_string = ft_strnew(1);
				join = doom->map->enemy_string;
				doom->map->enemy_string = ft_strjoin(doom->map->enemy_string, line);
				free(join);
				join = doom->map->enemy_string;
				doom->map->enemy_string = ft_strjoin(doom->map->enemy_string, "\n");
				free(join);
			}
			else if (ft_strnstr(line, "[Player]", 8)){
				if (doom->map->player_string == NULL)
					doom->map->player_string = ft_strnew(1);
				join = doom->map->player_string;
				doom->map->player_string = ft_strjoin(doom->map->player_string, line);
				free(join);
				join = doom->map->player_string;
				doom->map->player_string = ft_strjoin(doom->map->player_string, "\n");
				free(join);
			}
			free(line);
		}
		close(opened);
		return (1);
	}
	//ft_putendl("Warning: read_mapfile failed to open supplied map path:");
	//ft_putendl(map_path);
	return (0);
}*/
