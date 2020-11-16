#include "doom-nukem.h"

void 		polydraw_mouse_motion(int x, int y)
{
	t_linedraw      *data;

	// Check if polydraw_status->phase is not polydraw_continue() to early exit!
	if (polydraw_status()->phase != 1)
		return ;
	// When the phase is polydraw_continue(), draw a line at the front buffer
	data = polydraw_status()->data;
	assert(data->drawing_underway);
	assert(data->draw_from_x >= 0 && data->draw_from_x < EDT_WIN_WIDTH &&
		   data->draw_from_y >= 0 && data->draw_from_y < EDT_WIN_HEIGHT);
	data->draw_to_x = x;
	data->draw_to_y = y;
	flood_buffer(editor_front_buffer()->buff, 0xff000000);
	linedraw_to_buffer(data, editor_front_buffer()->buff, 0xffffffff);
	editor_front_buffer()->rendering_on = 1;
}

void 		polydraw_left_click(int x, int y)
{
	//ft_putendl("Polydraw left_clicked");
	t_status		*status;

	status = polydraw_status();
	status->click_x = x;
	status->click_y = y;
	status->phases[status->phase](status);
}

void 		polydraw_right_click(int x, int y)
{
	//ft_putendl("Polydraw right_clicked");
	t_status		*status;

	status = polydraw_status();
	// Check if polydraw_status->phase is not polydraw_continue() to early exit!
	if (status->phase != 1)
		return ;
	// When the phase is polydraw_continue(), proceed a cycle of polydraw_continue() followed
	// by the polydraw_end(). Invoke them in order with the "reflection", to maintain
	// coherence and code style and logic unity with polydraw_left_click function.
	status->click_x = x;
	status->click_y = y;
	// This invokes polydraw_continue()
	status->phases[status->phase](status);
	status->phase++;
	assert(status->phase == 2);
	// This invokes polydraw_end()
	status->phases[status->phase](status);
}

void 		polydraw_middle_click(int x, int y)
{
	t_status		*status;

	status = polydraw_status();
	// Check if polydraw_status->phase is not polydraw_continue() to early exit!
	if (status->phase == 0)
		return ;
	//ft_putendl("Polydraw middle_clicked");
	// Have mapped polydraw_status->reset (polydraw_reset) to polydraw_middle_click here.
	polydraw_status()->reset(polydraw_status());
}

