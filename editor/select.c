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