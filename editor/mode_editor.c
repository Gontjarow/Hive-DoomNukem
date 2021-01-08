#include "doom-nukem.h"

t_gui	*mode_select(void)
{
	static t_gui	*select = NULL;

	if (!select)
	{
		select = (t_gui*)malloc(sizeof(t_gui));
		if (!select)
			ft_die("Fatal error: Could not malloc t_gui at mode_select.");
		// TODO FLESH OUT
		select->activate = select_activate;
		select->deactivate = select_deactivate;
		select->change_zoom = select_change_zoom;
		select->left_click = select_left_click;
		select->right_click = select_right_click;
		select->middle_click = select_middle_click;
		select->motion = select_mouse_motion;
		select->has_motion = 1;
		set_protected_color(0xffffffff);
	}
	return (select);
}

t_gui	*mode_pickups(void)
{
	static t_gui	*pickups = NULL;

	if (!pickups)
	{
		pickups = (t_gui*)malloc(sizeof(t_gui));
		if (!pickups)
			ft_die("Fatal error: Could not malloc t_gui at mode_pickups.");
		pickups->activate = pickups_activate;
		pickups->deactivate = pickups_deactivate;
		pickups->change_zoom = pickups_change_zoom;
		pickups->left_click = pickups_left_click;
		pickups->right_click = pickups_right_click;
		pickups->middle_click = pickups_middle_click;
		pickups->motion = pickups_mouse_motion;
		pickups->has_motion = 1;
		set_protected_color(0xffffffff);
	}
	return (pickups);
}

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
