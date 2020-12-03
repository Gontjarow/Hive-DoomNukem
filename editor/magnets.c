#include "doom-nukem.h"

void            check_any_magnet_hits(int x, int y, t_model *mdl, t_state *state)
{
    int         wc;
    int			zf;
    int 		sx;
    int 		sy;
    t_wall      *wall;

    wc = mdl->wall_count;
    wall = mdl->wall_first;
    if (wall == NULL || state->job_abort)
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
            {
                state->thread_permission = 1;
                state->thread_color = 0xff00ff00;
            }
            return ;
        }
        else if (abs((wall->end.x / zf) - x - (sx / zf)) < 15 && abs((wall->end.y / zf) - y - (sy / zf)) < 15)
        {
                //ft_putendl("Magnet hit within 15 units for a ending point of a wall.");
            state->thread_x = (wall->end.x / zf) - (sx / zf);
            state->thread_y = (wall->end.y / zf) - (sy / zf);
            state->thread_hit = 1;
            return ;
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