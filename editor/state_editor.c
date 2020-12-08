#include "doom-nukem.h"

/* trigger_protection() is a notifier function called by set_pixel_safe() when
 * overwriting to a buffer over a set_protected_color() has occured. Triggers
 * are accumulated by the calls 'trigger_protection(0)'. Decoupled from this,
 * elsewhere in the code a has_protection = 1 enabled eg. mode_polydraw()
 * can implement a periodic or event based check wheter overwriting happened
 * by invoking 'trigger_protection(1)'. If yes, this protection ATM (stc)
 * calls a clean slate for editor back buffer by a wipe and a walls redraw  */
// TODO: Utilize trigger_protection to detect illegal wall draws or room draws
//  (creation of intersection (overlapping) walls should not be possible

void 				trigger_protection(int clear)
{
	static int triggers = 0;

	if (clear)
	{
		if (triggers == 0)
			return ;
		triggers = 0;
		//ft_putendl("Triggered clearance commisioned.");
		wipe_editor_back_buffer(0xff000000);
		x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, 0xffffffff);
		return ;
	}
	triggers++;
}

void		 		edt_swap_mode(t_state *state)
{
	//ft_putendl("Deactivating GUI mode");
	state->gui->deactivate(state);
	if (state->gui == mode_polydraw())
		state->gui = mode_planting();
	else if (state->gui == mode_planting())
		state->gui = mode_polydraw();
	//ft_putendl("Activating GUI mode");
	state->gui->activate(state);
}

void				edt_outward_zoom(void)
{
	//printf("Zoomed editor outward\n");
	if (get_state()->zoom_factor < 4)
		get_state()->zoom_factor *= 2;
	get_state()->confine_skip = 0;
	confine_scroll(get_state());
	get_state()->confine_skip = 1;
	get_state()->gui->change_zoom(get_state());
}

void				edt_inward_zoom(void)
{
	//printf("Zoomed editor inward\n");
	if (get_state()->zoom_factor > 1)
		get_state()->zoom_factor /= 2;
	get_state()->confine_skip = 0;
	confine_scroll(get_state());
	get_state()->confine_skip = 1;
	get_state()->gui->change_zoom(get_state());
}

/* get_state returns the overall "State" of the editor. In the field state->gui,
 * you can assign a GUI mode, that are retrieved from their Singleton functions.
 * For example, for the get_state() initial state, state->gui = mode_polydraw();
 * assigns the GUI's action state to polydraw mode, which is a mode that draws
 * polygons using mouse clicks. By following the mode_polydraw() trail, you can
 * explore and find out how the polydraw_* functions are organized and coded. */

t_state				*get_state(void)
{
	static t_state	*state = NULL;

	if (!state)
	{
		state = (t_state*)malloc(sizeof(t_state));
		if (!state)
			ft_die("Fatal error: Could not malloc state struct at get_state");
		state->gui = mode_planting();//Usually mode_polydraw()
		state->zoom_factor = 1;
		state->scroll_x = 0;
		state->scroll_y = 0;
		state->confine_skip = 1;
		state->gui->activate(state);
		//print_mode_info(state->gui);
		//draw_scroll_bars_to_backbuffer(state);
	}
	//printf("States address returned: %p\n", (void*)state);
	return (state);
}

