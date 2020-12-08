#include "doom-nukem.h"

static uint32_t type_colors(int type)
{
	static uint32_t t_colors[2] = { COLOR_PLAYER, COLOR_ENEMY };

	return (t_colors[type]);
}

void 			draw_plantings_to_backbuffer(t_model *mdl)
{
	t_enemy		*enemy;
	int 		ec;

	editor_back_buffer()->rendering_on = 1;
	if (mdl->player.x != -1)
		circle_to_buffer(editor_back_buffer()->buff, (t_point){(int)mdl->player.x, (int)mdl->player.y}, 10, type_colors(PLAYER));
	if (mdl->enemy_count == 0)
		return ;
	ec = mdl->enemy_count;
	enemy = mdl->enemy_first;
	while (ec--)
	{
		circle_to_buffer(editor_back_buffer()->buff, (t_point){enemy->x, enemy->y}, 10, type_colors(ENEMY));
		enemy = enemy->next;
	}
		puts("Drawing plantings to back buffer!");
}

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

void 			planting_swap_type(void)
{
	if (planting_logic()->plant_type == PLAYER)
		planting_logic()->plant_type = ENEMY;
	else if (planting_logic()->plant_type == ENEMY)
		planting_logic()->plant_type = PLAYER;
}

void 			planting_plant(int x, int y)
{
	int 		swap_type;

	swap_type = 0;
	if (planting_logic()->plant_type == PLAYER && get_model()->player.x == -1)
	{
		record_player((t_point) {x, y}, (t_point) {x + 10, y + 10}, get_model());
		swap_type = 1;
	}
	else if (planting_logic()->plant_type == PLAYER && get_model()->player.x != -1)
	{
		return ;
	}
	else if (planting_logic()->plant_type == ENEMY)
		record_enemy((t_point){x, y}, (t_point){x + 10, y + 10}, get_model());
	planting_logic()->planted_ticks = SDL_GetTicks();
	circle_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, 10, type_colors(planting_logic()->plant_type));
	editor_back_buffer()->rendering_on = 1;
	if (swap_type)
		planting_logic()->plant_type = ENEMY;
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
		//puts("Activating planting");
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
		//puts("Deactivating planting");
}

void			planting_change_zoom(t_state *state)
{
	wipe_editor_back_buffer(0xff000000);
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, 0xffffffff);
	print_mode_info(state->gui);
	draw_scroll_bars_to_backbuffer(state);
	draw_plantings_to_backbuffer(get_model());
}

void 			planting_mouse_motion(int x, int y)
{
	static t_point	last_preview = {-1, -1};

	if (SDL_GetTicks() - planting_logic()->planted_ticks < 20)
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