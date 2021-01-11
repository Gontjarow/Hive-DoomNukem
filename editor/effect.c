#include "doom-nukem.h"

void			effect_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	select_change_zoom(state);
		//puts("Activating effect");
}

void			effect_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	select_change_zoom(state);
		//puts("Deactivating effect");
}

void			effect_change_zoom(t_state *state)
{
	redraw_editor_to_backbuffer(COLOR_LINE);
}

void 			effect_mouse_motion(int x, int y)
{
	if (x < 0 && y < 0)
		puts("Effect mouse motion..!");
}

void 			effect_left_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		puts("Effect left click..!");
}

void 			effect_right_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		puts("Effect right click..!");
}

void 			effect_middle_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		puts("Effect middle click..!");
}

