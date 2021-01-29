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
		status->phases = (status_action*)malloc(sizeof(status_action) * status->phase_count);
		status->phases[0] = polydraw_start;
		status->phases[1] = polydraw_continue;
		status->phases[2] = polydraw_end;
		status->reset = polydraw_reset;
	}
	return (status);
}

void 			polydraw_start(t_status *status)
{
	t_linedraw      *data;
	t_point			conv;
	t_point			detect;

	data = (t_linedraw*)status->data;
	assert(status->phase == 0);

	conv.x = (status->click_x * get_state()->zoom_factor) + get_state()->scroll_x;
	conv.y = (status->click_y * get_state()->zoom_factor) + get_state()->scroll_y;
	/* BLOCK OF CODE FOR PORTALIZATION */
	detect = detect_wall_point(conv.x, conv.y, get_model(), get_state());
	if (detect.x >= 0 && detect.y >= 0)
		conv = detect;
	data->origin_id = get_model()->wall_count;
	//printf("Data->origin_id SET = %d\n", data->origin_id);
	data->drawing_underway = 1;
	data->draw_from_x = conv.x;
	data->draw_from_y = conv.y;
	//printf("scroll x | y: %d | %d\n", get_state()->scroll_x, get_state()->scroll_y);
	//printf("from x | y: %d | %d\n", data->draw_from_x, data->draw_from_y);
	status->phase = 1;
	get_state()->thread_target_id = data->origin_id;
	//ft_putendl("Polydraw start");
}

static int		isolated_portalization_check(t_status *status, int x, int y)
{
	t_linedraw	*data;

	data = (t_linedraw*)status->data;
	if (!(data->portalizing))
		return (0);
	if (abs(data->portal_a_loc.x - x) < 15 && abs(data->portal_a_loc.y - y) < 15)
	{
		data->portalizing_to_a = 1;
		return (1);
	}
	else if (abs(data->portal_b_loc.x - x) < 15 && abs(data->portal_b_loc.y - y) < 15)
	{
		data->portalizing_to_b = 1;
		return (1);
	}
	return (0);
}

static int			handle_portalization(t_status *status, t_linedraw *data)
{
	t_wall 	*wall;
	/* ISOLATE PORTALIZATION HERE */
	//printf("data | por_a = %d [%d, %d] | por_b = %d [%d, %d]\n", data->portal_option_a, data->portal_a_loc.x,
	// data->portal_a_loc.y, data->portal_option_b, data->portal_b_loc.x, data->portal_b_loc.y);
	if (isolated_portalization_check(status, (status->click_x * get_state()->zoom_factor) + get_state()->scroll_x,
									 (status->click_y * get_state()->zoom_factor) + get_state()->scroll_y))
	{
		if (data->portalizing_to_a)
		{
			data->draw_to_x = data->portal_a_loc.x;
			data->draw_to_y = data->portal_a_loc.y;
		}
		else if (data->portalizing_to_b)
		{
			data->draw_to_x = data->portal_b_loc.x;
			data->draw_to_y = data->portal_b_loc.y;
		}
		status->phase++;
		assert(status->phase == 2);
		wall_to_buffer(linedraw_to_wall(data), editor_back_buffer()->buff, 0xffffffff);

		// A COMPLETING CONNECTOR WALL THAT AUTOCOMPLETES THE ROOM CREATION AND DOES THE PORTAL
		// THUS FINISHING THE PORTALIZATION PROCESS
		data->drawing_underway = 1;
		data->draw_from_x = data->draw_to_x;
		data->draw_from_y = data->draw_to_y;
		data->draw_to_x = wall_by_id(data->origin_id)->start.x;
		data->draw_to_y = wall_by_id(data->origin_id)->start.y;
		wall_to_buffer(linedraw_to_wall(data), editor_back_buffer()->buff, COLOR_PORTAL);

		// Invokation of the ending phase, polydraw_end, if status->thread_hit was TRUE, do polydraw_end (phase 2)
		// This basically ONLY happens, when a Room is created properly, thus, call record_room() here.
		if (status->phase == 2)
		{
			// RESERVED FOR HANDLING PORTAL CREATION TO MODEL
			wall = wall_by_id(get_model()->wall_count - 1);
			//ft_putendl("PORTAL CREATED TOO!");
			//printf("wall[%d] equals new portal! %d, %d -> %d, %d\n", wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
			if (record_room(get_model(), wall_by_id(data->origin_id), data->origin_id))
				record_portal(get_model(), wall);
			get_state()->saving_choice = 0;
			//debug_model_rooms();
			status->phases[status->phase](status);
		}
		return (1);
	}
	return (0);
	/* END OF PORTALIZATION ISOLATION BLOCK */
}

void 			polydraw_continue(t_status *status)
{
	t_linedraw      *data;

	data = (t_linedraw*)status->data;
	assert(status->phase == 1);
	assert(data->drawing_underway == 1);

	// ISOLATED FANCY SHINY PORTALIZATION BEGINS
	if (handle_portalization(status, data))
		return ;
	// ISOLATED FANCY SHINY PORTALIZATION ENDS

	if (get_state()->thread_hit)
	{
		if (!get_state()->thread_permission)
			return ;
		data->draw_to_x = (get_state()->thread_x * get_state()->zoom_factor) + get_state()->scroll_x;
		data->draw_to_y = (get_state()->thread_y * get_state()->zoom_factor) + get_state()->scroll_y;
		status->phase++;
		assert(status->phase == 2);
	}
	else
	{
		data->draw_to_x = (status->click_x * get_state()->zoom_factor) + get_state()->scroll_x;
		data->draw_to_y = (status->click_y * get_state()->zoom_factor) + get_state()->scroll_y;
		//printf("scroll x | y: %d | %d\n", get_state()->scroll_x, get_state()->scroll_y);
		//printf("to x | y: %d | %d\n", data->draw_to_x, data->draw_to_y);
	}
	wall_to_buffer(linedraw_to_wall(data), editor_back_buffer()->buff, 0xffffffff);
	//printf("With zoom of %d: linedraw (%d, %d) to (%d, %d)\n", get_state()->zoom_factor,
	//data->draw_from_x, data->draw_from_y, data->draw_to_x, data->draw_to_y);
	//debug_model_walls();
	editor_back_buffer()->rendering_on = 1;
	//*data = (t_linedraw){data->origin_id, 1, (status->click_x * get_state()->zoom_factor) + get_state()->scroll_x,
	//					 (status->click_y * get_state()->zoom_factor) + get_state()->scroll_y, 0}; // OBSOLETED
	data->drawing_underway = 1;
	data->draw_from_x = (status->click_x * get_state()->zoom_factor) + get_state()->scroll_x;
	data->draw_from_y = (status->click_y * get_state()->zoom_factor) + get_state()->scroll_y;
	data->draw_to_x = 0;
	data->draw_to_y = 0;
	// Invokation of the ending phase, polydraw_end, if status->thread_hit was TRUE, do polydraw_end (phase 2)
	// This basically ONLY happens, when a Room is created properly, thus, call record_room() here.
	if (status->phase == 2)
	{
		record_room(get_model(), wall_by_id(data->origin_id), data->origin_id);
		get_state()->saving_choice = 0;
			//debug_model_rooms();
		status->phases[status->phase](status);
	}
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