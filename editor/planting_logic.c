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
		logic->sweep_counter = 0;
		logic->sweep[0].x = 0;
		logic->sweep[0].y = 0;
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
	int 		clean_up;
	t_point		relative;
	t_point		*tail;
	t_enemy		*enemy;

	clean_up = 0;
	relative = relative_position(x, y, get_state());
	tail = &planting_logic()->sweep[1];
	if (planting_logic()->plant_type == PLAYER && get_model()->player.x == -1)
	{
		record_player(relative, tail, get_model());
		update_tail_to_buffer(editor_back_buffer()->buff, (void*)&(get_model()->player), PLAYER);
		clean_up = 1;
	}
	else if (planting_logic()->plant_type == PLAYER && get_model()->player.x != -1)
	{
		record_player(relative, tail, get_model());
		clean_up = 2;
	}
	else if (planting_logic()->plant_type == ENEMY)
	{
		enemy = record_enemy(relative, tail, get_model());
		digit_to_buffer(editor_back_buffer()->buff, (t_point){x, y - 5}, enemy->wep.type_id * 10, type_colors(ENEMY));
		digit_to_buffer(editor_back_buffer()->buff, (t_point){x - 5, y + 5}, enemy->ai.type_id, type_colors(ENEMY));
		//digit_to_buffer(editor_back_buffer()->buff, (t_point){x + 5, y + 5}, enemy->sprite_id, type_colors(ENEMY));
	}
	if (!clean_up)
		update_tail_to_buffer(editor_back_buffer()->buff, enemy, ENEMY);
	planting_logic()->planted_ticks = SDL_GetTicks();
	circle_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, 12 / get_state()->zoom_factor, type_colors(planting_logic()->plant_type));
	editor_back_buffer()->rendering_on = 1;
	if (clean_up == 1)
		planting_logic()->plant_type = ENEMY;
	if (clean_up == 2)
		redraw_editor_to_backbuffer(COLOR_LINE);
	get_state()->saving_choice = 0;
}