#include "doom-nukem.h"

// TODO FEATURE DEBT: ADD LIVE PREVIEW TO SWEEPING TAIL

void 			planting_mouse_motion(int x, int y)
{
	static t_point	last_preview = {-1, -1};
	t_point			*sweep;

	if (planting_logic()->sweep_counter == 0)
	{
		sweep = planting_logic()->sweep;
		sweep[1].x = sweep[0].x;
		sweep[1].y = sweep[0].y;
		sweep[0].x = x;
		sweep[0].y = y;
		planting_logic()->sweep_counter = 10;
		//printf("sweeped to %d, %d\n", sweep[1].x, sweep[1].y);
	}
	else
	{
		planting_logic()->sweep_counter--;
		//puts("*sweeping*");
	}
	if (SDL_GetTicks() - planting_logic()->planted_ticks < 20)
	{
		last_preview.x = -1;
		last_preview.y = -1;
		return ;
	}
	if (last_preview.x != -1 && last_preview.y != -1)
		preserving_circle_to_buffer(doom_ptr()->edt->buff, last_preview, 12 / get_state()->zoom_factor, type_colors(planting_logic()->plant_type));
	unpreserving_circle_to_buffer(doom_ptr()->edt->buff, (t_point){x, y}, 12 / get_state()->zoom_factor, type_colors(planting_logic()->plant_type));
	last_preview.x = x;
	last_preview.y = y;
}

void 			planting_left_click(int x, int y)
{
	planting_logic()->plant(x, y);
}

void 			planting_right_click(int x, int y)
{
	planting_logic()->swap_type();
}

void 			planting_middle_click(int x, int y)
{
	x = y;
	return ;
}
