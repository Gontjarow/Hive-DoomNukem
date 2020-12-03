#include "doom-nukem.h"

void			planting_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	planting_change_zoom(state);
}

void			planting_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	planting_change_zoom(state);
}

void			planting_change_zoom(t_state *state)
{
	wipe_editor_back_buffer(0xff000000);
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, 0xffffffff);
	print_mode_info(state->gui);
	draw_scroll_bars_to_backbuffer(state);
}

void 			planting_mouse_motion(int x, int y)
{
	 x = y;
	 return ;
}

void 			planting_left_click(int x, int y)
{
	x = y;
	return ;
}

void 			planting_right_click(int x, int y)
{
	x = y;
	return ;
}

void 			planting_middle_click(int x, int y)
{
	x = y;
	return ;
}