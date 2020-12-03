#include "doom-nukem.h"

t_logic 		*planting_logic(void)
{
	static		t_logic *logic = NULL;

	if (!logic)
	{
		logic = (t_logic*)malloc(sizeof(t_logic));
		if (!logic)
			ft_die("Fatal error: Could not malloc logic for planting at planting_logic");
		logic->plant_type = PLAYER;
		logic->plant = planting_plant;
		logic->swap_type = planting_swap_type;
	}
	return (logic);
}

static uint32_t type_colors(int type)
{
	static uint32_t t_colors[2] = {0xff00ff00, 0xffffff00};

	return (t_colors[type]);
}

void 			planting_swap_type(void)
{
	if (planting_logic()->plant_type == PLAYER)
		planting_logic()->plant_type = ENEMY;
	else if (planting_logic()->plant_type == ENEMY)
		planting_logic()->plant_type = PLAYER;
}

void 			planting_plant(int x, int y)
{
	planting_logic()->planted_ticks = SDL_GetTicks();
	circle_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, 10, type_colors(planting_logic()->plant_type));
	editor_back_buffer()->rendering_on = 1;
}

void			planting_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	planting_change_zoom(state);
}

void			planting_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	planting_change_zoom(state);
}

void			planting_change_zoom(t_state *state)
{
	wipe_editor_back_buffer(0xff000000);
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, 0xffffffff);
	print_mode_info(state->gui);
	draw_scroll_bars_to_backbuffer(state);
}

void 			planting_mouse_motion(int x, int y)
{
	static t_point	last_preview = {-1, -1};

	if (SDL_GetTicks() - planting_logic()->planted_ticks < 200)
	{
		last_preview.x = -1;
		last_preview.y = -1;
		return ;
	}
	if (last_preview.x != -1 && last_preview.y != -1)
		preserving_circle_to_buffer(doom_ptr()->edt->buff, last_preview, 10, type_colors(planting_logic()->plant_type));
	unpreserving_circle_to_buffer(doom_ptr()->edt->buff, (t_point){x, y}, 10, type_colors(planting_logic()->plant_type));
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