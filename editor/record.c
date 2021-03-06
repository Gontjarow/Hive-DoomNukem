#include "doom-nukem.h"

void			show_editor_polymap(SDL_Surface *polymap, uint32_t *colors)
{
	SDL_Surface		*buff;
	unsigned int	*pixels;
	unsigned int 	col;
	int				x;
	int				y;
	int 			src_y;
	int 			src_x;
	int 			sy;
	int 			sx;
	int 			zf;

	zf = get_state()->zoom_factor;
	sx = get_state()->scroll_x;
	sy = get_state()->scroll_y;
	y = 0;
	buff = editor_back_buffer()->buff;
	pixels = (unsigned int*)polymap->pixels;
	while (y < EDT_WIN_HEIGHT)
	{
		x = 0;
		src_y = sy + (y * zf);
		while (x < EDT_WIN_WIDTH)
		{
			src_x = sx + (x * zf);
			col = pixels[src_x + (src_y * GAME_POLYMAP_WIDTH)];
			if (colors != NULL)
				col = colors[(int)col];
			if (col != 0xffffffff)
				set_pixel(buff, x, y, (uint32_t)col);
			x++;
		}
		y++;
	}
		//printf("iterated (%d) times, x, y = %d, %d\n", x * y, x, y);
	editor_back_buffer()->rendering_on = 1;
		//puts("Drew polymap onto screen!");
}

void 			record_portal(t_model *mdl, t_wall *wall)
{
	t_wall	*next_portal;

	mdl->portals->id = mdl->portal_count;
	mdl->portals->start.x = wall->start.x;
	mdl->portals->start.y = wall->start.y;
	mdl->portals->end.x = wall->end.x;
	mdl->portals->end.y = wall->end.y;
	mdl->portal_count++;
	next_portal = (t_wall*)malloc(sizeof(t_wall));
	if (!next_portal)
		ft_die("Fatal error: Could not malloc t_wall at record_portal.");
	if (mdl->portal_count == 1)
		mdl->portal_first = mdl->portals;
	mdl->portals->next = next_portal;
	mdl->portals = next_portal;
}

int			record_room(t_model *mdl, t_wall *room_first_wall, int prev_rooms_wall_count)
{
	t_room	*next_room;
	int 	can_be_recorded;

	mdl->rooms->id = mdl->room_count;
	mdl->rooms->floor_height = select_logic()->last_floor;
	mdl->rooms->roof_height = select_logic()->last_roof;
	mdl->rooms->visual.x = -1;
	mdl->rooms->visual.y = -1;
	mdl->room_count++;
	next_room = (t_room*)malloc(sizeof(t_room));
	if (!next_room)
		ft_die("Fatal error: Could not malloc t_room at record_room.");
	if (mdl->room_count == 1)
	{
		mdl->rooms->first_wall = mdl->wall_first;
		mdl->rooms->wall_count = mdl->wall_count;
		mdl->room_first = mdl->rooms;
	}
	else
	{
		mdl->rooms->wall_count = mdl->wall_count - prev_rooms_wall_count;
		mdl->rooms->first_wall = room_first_wall;
	}
	mdl->rooms->first_wall_id = mdl->rooms->first_wall->id;
	can_be_recorded = is_clockwise_convex_polygon(mdl->rooms, mdl->rooms->first_wall, mdl->rooms->wall_count);
	add_room_polymap(mdl->rooms, mdl->poly_map, get_conv_colors());
	mdl->rooms->next = next_room;
	mdl->rooms = next_room;
	if (!can_be_recorded)
	{
		delete_room(room_by_id(mdl->room_count - 1),
			room_by_id(mdl->room_count - 1)->wall_count, mdl);
		Mix_PlayChannel(-1, doom_ptr()->sounds->mcPlop, 0);
		get_state()->gui->change_zoom(get_state());
		return (0);
	}
	else if (can_be_recorded == NEEDS_FLIPPING)
		flip_room(room_by_id(mdl->room_count - 1), mdl);
	return (1);
}

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

void	record_player(t_point location, t_point *tail, t_model *mdl)
{
	mdl->player.x = (double)location.x;
	mdl->player.y = (double)location.y;
	mdl->player.rot = degree_rot((t_point){mdl->player.x, mdl->player.y}, tail);
		//printf("Player rot value is %d\n", mdl->enemies->rot);
		//puts("Player planted!");
}

t_enemy	*record_enemy(t_point location, t_point *tail, t_model *mdl)
{
	t_enemy	*added_enemy;
	t_enemy	*new_enemy;

	added_enemy = mdl->enemies;
	mdl->enemies->id = mdl->enemy_count;
	mdl->enemies->x = location.x;
	mdl->enemies->y = location.y;
	mdl->enemies->wep.type_id = get_state()->selected_weapon_type;
	mdl->enemies->hp.max = 100;
	mdl->enemies->rot = degree_rot((t_point){location.x, location.y}, tail);
		//printf("Enemy rot value is %d\n", mdl->enemies->rot);
	mdl->enemies->ai.type_id = get_state()->selected_ai_type;
	//mdl->enemies->sprite_id = get_state()->selected_sprite_id;
	new_enemy = (t_enemy*)malloc(sizeof(t_enemy));
	if (!new_enemy)
		ft_die("Fatal error: Could not malloc t_enemy at record_enemy");
	if (mdl->enemy_count == 0)
		mdl->enemy_first = mdl->enemies;
	mdl->enemy_count++;
	mdl->enemies->next = new_enemy;
	mdl->enemies = new_enemy;
	return (added_enemy);
}

void		create_strings_from_model(t_model *mdl, t_mapfile *map)
{
	int			count;
	t_enemy		*enemy;
	t_room		*room;
	t_wall		*wall_or_portal;
	t_pickup	*pickup;
	t_effect	*effect;

	update_player_string(mdl, map);
	if (map->chain_string)
		update_chain_string(mdl, map);
	count = mdl->enemy_count;
	enemy = mdl->enemy_first;
	while (count--)
	{
		add_enemy_to_string(enemy, map);
		enemy = enemy->next;
	}
	count = mdl->room_count;
	room = mdl->room_first;
	while (count--)
	{
		add_room_to_string(room, map);
		room = room->next;
	}
	count = mdl->wall_count;
	wall_or_portal = mdl->wall_first;
	while (count--)
	{
		add_wall_to_string(wall_or_portal, map);
		wall_or_portal = wall_or_portal->next;
	}
	count = mdl->portal_count;
	wall_or_portal = mdl->portal_first;
	while (count--)
	{
		add_portal_to_string(wall_or_portal, map);
		wall_or_portal = wall_or_portal->next;
	}
	count = mdl->effect_count;
	effect = mdl->effect_first;
	while (count--)
	{
		add_effect_to_string(effect, map);
		effect = effect->next;
	}
	count = mdl->pickup_count;
	pickup = mdl->pickup_first;
	while (count--)
	{
		add_pickup_to_string(pickup, map);
		pickup = pickup->next;
	}
}

/* LEGACY

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
}
*/
