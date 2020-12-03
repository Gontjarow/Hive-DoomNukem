#include "doom-nukem.h"

t_gui	*mode_planting(void)
{
	static t_gui	*planting = NULL;

	if (!planting)
	{
		planting = (t_gui*)malloc(sizeof(t_gui));
		if (!planting)
			ft_die("Fatal error: Could not malloc t_gui at mode_planting.");
		planting->activate = planting_activate;
		planting->deactivate = planting_deactivate;
		planting->change_zoom = planting_change_zoom;
		planting->left_click = planting_left_click;
		planting->right_click = planting_right_click;
		planting->middle_click = planting_middle_click;
		planting->motion = planting_mouse_motion;
		planting->has_motion = 1;
		set_protected_color(0xffffffff);
	}
	return (planting);
}

/* mode_polydraw() returns as a a singleton the properties for the polydrawing mode.
 * It has a mouse motion function enabled, and the clicks. It protects overwriting
 * to white color and redraws when necessary (if overwriting has occured).
 * It is a t_gui struct that lives in get_state()->gui. The polydraw_* functions
 * are elsewhere and live in polydraw_status(), that handles the Status of the mode
 * during its activation. When polydrawing ends (is replaced by another t_gui mode
 * or the editor exits) it should know how to reset and clean itself up by
 * polydraw_status()->reset(polydraw_status());
 * */

t_gui	*mode_polydraw(void)
{
	static t_gui	*polydraw = NULL;

	if (!polydraw)
	{
		polydraw = (t_gui*)malloc(sizeof(t_gui));
		if (!polydraw)
			ft_die("Fatal error: Could not malloc t_gui at mode_polydraw.");
		polydraw->activate = polydraw_activate;
		polydraw->deactivate = polydraw_deactivate;
		polydraw->change_zoom = polydraw_change_zoom;
		polydraw->left_click = polydraw_left_click;
		polydraw->right_click = polydraw_right_click;
		polydraw->middle_click = polydraw_middle_click;
		polydraw->motion = polydraw_mouse_motion;
		polydraw->has_motion = 1;
		set_protected_color(0xffffffff);
	}
	return (polydraw);
}
