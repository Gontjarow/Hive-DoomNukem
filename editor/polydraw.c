#include "doom-nukem.h"

void 			polydraw_start(t_status *status)
{
	t_linedraw      *data;

	data = (t_linedraw*)status->data;
	assert(status->phase == 0);
	data->origin_id = get_model()->wall_count;
		//printf("Data->origin_id SET = %d\n", data->origin_id);
	data->drawing_underway = 1;
	data->draw_from_x = status->click_x * get_state()->zoom_factor;
	data->draw_from_y = status->click_y * get_state()->zoom_factor;
	status->phase = 1;
	get_state()->thread_target_id = data->origin_id;
		//ft_putendl("Polydraw start");
}

void 			polydraw_continue(t_status *status)
{
	t_linedraw      *data;

	data = (t_linedraw*)status->data;
	assert(status->phase == 1);
	assert(data->drawing_underway == 1);

	if (get_state()->thread_hit)
    {
        if (!get_state()->thread_permission)
            return ;
	    data->draw_to_x = get_state()->thread_x * get_state()->zoom_factor;
	    data->draw_to_y = get_state()->thread_y * get_state()->zoom_factor;
	    status->phase++;
	    assert(status->phase == 2);
    }
	else
    {
        data->draw_to_x = status->click_x * get_state()->zoom_factor;
        data->draw_to_y = status->click_y * get_state()->zoom_factor;
    }
	wall_to_buffer(linedraw_to_wall(data), editor_back_buffer()->buff, 0xffffffff);
		//printf("With zoom of %d: linedraw (%d, %d) to (%d, %d)\n", get_state()->zoom_factor,
		//data->draw_from_x, data->draw_from_y, data->draw_to_x, data->draw_to_y);
		//debug_model_walls();
	editor_back_buffer()->rendering_on = 1;
	*data = (t_linedraw){data->origin_id, 1, status->click_x * get_state()->zoom_factor,
					  status->click_y * get_state()->zoom_factor, 0};
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
	get_state()->thread_target_id = -1;
	get_state()->job_abort = 0;
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
	get_state()->job_abort = 1;
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

void			polydraw_activate(t_state *state)
{
    state->job_running = 0;
    state->job_abort = 0;
    state->thread_hit = 0;
    state->thread_color = 0xffff0000;
    state->thread_permission = 0;
    state->thread_target_id = -1;
}

void 			polydraw_change_zoom(t_state *state)
{
	if (polydraw_status()->phase != 0)
		polydraw_status()->reset;
	// Simplify debug with incomplete code
		state->scroll_x = 0;
		state->scroll_y = 0;
	wipe_editor_back_buffer(0xff000000);
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, 0xffffffff);
	print_mode_info(state->gui);
}