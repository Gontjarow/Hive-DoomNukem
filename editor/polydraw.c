#include "doom-nukem.h"

void			polydraw_activate(t_state *state)
{
    state->job_running = 0;
    state->job_abort = 0;
    state->thread_hit = 0;
    state->thread_color = 0xffff0000;
    state->thread_permission = 0;
    state->thread_target_id = -1;
    polydraw_change_zoom(state);
}

void			polydraw_deactivate(t_state *state)
{
	if (polydraw_status()->phase != 0)
		polydraw_status()->reset(polydraw_status());
	wipe_editor_back_buffer(0xff000000);
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, COLOR_LINE);
}

void 			polydraw_change_zoom(t_state *state)
{
	if (polydraw_status()->phase != 0)
		polydraw_status()->reset(polydraw_status());
	redraw_editor_to_backbuffer(COLOR_LINE);
}