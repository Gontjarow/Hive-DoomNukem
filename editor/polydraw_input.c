#include "doom-nukem.h"

#include <pthread.h>

void check_any_magnet_hits(int x, int y, t_model *mdl)
{
    int wc;
    t_wall *wall;

    wc = mdl->wall_count;
    wall = mdl->wall_first;
    if (wall == NULL || polydraw_status()->thread_target_id == -1)
        return ;
    // Here, check if still hitting a previously found magnet area
    if (polydraw_status()->thread_hit)
    {
        if (abs(polydraw_status()->thread_x - x) < 15 && abs(polydraw_status()->thread_y - y) < 15)
            return ;
        // If no longer hitting, start looking for hits again
        polydraw_status()->thread_hit = 0;
    }
    // Loop through all the walls to search for hits
    polydraw_status()->thread_permission = 0;
    polydraw_status()->thread_color = 0xffff0000;
    while (wc--)
    {
        if (abs(wall->start.x - x) < 15 && abs(wall->start.y - y) < 15)
        {
                //ft_putendl("Magnet hit within 15 units for a starting point of a wall.");
            polydraw_status()->thread_x = wall->start.x;
            polydraw_status()->thread_y = wall->start.y;
            polydraw_status()->thread_hit = 1;
            if (wall->id == polydraw_status()->thread_target_id)
            {
                polydraw_status()->thread_permission = 1;
                polydraw_status()->thread_color = 0xff00ff00;
            }
            return ;
        }
        else if (abs(wall->end.x - x) < 15 && abs(wall->end.y - y) < 15)
        {
                //ft_putendl("Magnet hit within 15 units for a ending point of a wall.");
            polydraw_status()->thread_x = wall->end.x;
            polydraw_status()->thread_y = wall->end.y;
            polydraw_status()->thread_hit = 1;
            return ;
        }
        // For segmentation fault avoidance, abort mission if the phase changes
        if (polydraw_status()->phase != 1)
            return ;
        wall = wall->next;
    }
}

void magnet_test(void* argv) {
    t_status    *status;
    t_model     *mdl;
    int         x;
    int         y;

    status = (t_status*)argv;
    mdl = get_model();
    while (status->phase == 1)
    {
        x = status->motion_x;
        y = status->motion_y;
            //printf("Magnet testing for %d, %d [status->phase = %d]\n", x, y, status->phase);
        check_any_magnet_hits(x, y, mdl);
    }
    status->job_running = 0;
}

void 		polydraw_mouse_motion(int x, int y)
{
	t_linedraw          *data;
	static pthread_t    magnet_thread;
	static int          updated_magnet_hover = 0;

	// Check if polydraw_status->phase is not polydraw_continue() to early exit!
	if (polydraw_status()->phase != 1)
		return ;
	// When the phase is polydraw_continue(), draw a line at the front buffer
	data = polydraw_status()->data;
	assert(data->drawing_underway);
	assert(data->draw_from_x >= 0 && data->draw_from_x < EDT_WIN_WIDTH &&
		   data->draw_from_y >= 0 && data->draw_from_y < EDT_WIN_HEIGHT);
	// When the phase is polydraw_continue(), check for magnetization
    polydraw_status()->motion_x = x;
    polydraw_status()->motion_y = y;
	if (!polydraw_status()->job_running)
	{
        polydraw_status()->job_running = 1;
	    pthread_create(&magnet_thread, NULL, magnet_test, (void*)polydraw_status());
    }
    if (polydraw_status()->thread_hit && !updated_magnet_hover)
    {
        data->draw_to_x = polydraw_status()->thread_x;
        data->draw_to_y = polydraw_status()->thread_y;
        wipe_editor_front_buffer(0xff000000);
        linedraw_to_buffer(data, editor_front_buffer()->buff, 0xffffffff);
        //editor_front_buffer_hover_circle(polydraw_status()->thread_x, polydraw_status()->thread_y, 10, 0xff0000ff);
        circle_to_buffer(editor_front_buffer()->buff, (t_point){data->draw_to_x, data->draw_to_y}, 10, polydraw_status()->thread_color);
        editor_front_buffer()->rendering_on = 1;
        updated_magnet_hover = 1;
        return ;
    }
    else if (!polydraw_status()->thread_hit)
    {
        data->draw_to_x = x;
        data->draw_to_y = y;
        wipe_editor_front_buffer(0xff000000);
        linedraw_to_buffer(data, editor_front_buffer()->buff, 0xffffffff);
        updated_magnet_hover = 0;
        editor_front_buffer()->rendering_on = 1;
    }
	//editor_front_buffer()->rendering_on = 1;
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

