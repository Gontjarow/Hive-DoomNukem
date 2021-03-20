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

static void 	select_room(int x, int y)
{
	select_logic()->selected_room_id = room_id_from_polymap(get_model()->poly_map, x, y);
	select_change_zoom(get_state());
}

void 			select_left_click(int x, int y)
{
	t_point		rel;

	rel = relative_position(x, y, get_state());
	select_room(rel.x, rel.y);
}

void 			select_right_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		puts("Select right click..!");
}

void 			select_middle_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		puts("Select middle click..!");
}