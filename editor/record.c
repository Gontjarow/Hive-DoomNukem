#include "doom-nukem.h"

/*
void		create_strings_from_state(t_editor *edt)
{
	int		count;
	t_room	*room;
	t_wall	*wall;

	count = edt->room_count;
	room = edt->room_first;
	while (count--)
	{
		add_room_to_string(edt, room);
		room = room->next;
	}
	count = edt->wall_count;
	wall = edt->wall_begin;
	while (count--)
	{
		add_wall_to_string(edt, wall);
		wall = wall->next;
	}
	count = edt->portal_count;
	wall = edt->portal_begin;
	while (count--)
	{
		add_portal_to_string(edt, wall);
		wall = wall->next;
	}
}

void	record_room(t_editor *edt)
{
	t_room	*next_room;
	int 	wallcountofpreviousrooms;

	wallcountofpreviousrooms = wall_count_of_previous_rooms(edt);
	edt->rooms->id = edt->room_count;
	edt->rooms->floor_height = 1000;
	edt->rooms->roof_height = 1300;
	edt->room_count++;
	next_room = (t_room*)malloc(sizeof(t_room));
	if (!next_room)
		ft_die("Fatal error: Could not malloc t_room at record_room.");
	if (edt->room_count == 1)
	{
		edt->rooms->first_wall = edt->wall_begin;
		edt->rooms->wall_count = edt->wall_count;
		edt->room_first = edt->rooms;
	}
	else
	{
		edt->rooms->wall_count = edt->wall_count - wallcountofpreviousrooms;
		edt->rooms->first_wall = wall_by_count(edt, wallcountofpreviousrooms);
	}
	edt->rooms->first_wall_id = edt->rooms->first_wall->id;
	//printf("Room id: %d | first_wall_id: %d | wall_count: %d | floor_height: %d | roof_height: %d\n",
	//	   edt->rooms->id, edt->rooms->first_wall_id, edt->rooms->wall_count, edt->rooms->floor_height, edt->rooms->roof_height);
	// DEPRECEATED, NOW HANDLED CENTRALLY AT CREATE_STRINGS_FROM_STATE
	//expand_room_string(edt);
	expand_room_polygon_map(edt->rooms, edt->parent, edt->poly_map, edt->conversion_colors);
	find_visual_xy(edt, edt->rooms);
	edt->rooms->next = next_room;
	edt->rooms = next_room;
}
*/

static int 	degree_rot(t_point location, t_point *tail)
{
	double	result;
	int		x;
	int 	y;

	x = tail->x - location.x;
	y = tail->y - location.y;
	result = atan2(y, x) * 180.0 / M_PI;;
	result += 180.0;
	return ((int)result);
}

// TODO ADD DRAWING OF TAILS BASED ON ROT VALUES TO EDITOR VIEW
//		(COPY FROM GAME.C PROPABLY)

void	record_player(t_point location, t_point *tail, t_model *mdl)
{
	//TODO FILL FROM LEGACY CODE BASE
	//TODO STRINGIFY THE RESULT ?
	mdl->player.x = (double)location.x;
	mdl->player.y = (double)location.y;
	mdl->player.rot = degree_rot((t_point){mdl->player.x, mdl->player.y}, tail);
		printf("Player rot value is %d\n", mdl->enemies->rot);
		//puts("Player planted!");
}

void	record_enemy(t_point location, t_point *tail, t_model *mdl)
{
	t_enemy	*new_enemy;

	mdl->enemies->id = mdl->enemy_count;
	mdl->enemies->x = location.x;
	mdl->enemies->y = location.y;
	mdl->enemies->wep.type_id = 0;
	mdl->enemies->hp.max = 100;
	mdl->enemies->rot = degree_rot((t_point){location.x, location.y}, tail);
		printf("Enemy rot value is %d\n", mdl->enemies->rot);
	new_enemy = (t_enemy*)malloc(sizeof(t_enemy));
	if (!new_enemy)
		ft_die("Fatal error: Could not malloc t_enemy at record_enemy");
	if (mdl->enemy_count == 0)
		mdl->enemy_first = mdl->enemies;
	mdl->enemy_count++;
	mdl->enemies->next = new_enemy;
	mdl->enemies = new_enemy;
	//TODO FILL FROM LEGACY CODE BASE // DONE
	//TODO STRINGIFY THE RESULT ?
}
/*
void	record_enemy_legacy(int x, int y, t_editor *edt)
{
	t_point		rot_point;
	t_point		enemy_point;
	t_enemy		*next_enemy;

	if (!edt->enemy_set)
	{
		edt->enemies->id = edt->enemy_count;
		edt->enemies->x = x;
		edt->enemies->y = y;
		edt->enemy_set = 1;
		return ;
	}
	rot_point.x = x;
	rot_point.y = y;
	enemy_point.x = edt->enemies->x;
	enemy_point.y = edt->enemies->y;
	modify_line_length(15, &enemy_point, &rot_point, &edt->enemies->tail);
	edt->last_enemy.x = edt->enemies->x;
	edt->last_enemy.y = edt->enemies->y;
	edt->enemies->wep.type_id = 0;
	edt->enemies->hp.max = 100;
	edt->enemies->rot = degree_rot(edt->enemies->x, edt->enemies->y, &edt->enemies->tail);
	expand_enemy_string(edt);
	next_enemy = (t_enemy*)malloc(sizeof(t_enemy));
	if (!next_enemy)
		ft_die("Fatal error: Could not malloc t_enemy at record_enemy.");
	if (edt->enemy_count == 0)
		edt->enemy_first = edt->enemies;
	edt->enemy_count++;
	edt->enemies->next = next_enemy;
	edt->enemies = next_enemy;
	edt->enemy_set = 0;
	print_walls(edt);
}
*/
/*
void	record_player(int x, int y, t_editor *edt)
{
	t_point	start;
	if (!edt->player_set)
	{
		edt->player.x = x;
		edt->player.y = y;
		edt->player_set = -1;
	}
	else if (edt->player_set == -1)
	{
		edt->player.tail.x = x;
		edt->player.tail.y = y;
		start.x = (int)edt->player.x;
		start.y = (int)edt->player.y;
		modify_line_length(15, &start, &edt->player.tail, &edt->player.tail);
		edt->player.rot = degree_rot(edt->player.x, edt->player.y, &edt->player.tail);
		edt->player_set = 1;
		update_player_string(edt);
	}
	print_walls(edt);
}

void 	record_portal(t_editor *edt)
{
	t_wall *next_portal;

	edt->portals->id = edt->portal_count;
	edt->portals->start.x = edt->new_portal->start.x;
	edt->portals->start.y = edt->new_portal->start.y;
	edt->portals->end.x = edt->new_portal->end.x;
	edt->portals->end.y = edt->new_portal->end.y;
	// DEPRECEATED. REPLACED WITH CENTRAL CREATE_STRINGS_FROM_STATE
	//expand_portal_string(edt);
	next_portal = (t_wall*)malloc(sizeof(t_wall));
	if (!next_portal)
		ft_die("Fatal error: Could not malloc t_wall at record_portal.");
	if (edt->portal_count == 0)
		edt->portal_begin = edt->portals;
	edt->portal_count++;
	edt->portals->next = next_portal;
	edt->portals = next_portal;
	print_walls(edt);
}*/