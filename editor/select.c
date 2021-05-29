#include "doom-nukem.h"

t_select 		*select_logic(void)
{
	static		t_select *logic = NULL;

	if (!logic)
	{
		logic = (t_select*)malloc(sizeof(t_select));
		if (!logic)
			ft_die("Fatal error: Could not malloc logic for planting at select_logic");
		logic->selected_room_id = -1;
		logic->last_floor = FLOOR_DEFAULT;
		logic->last_roof = ROOF_DEFAULT;
		logic->selected_wall_id = -1;
		logic->adding_slope = 0;
		logic->virtual_slope_wall.id = -1;
	}
	return (logic);
}

void			select_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	select_change_zoom(state);
		//puts("Activating select");
}

void			select_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	select_logic()->selected_room_id = -1;
	select_change_zoom(state);
		//puts("Deactivating select");
}

void			select_change_zoom(t_state *state)
{
	redraw_editor_to_backbuffer(COLOR_LINE);
}

void 			select_mouse_motion(int x, int y)
{
	if (x < 0 && y < 0)
		puts("Select mouse motion..!");
}

void 			select_roof(int dir)
{
	t_room		*room;

	if (get_state()->gui != mode_select())
		return ;
	if (select_logic()->selected_room_id == -1)
		return ;
	room = room_by_id(select_logic()->selected_room_id);
	room->roof_height += dir * HEIGHT_STEPPING;
	if (room->roof_height < ROOF_MIN)
		room->roof_height = ROOF_MIN;
	if (room->roof_height > ROOF_MAX)
		room->roof_height = ROOF_MAX;
	if (room->roof_height - room->floor_height < FLOOR_ROOF_DIFF_LIMIT)
		room->roof_height = room->floor_height + FLOOR_ROOF_DIFF_LIMIT;
	select_logic()->last_floor = room->floor_height;
	select_logic()->last_roof = room->roof_height;
	select_change_zoom(get_state());
	get_state()->saving_choice = 0;
}

void 			select_floor(int dir)
{
	t_room		*room;

	if (get_state()->gui != mode_select())
		return ;
	if (select_logic()->selected_room_id == -1)
		return ;
	room = room_by_id(select_logic()->selected_room_id);
	room->floor_height += dir * HEIGHT_STEPPING;
	if (room->floor_height < FLOOR_MIN)
		room->floor_height = FLOOR_MIN;
	if (room->floor_height > FLOOR_MAX)
		room->floor_height = FLOOR_MAX;
	if (room->roof_height - room->floor_height < FLOOR_ROOF_DIFF_LIMIT)
		room->floor_height = room->roof_height - FLOOR_ROOF_DIFF_LIMIT;
	select_logic()->last_floor = room->floor_height;
	select_logic()->last_roof = room->roof_height;
	select_change_zoom(get_state());
	get_state()->saving_choice = 0;
}

void 			select_delete_room(void)
{
	if (select_logic()->selected_room_id == -1)
		return;
	delete_portals_by_room(room_by_id(select_logic()->selected_room_id), get_model());
	delete_room(room_by_id(select_logic()->selected_room_id),
		room_by_id(select_logic()->selected_room_id)->wall_count, get_model());
	select_logic()->selected_room_id = -1;
	select_change_zoom(get_state());
	get_state()->saving_choice = 0;
}

static void		select_wall(int room_id, int cycle_dir)
{
	t_room*		room;

	select_logic()->selected_wall_id += cycle_dir;
	room = room_by_id(room_id);
	if (room->first_wall_id > select_logic()->selected_wall_id)
	{
		select_logic()->selected_wall_id = room->first_wall_id + room->wall_count - 1;
	}
	else if (select_logic()->selected_wall_id >= room->first_wall_id + room->wall_count)
	{
		select_logic()->selected_wall_id = room->first_wall_id;
	}
	select_change_zoom(get_state());
}

static void 	select_room(int x, int y, int cycle_dir)
{
	int clicked_room_id;

	clicked_room_id = room_id_from_polymap(get_model()->poly_map, x, y);
	if (clicked_room_id == -1)
	{
		select_logic()->selected_room_id = clicked_room_id;
		return ;
	}		
	if (clicked_room_id == select_logic()->selected_room_id && cycle_dir == NEXT_WALL)
	{
		select_logic()->selected_room_id = clicked_room_id;
		select_wall(clicked_room_id, NEXT_WALL);
		return ;
	}
	if (clicked_room_id == select_logic()->selected_room_id && cycle_dir == PREV_WALL)
	{
		select_logic()->selected_room_id = clicked_room_id;
		select_wall(clicked_room_id, PREV_WALL);
		return ;
	}
	select_logic()->selected_room_id = clicked_room_id;
	select_logic()->selected_wall_id = room_by_id(select_logic()->selected_room_id)->first_wall_id;
	select_change_zoom(get_state());
}

static t_xy		hinge_algorithm(t_xy point, t_wall *hinge, int hinge_mirror_x, int hinge_mirror_y)
{
	t_xy orig;
	t_xy vec;
	t_xy half;

	orig.x = hinge->start.x;
	orig.y = hinge->start.y;	
	vec.x = hinge_mirror_x;
	vec.y = hinge_mirror_y;	
	half = vec2_mul(vec2_norm(vec), 0.5f * vec2_mag(vec));
	half = vec2_rot(half, M_PI);	
	return (vec2_point_to_line(point, orig, vec2_norm(half)));
}
/*
int			record_room(t_model *mdl, t_wall *room_first_wall, int prev_rooms_wall_count)
{
	t_room	*next_room;
	int 	can_be_recorded;

	mdl->rooms->id = mdl->room_count;
	mdl->rooms->floor_height = select_logic()->last_floor;
	mdl->rooms->roof_height = select_logic()->last_roof;
	mdl->rooms->visual.x = -1;
	mdl->rooms->visual.y = -1;
	mdl->rooms->has_ceiling = get_state()->give_ceiling_to_rooms;
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
*/

static t_wall	*make_wall(t_point start, t_point end, int can_portal)
{
	t_wall	*wall;
	t_wall	*ret_wall;

	wall = get_model()->walls;
	ret_wall = wall;
	wall->id = get_model()->wall_count;
	wall->start.x = start.x;
	wall->start.y = start.y;
	wall->end.x = end.x;
	wall->end.y = end.y;
	wall->can_portal = can_portal;
	wall->texture_id = get_state()->selected_weapon_type;
	//printf("Wall->texture_id was set to %d\n", wall->texture_id);
	wall->active_sprite = NULL;
	get_model()->wall_count++;
	wall = (t_wall*)malloc(sizeof(t_wall));
	if (!wall)
		ft_die("Fatal error: Could not malloc t_wall at make_wall.");
	if (get_model()->wall_count == 1)
		get_model()->wall_first = get_model()->walls;
	get_model()->walls->next = wall;
	get_model()->walls = wall;
	return (ret_wall);
}

static void		add_hallway(t_wall *origin, t_wall *virtual)
{
	t_wall	*first;
	t_wall	*visual;
	int		prev_room_wall_count;

	prev_room_wall_count = get_model()->wall_count;
	
	// add walls from current id, like in polydraw

	// first wall becomes the wall that already was (WTAW)
	visual = make_wall(origin->end, origin->start, 0);
	first = visual;
	wall_to_buffer(visual, editor_back_buffer()->buff, COLOR_SELECTION_VECTOR);
	// second wall becomes from the (WTAW).start->virtual.start
	visual = make_wall(origin->start, virtual->start, 0);
	wall_to_buffer(visual, editor_back_buffer()->buff, COLOR_SELECTION_VECTOR);
	// third wall is the virtual wall
	visual = make_wall(virtual->start, virtual->end, 0);
	wall_to_buffer(visual, editor_back_buffer()->buff, COLOR_SELECTION_VECTOR);
	// fourth wall becomes from virtual.end->(WTAW).end
	visual = make_wall(virtual->end, origin->end, 0);
	wall_to_buffer(visual, editor_back_buffer()->buff, COLOR_SELECTION_VECTOR);

	editor_back_buffer()->rendering_on = 1;

	record_room(get_model(), first, prev_room_wall_count);
	puts("TADAA~~!!");
}

static void		select_hinge(int x, int y)
{
	t_wall*		hinge;
	int			hinge_mirror_x;
	int			hinge_mirror_y;
	t_xy		result;

	select_logic()->adding_slope = 1;
	select_logic()->virtual_slope_wall.start.x = x;
	select_logic()->virtual_slope_wall.start.y = y;
	hinge = wall_by_id(select_logic()->selected_wall_id);
	hinge_mirror_x = hinge->end.x - hinge->start.x;
	hinge_mirror_y = hinge->end.y - hinge->start.y;
	result = hinge_algorithm(vec2(x,y), hinge, hinge_mirror_x, hinge_mirror_y);
	select_logic()->virtual_slope_wall.start.x = result.x;
	select_logic()->virtual_slope_wall.start.y = result.y;
	select_logic()->virtual_slope_wall.end.x = result.x + hinge_mirror_x;
	select_logic()->virtual_slope_wall.end.y = result.y + hinge_mirror_y;
	add_hallway(wall_by_id(select_logic()->selected_wall_id), &select_logic()->virtual_slope_wall);
	wall_to_buffer(&select_logic()->virtual_slope_wall, editor_back_buffer()->buff, COLOR_SELECTION_WALL);
	editor_back_buffer()->rendering_on = 1;
	draw_box(result, 3, COLOR_SELECTION_VECTOR, editor_back_buffer()->buff);
}

void 			select_left_click(int x, int y)
{
	t_point		rel;

	rel = relative_position(x, y, get_state());
	select_room(rel.x, rel.y, NEXT_WALL);
}

void 			select_right_click(int x, int y)
{
	t_point		rel;

	rel = relative_position(x, y, get_state());
	select_room(rel.x, rel.y, PREV_WALL);
}

void 			select_middle_click(int x, int y)
{
	t_point		rel;

	rel = relative_position(x, y, get_state());
	select_hinge(rel.x, rel.y);
}