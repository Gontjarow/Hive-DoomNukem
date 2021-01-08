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
		//puts("Activating planting");
}

void			planting_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	get_state()->cooldown = 0;
	planting_change_zoom(state);
		//puts("Deactivating planting");
}

void			planting_change_zoom(t_state *state)
{
	redraw_editor_to_backbuffer(COLOR_LINE);
}
