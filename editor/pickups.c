#include "doom-nukem.h"

static void		square_to_buffer(SDL_Surface *buff, t_point xy, int radius, uint32_t color)
{
	unsigned int *pixels;
	int address;
	int x;
	int y;

	pixels = buff->pixels;

	y = 0;
	while (y <= radius)
	{
		x = 0;
		while (x <= radius)
		{
			if (y == 0 || y == radius || x == 0 || x == radius)
			{
				address = xy.x + x + (xy.y + y) * buff->w;
				if (address >= 0 && address < buff->h * buff->w)
					pixels[address] = color;
				else
				ft_putendl("Warning: square_to_buffer tried to draw outside buffer memory area. Operation was blocked.");
			}
			x++;
		}
		y++;
	}
}

void			pickups_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	pickups_change_zoom(state);
		//puts("Activating pickups");
}

void			pickups_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	pickups_change_zoom(state);
		//puts("Deactivating pickups");
}

void			pickups_change_zoom(t_state *state)
{
	redraw_editor_to_backbuffer(COLOR_LINE);
}

void 			pickups_mouse_motion(int x, int y)
{
	if (x == 12345678)
		y = 1;
	//puts("Pickups mouse motion!");
}

void 			pickups_left_click(int x, int y)
{
	int radius;

	radius = 12 / get_state()->zoom_factor;
	x -= radius / 2;
	y -= radius / 2;
	square_to_buffer(doom_ptr()->edt->buff, (t_point){x, y}, radius, COLOR_HEALTH_PICKUP);
		//puts("Pickups debug: Mock drawing a square directly to buffer!");
}

void 			pickups_right_click(int x, int y)
{
	//planting_logic()->swap_type();
	if (x == 12345678)
		y = 1;
	puts("Pickups mouse right click!");
}

void 			pickups_middle_click(int x, int y)
{
	if (x == 12345678)
		y = 1;
	puts("Pickups mouse middle click!");
}