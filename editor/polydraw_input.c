#include "doom-nukem.h"

// Cleared TO-DO Create a direct draw system for the preview lines.
//  	Stage 1: For each line, have a) "draw" and b) "undraw" // DONE
//		Stage 2: draw->undraw->draw-> for the preview lines directly in buffer // DONE
//		Stage 3: This will cause overwritten data to be lost. "Solution": Disable overwrite "draw" and "undraw" // DONE
//		Stage 4: Detect bug in the cycle where the magnetization kicks in and causes undraw to be lost // DONE
//		Stage 5: Create masked_circle_to_buffer function to replace the default version from the code // DONE
//		Stage 6: Cleanup the additional code, update headers, and test for bugs // DONE
//		Stage 7: Commit and move to the next task // DONE

void 		polydraw_mouse_motion(int x, int y)
{
	t_linedraw          *data;
	static t_linedraw	previous_data = { 0 };
	static uint32_t 	masked_color = 0;
	static uint32_t 	avoid[3] = { COLOR_LINE, COLOR_PLAYER, COLOR_ENEMY };
	static pthread_t    magnet_thread;
	static int          updated_magnet_hover = 0;
	int					tmp_scroll_x;
	int					tmp_scroll_y;

	// Check if polydraw_status->phase is not polydraw_continue() to early exit!
	if (polydraw_status()->phase != 1)
		return ;
	data = polydraw_status()->data;
	assert(data->drawing_underway);
	// When the phase is polydraw_continue(), check for magnetization
    polydraw_status()->motion_x = x;
    polydraw_status()->motion_y = y;
    // Save local copies of scroll_x and scroll_y for adjusting the screen drawing code with
	tmp_scroll_x = get_state()->scroll_x;
	tmp_scroll_y = get_state()->scroll_y;
	if (!get_state()->job_running)
	{
        get_state()->job_running = 1;
	    pthread_create(&magnet_thread, NULL, magnet_test, (void*)polydraw_status());
    }
    if (get_state()->thread_hit && !updated_magnet_hover)
    {
		// This block of code is for when the magnetization has hitted and we draw the
		// preview line to magnetized target point, when user aims to end polydrawing
        data->draw_to_x = get_state()->thread_x;
        data->draw_to_y = get_state()->thread_y;
		if (previous_data.drawing_underway)
			careful_linedraw_to_buffer(&previous_data, doom_ptr()->edt->buff, 0xff000000, &avoid);
		data->draw_from_x -= tmp_scroll_x;
		data->draw_from_y -= tmp_scroll_y;
		previous_data.draw_to_y = data->draw_to_y;
		previous_data.draw_to_x = data->draw_to_x;
		previous_data.draw_from_y = data->draw_from_y;
		previous_data.draw_from_x = data->draw_from_x;
		previous_data.drawing_underway = 1;
        careful_linedraw_to_buffer(data, doom_ptr()->edt->buff, 0xfffffffe, &avoid);
		masked_color = get_state()->thread_color;
        masked_circle_to_buffer(doom_ptr()->edt->buff, (t_point){data->draw_to_x, data->draw_to_y}, 15,
								masked_color, &avoid);
		data->draw_from_x += tmp_scroll_x;
		data->draw_from_y += tmp_scroll_y;
        updated_magnet_hover = 1;
    }
    else if (!get_state()->thread_hit)
    {
    	// Regular case. Draw a preview line to mouse cursors position on the screen
        data->draw_to_x = x;
        data->draw_to_y = y;
		if (previous_data.drawing_underway)
			careful_linedraw_to_buffer(&previous_data, doom_ptr()->edt->buff, 0xff000000, &avoid);
		if (updated_magnet_hover)
			unmasked_circle_to_buffer(doom_ptr()->edt->buff, (t_point){previous_data.draw_to_x, previous_data.draw_to_y}, 15,
									  0xffffffff, masked_color);
		data->draw_from_x -= tmp_scroll_x;
		data->draw_from_y -= tmp_scroll_y;
		previous_data.draw_to_y = data->draw_to_y;
		previous_data.draw_to_x = data->draw_to_x;
		previous_data.draw_from_y = data->draw_from_y;
		previous_data.draw_from_x = data->draw_from_x;
		previous_data.drawing_underway = 1;
		careful_linedraw_to_buffer(data, doom_ptr()->edt->buff, 0xfffffffe, &avoid);
		data->draw_from_x += tmp_scroll_x;
		data->draw_from_y += tmp_scroll_y;
		updated_magnet_hover = 0;
    }
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
	// TODO: THIS FAILS IF RIGHT CLICKING ON GREEN MAGNETIZED AREA
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