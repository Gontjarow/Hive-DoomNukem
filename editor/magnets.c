#include "doom-nukem.h"

static void		set_portal_ids_to_linedraw_data(t_status *status, int id, int other_wall_id)
{
	t_linedraw *data;

	data = (t_linedraw*)status->data;
	if (other_wall_id == 1) // same id start, plus one id end
	{
		data->portal_option_a = id;
		data->portal_option_b = id + 1;
		data->portal_a_loc = (t_point) {wall_by_id(data->portal_option_a)->start.x, wall_by_id(data->portal_option_a)->start.y};
		data->portal_b_loc = (t_point) {wall_by_id(data->portal_option_b)->end.x, wall_by_id(data->portal_option_b)->end.y};
	}
	else if (other_wall_id == -1) // same id end, minus one id start
	{
		data->portal_option_a = id;
		data->portal_option_b = id - 1;
		data->portal_a_loc = (t_point) {wall_by_id(data->portal_option_a)->end.x, wall_by_id(data->portal_option_a)->end.y};
		data->portal_b_loc = (t_point) {wall_by_id(data->portal_option_b)->start.x, wall_by_id(data->portal_option_b)->start.y};
	}
	data->portalizing = 1;
	data->portalizing_to_a = 0;
	data->portalizing_to_b = 0;
	// TODO plot to buffer according to zoom and scrolls here
	circle_to_buffer(doom_ptr()->edt->buff, scroll_adjusted(data->portal_a_loc), 15 / get_state()->zoom_factor, 0xff00ffff);
	circle_to_buffer(doom_ptr()->edt->buff, scroll_adjusted(data->portal_b_loc), 15 / get_state()->zoom_factor, 0xff00ffff);
}

t_point 		detect_wall_point(int x, int y, t_model *mdl, t_state *state)
{
	int         wc;
	t_wall      *wall;

	wc = mdl->wall_count;
	wall = mdl->wall_first;
	while (wc--)
	{
		if (abs((wall->start.x) - x) < 15 && abs((wall->start.y) - y < 15))
		{
			set_portal_ids_to_linedraw_data(polydraw_status(), wall->id, -1);
			return (t_point) {wall->start.x, wall->start.y};
		}
		else if (abs((wall->end.x) - x) < 15 && abs((wall->end.y) - y) < 15)
		{
			set_portal_ids_to_linedraw_data(polydraw_status(), wall->id, 1);
			return (t_point) {wall->end.x, wall->end.y};
		}
		wall = wall->next;
	}
	return ((t_point){-1, -1});
}

static void 	thread_coloring_rule(t_state *state, int found_origin)
{
	t_linedraw *data;

	if (found_origin)
	{
		state->thread_permission = 1;
		state->thread_color = 0xff00ff00;
	}
	else if (polydraw_status() != NULL)
	{
		data = (t_linedraw*)polydraw_status()->data;
		if (abs(data->portal_a_loc.x - state->thread_x) < 15 && abs(data->portal_a_loc.y - state->thread_y) < 15)
			state->thread_color = 0xff00ffff;
		else if (abs(data->portal_b_loc.x - state->thread_x) < 15 && abs(data->portal_b_loc.y - state->thread_y) < 15)
			state->thread_color = 0xff00ffff;
	}
}

void            check_any_magnet_hits(int x, int y, t_model *mdl, t_state *state)
{
    int			zf;
    int 		sx;
    int 		sy;
	int         wc;
    t_wall      *wall;

	if (state->job_abort || mdl == NULL)
		return ;
    wc = mdl->wall_count;
    wall = mdl->wall_first;
    if (wall == NULL)
        return ;
    // Here, check if still hitting a previously found magnet area
    if (state->thread_hit)
    {
        if (abs(state->thread_x - x) < 15 && abs(state->thread_y - y) < 15)
            return ;
        // If no longer hitting, start looking for hits again
        state->thread_hit = 0;
    }
    zf = state->zoom_factor;
    sx = state->scroll_x;
    sy = state->scroll_y;
    // Loop through all the walls to search for hits
    state->thread_permission = 0;
    state->thread_color = 0xffff0000;
    while (wc--)
    {
        if (abs((wall->start.x / zf) - x - (sx / zf)) < 15 && abs((wall->start.y / zf) - y - (sy / zf)) < 15)
        {
                //ft_putendl("Magnet hit within 15 units for a starting point of a wall.");
            state->thread_x = (wall->start.x / zf) - (sx / zf);
            state->thread_y = (wall->start.y / zf) - (sy / zf);
            state->thread_hit = 1;

            if (wall->id == state->thread_target_id)
				return (thread_coloring_rule(state, 1));
            return (thread_coloring_rule(state, 0));
        }
        else if (abs((wall->end.x / zf) - x - (sx / zf)) < 15 && abs((wall->end.y / zf) - y - (sy / zf)) < 15)
        {
                //ft_putendl("Magnet hit within 15 units for a ending point of a wall.");
            state->thread_x = (wall->end.x / zf) - (sx / zf);
            state->thread_y = (wall->end.y / zf) - (sy / zf);
            state->thread_hit = 1;
            return (thread_coloring_rule(state, 0));
        }
        // For segmentation fault avoidance, abort mission if the phase changes
        if (state->job_abort)
            return ;
        wall = wall->next;
    }
}

void            magnet_test(void* argv)
{
    t_status    *status;
    int         x;
    int         y;

    status = (t_status*)argv;
    while (!get_state()->job_abort)
    {
            //printf("Magnet testing for %d, %d\n", x, y);
        x = status->motion_x;
        y = status->motion_y;
        check_any_magnet_hits(x, y, get_model(), get_state());
    }
    get_state()->job_running = 0;
}