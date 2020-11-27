#include "doom-nukem.h"

t_status		*polydraw_status(void)
{
    static		t_status *status = NULL;

    if (!status)
    {
        status = (t_status*)malloc(sizeof(t_status));
        if (!status)
            ft_die("Fatal error: Could not malloc status for polydraw at polydraw_status");
        status->data = (t_linedraw*)malloc(sizeof(t_linedraw));
        if (!status->data)
            ft_die("Fatal error: Could not malloc data for polydraw at polydraw_status");
        init_linedraw_data(status->data);
        status->phase = 0;
        status->phase_count = 3;
        status->phases = (gui_action*)malloc(sizeof(gui_action) * status->phase_count);
        status->phases[0] = polydraw_start;
        status->phases[1] = polydraw_continue;
        status->phases[2] = polydraw_end;
        status->reset = polydraw_reset;
    }
    return (status);
}

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
	static pthread_t    magnet_thread;
	static int          updated_magnet_hover = 0;

	// Check if polydraw_status->phase is not polydraw_continue() to early exit!
	if (polydraw_status()->phase != 1)
		return ;
	// When the phase is polydraw_continue(), draw a line at the front buffer
	data = polydraw_status()->data;
	assert(data->drawing_underway);
	assert(data->draw_from_x >= 0 && data->draw_from_x < EDT_WIN_WIDTH * get_state()->zoom_factor &&
		   data->draw_from_y >= 0 && data->draw_from_y < EDT_WIN_HEIGHT * get_state()->zoom_factor);
	// When the phase is polydraw_continue(), check for magnetization
    polydraw_status()->motion_x = x;
    polydraw_status()->motion_y = y;
	if (!get_state()->job_running)
	{
        get_state()->job_running = 1;
	    pthread_create(&magnet_thread, NULL, magnet_test, (void*)polydraw_status());
    }
    if (get_state()->thread_hit && !updated_magnet_hover)
    {
        data->draw_to_x = get_state()->thread_x; //* get_state()->zoom_factor;
        data->draw_to_y = get_state()->thread_y; //* get_state()->zoom_factor;
        //wipe_editor_front_buffer(0xff000000);
		if (previous_data.drawing_underway)
			careful_linedraw_to_buffer(&previous_data, doom_ptr()->edt->buff, 0xff000000, 0xffffffff);
		previous_data.draw_to_y = data->draw_to_y;
		previous_data.draw_to_x = data->draw_to_x;
		previous_data.draw_from_y = data->draw_from_y;
		previous_data.draw_from_x = data->draw_from_x;
		previous_data.drawing_underway = 1;
        careful_linedraw_to_buffer(data, doom_ptr()->edt->buff, 0xfffffffe, 0xffffffff);
		masked_color = get_state()->thread_color;
        masked_circle_to_buffer(doom_ptr()->edt->buff, (t_point){data->draw_to_x, data->draw_to_y}, 15,
								masked_color, 0xffffffff);
		//linedraw_to_buffer(data, editor_front_buffer()->buff, 0xffffffff);
		//circle_to_buffer(editor_front_buffer()->buff, (t_point){data->draw_to_x, data->draw_to_y}, 15, get_state()->thread_color);
        //editor_front_buffer()->rendering_on = 1;
        updated_magnet_hover = 1;
        return ;
    }
    else if (!get_state()->thread_hit)
    {
    	// Regular case. Draw a preview line on the screens front buffer.
        data->draw_to_x = x;
        data->draw_to_y = y;
        //wipe_editor_front_buffer(0xff000000);
        //linedraw_to_buffer(data, editor_front_buffer()->buff, 0xffffffff);
        //editor_front_buffer()->rendering_on = 1;
		if (previous_data.drawing_underway)
			careful_linedraw_to_buffer(&previous_data, doom_ptr()->edt->buff, 0xff000000, 0xffffffff);
		if (updated_magnet_hover)
			unmasked_circle_to_buffer(doom_ptr()->edt->buff, (t_point){previous_data.draw_to_x, previous_data.draw_to_y}, 15,
									  0xffffffff, masked_color);
		previous_data.draw_to_y = data->draw_to_y;
		previous_data.draw_to_x = data->draw_to_x;
		previous_data.draw_from_y = data->draw_from_y;
		previous_data.draw_from_x = data->draw_from_x;
		previous_data.drawing_underway = 1;
		careful_linedraw_to_buffer(data, doom_ptr()->edt->buff, 0xfffffffe, 0xffffffff);
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