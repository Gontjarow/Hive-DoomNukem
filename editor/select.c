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
		logic->selected_portal_id = -1;
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
	int			*adjust;
	int			*diff;

	if (get_state()->gui != mode_select())
		return ;
	if (select_logic()->selected_room_id == -1)
		return ;
	room = room_by_id(select_logic()->selected_room_id);
	adjust = &room->roof_height;
	if (room->adjusting_opposite)
		adjust = &room->slope_roof;
	diff = &room->floor_height;
	if (room->adjusting_opposite)
		diff = &room->slope_floor;
	*adjust += dir * HEIGHT_STEPPING;
	if (*adjust < ROOF_MIN)
		*adjust = ROOF_MIN;
	if (*adjust > ROOF_MAX)
		*adjust = ROOF_MAX;
	if (*adjust - *diff < FLOOR_ROOF_DIFF_LIMIT)
		*adjust = *diff + FLOOR_ROOF_DIFF_LIMIT;
	select_logic()->last_floor = *diff;
	select_logic()->last_roof = *adjust;
	select_change_zoom(get_state());
	get_state()->saving_choice = 0;
}

void 			select_floor(int dir)
{
	t_room		*room;
	int			*adjust;
	int			*diff;

	if (get_state()->gui != mode_select())
		return ;
	if (select_logic()->selected_room_id == -1)
		return ;
	room = room_by_id(select_logic()->selected_room_id);
	adjust = &room->floor_height;
	if (room->adjusting_opposite)
		adjust = &room->slope_floor;
	diff = &room->roof_height;
	if (room->adjusting_opposite)
		diff = &room->slope_roof;
	*adjust += dir * HEIGHT_STEPPING;
	if (*adjust < FLOOR_MIN)
		*adjust = FLOOR_MIN;
	if (*adjust > FLOOR_MAX)
		*adjust = FLOOR_MAX;
	if (*diff - *adjust < FLOOR_ROOF_DIFF_LIMIT)
		*adjust = *diff - FLOOR_ROOF_DIFF_LIMIT;
	select_logic()->last_floor = *adjust;
	select_logic()->last_roof = *diff;
	select_change_zoom(get_state());
	get_state()->saving_choice = 0;
}

void 			select_delete_room(void)
{
	if (select_logic()->selected_room_id == -1)
		return;
	delete_portals_by_room(room_by_id(select_logic()->selected_room_id), get_model());
	delete_room(room_by_id(select_logic()->selected_room_id), room_by_id(select_logic()->selected_room_id)->wall_count, get_model());
	select_logic()->selected_room_id = -1;
	select_change_zoom(get_state());
	get_state()->saving_choice = 0;
}

static int		selected_portal(int wall_id)
{
	t_wall		*wall;
	t_wall		*portal;
	int			pc;

	wall = wall_by_id(wall_id);
	pc = get_model()->portal_count;
	portal = get_model()->portal_first;
	while (pc--)
	{
		if (wall->start.x == portal->start.x && wall->end.x == portal->end.x && wall->start.y == portal->start.y && wall->end.y == portal->end.y)
			return (portal->id);
		else if (wall->start.x == portal->end.x && wall->end.x == portal->start.x && wall->start.y == portal->end.y && wall->end.y == portal->start.y)
			return (portal->id);
		portal = portal->next;
	}
	return (-1);
}

static void		select_wall(int room_id, int cycle_dir)
{
	t_room		*room;
	int			selection_is_portal;
	
	select_logic()->selected_wall_id += cycle_dir;
	select_logic()->selected_portal_id = -1;
	room = room_by_id(room_id);
	if (room->first_wall_id > select_logic()->selected_wall_id)
	{
		select_logic()->selected_wall_id = room->first_wall_id + room->wall_count - 1;
	}
	else if (select_logic()->selected_wall_id >= room->first_wall_id + room->wall_count)
	{
		select_logic()->selected_wall_id = room->first_wall_id;
	}
	// Check if selected_wall_id matches with a portal, if so, flip a boolean
	if (select_logic()->selected_wall_id != -1)
	{
		selection_is_portal = selected_portal(select_logic()->selected_wall_id);
		if (selection_is_portal != -1)
		{
			select_logic()->selected_portal_id = selection_is_portal;
			//puts("SELECTION IS PORTAL!!!!");
		}
	}	
	select_change_zoom(get_state());
}

static void 	select_room(int x, int y, int cycle_dir)
{
	int clicked_room_id;

	clicked_room_id = room_id_from_polymap(get_model()->poly_map, x, y);
	if (clicked_room_id == -1)
	{
		select_logic()->selected_portal_id = -1;
		select_logic()->selected_room_id = clicked_room_id;
		return ;
	}
	if (room_by_id(clicked_room_id)->is_hallway)
	{
		select_logic()->selected_room_id = clicked_room_id;
		room_by_id(clicked_room_id)->adjusting_opposite = !(room_by_id(clicked_room_id)->adjusting_opposite);
		if (room_by_id(clicked_room_id)->adjusting_opposite)
			select_logic()->selected_wall_id = room_by_id(clicked_room_id)->first_wall_id + 1;
		else
			select_logic()->selected_wall_id = room_by_id(clicked_room_id)->first_wall_id + 3;
		select_change_zoom(get_state());
		return ;
	}
	if (clicked_room_id != select_logic()->selected_room_id)
		select_logic()->selected_portal_id = -1;	
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

static void		wipe_hallway(int origin_id)
{
	t_wall		*wall;
	t_wall		*wipe;
	int			wc;

		int before = get_model()->wall_count;
	wc = origin_id;
	wall = get_model()->wall_first;
	while (wc--)
		wall = wall->next;
	wc = get_model()->wall_count - origin_id;
	if (!wc)
		return;
	while (wc--)
	{
		wipe = wall;
		wall = wall->next;
		free(wipe);
		get_model()->wall_count--;
	}
	relink_model_walls(wall);
		int after = get_model()->wall_count;
		printf("wipe hallway: wall_count before [%d] wall_count after [%d]\n", before, after);
}

static void		add_hallway(t_wall *origin, t_wall *virtual)
{
	t_wall	*first;
	t_wall	*new_wall;
	int		prev_room_wall_count;

	prev_room_wall_count = get_model()->wall_count;	
	// add walls from current id, like in polydraw
	// 1st wall becomes from the (WTAW).start->virtual.start
	new_wall = make_wall(origin->start, virtual->start, 0);
	first = new_wall;
	// 2nd wall is the virtual wall
	new_wall = make_wall(virtual->start, virtual->end, 0);
	// 3rd wall becomes from virtual.end->(WTAW).end
	new_wall = make_wall(virtual->end, origin->end, 0);
	// 4th wall becomes the wall that already was (WTAW)
	new_wall = make_wall(origin->end, origin->start, 0);	
	if (record_room(get_model(), first, prev_room_wall_count))
	{
		record_portal(get_model(), new_wall);
		room_by_id(get_model()->room_count - 1)->is_hallway = 1;
		room_by_id(get_model()->room_count - 1)->slope_floor = room_by_id(get_model()->room_count - 1)->floor_height;
		room_by_id(get_model()->room_count - 1)->slope_roof = room_by_id(get_model()->room_count - 1)->roof_height;
	}
	else
		wipe_hallway(first->id);
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
	select_change_zoom(get_state());
}