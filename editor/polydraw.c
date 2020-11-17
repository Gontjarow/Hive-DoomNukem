#include "doom-nukem.h"

void 			polydraw_start(t_status *status)
{
	t_linedraw      *data;

	data = (t_linedraw*)status->data;
	assert(status->phase == 0);
	data->origin_id = get_model()->wall_count;
		//printf("Data->origin_id SET = %d\n", data->origin_id);
	data->drawing_underway = 1;
	data->draw_from_x = status->click_x;
	data->draw_from_y = status->click_y;
	status->phase = 1;
	status->thread_target_id = data->origin_id;
		//ft_putendl("Polydraw start");
}

void 			polydraw_continue(t_status *status)
{
	t_linedraw      *data;

	data = (t_linedraw*)status->data;
	assert(status->phase == 1);
	assert(data->drawing_underway == 1);
	if (status->thread_hit)
    {
        if (!status->thread_permission)
            return ;
	    data->draw_to_x = status->thread_x;
	    data->draw_to_y = status->thread_y;
	    status->phase++;
	    assert(status->phase == 2);
    }
	else
    {
        data->draw_to_x = status->click_x;
        data->draw_to_y = status->click_y;
    }
	linedraw_to_wall(data);
	linedraw_to_buffer_safe(data, editor_back_buffer()->buff, 0xffffffff);
	editor_back_buffer()->rendering_on = 1;
	*data = (t_linedraw){data->origin_id, 1, status->click_x, status->click_y, 0};
	// Invokation of the ending phase, polydraw_end, if status->thread_hit was TRUE, do polydraw_end (phase 2)
	if (status->phase == 2)
		status->phases[status->phase](status);
}

void 			polydraw_end(t_status *status)
{
	t_linedraw      *data;

	data = (t_linedraw*)status->data;
	assert(status->phase == 2);
	status->phase = 0;
	status->thread_target_id = -1;
	*data = (t_linedraw){0};
    wipe_editor_front_buffer(0xff000000);
		//ft_putendl("Polydraw end");
}

void			polydraw_reset(t_status *status)
{
	t_linedraw	*data;
	t_wall		*wall;
	t_wall		*wipe;
	int			wc;

	wipe_editor_front_buffer(0xff000000);
	data = (t_linedraw*)status->data;
	wc = data->origin_id;
		//printf("Data->origin_id GET = %d\n", data->origin_id);
	wall = get_model()->wall_first;
	while (wc--)
		wall = wall->next;
	wc = get_model()->wall_count - data->origin_id;
	if (!wc)
		return;
	while (wc--)
	{
		wall_to_buffer(wall, editor_back_buffer()->buff, 0xff000000);
		wipe = wall;
		wall = wall->next;
		free(wipe);
		get_model()->wall_count--;
	}
	relink_model_walls(wall);
	trigger_protection(1);
	// Trigger polydraw_end for proper cleanup
    status->phase = 2;
    status->phases[status->phase](status);
}


static void 		polydraw_reset_commented_unupdated_out_of_sync_with_above(t_status *status)
{
	t_linedraw	*data;
	t_wall		*wall;
	t_wall		*wipe;
	int			wc;

	//ft_putendl("Polydraw reset");
	status->phase = 0;
	wipe_editor_front_buffer(0xff000000);
	data = (t_linedraw *) status->data;
	wc = data->origin_id;
	wall = get_model()->wall_first;
	// Iterate to a wall that marks the location where the last polydraw sequence started from
	while (wc--)
		wall = wall->next;
	// Calculate how many deletions will be done in the resetation, updating model wall_count
	wc = get_model()->wall_count - data->origin_id;
	*data = (t_linedraw){0};
	//debug_model_walls();
	// Prevent deletion sequence by early exit if no deletions are to be made
	if (!wc)
		return;
	//printf("Wallcount during reset is %d\n", get_model()->wall_count);
	// Iterate the deletions, clearing the wall from the back buffer and data
	while (wc--)
	{
		wall_to_buffer(wall, editor_back_buffer()->buff, 0xff000000);
		wipe = wall;
		wall = wall->next;
		free(wipe);
		//ft_putendl("Deleted a wall data.");
		get_model()->wall_count--;
	}
	// Clearing the linked list data nodes, then relinking up the saved current node
	//printf("Wallcount after deletion sequence of model walls is %d\n", get_model()->wall_count);
	relink_model_walls(wall);
	//debug_model_walls();
}

t_status		*polydraw_status()
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
		status->job_running = 0;
		status->thread_hit = 0;
        status->thread_color = 0xffff0000;
		status->thread_permission = 0;
		status->thread_target_id = -1;
	}
	return (status);
}