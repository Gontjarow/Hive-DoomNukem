#include "doom-nukem.h"

static uint32_t	type_colors(int type)
{
	static uint32_t t_colors[2] = { COLOR_PLAYER, COLOR_ENEMY };

	return (t_colors[type]);
}

t_point			relative_position(int x, int y, t_state *state)
{
	int relative_x;
	int relative_y;

	relative_x = state->scroll_x;
	relative_y = state->scroll_y;
	x *= state->zoom_factor;
	y *= state->zoom_factor;
	relative_x += x;
	relative_y += y;
		//printf("relativized position to  ( %d | %d )\n", relative_x, relative_y);
	return ((t_point){relative_x, relative_y});
}

static t_point	scrolled_position(int x, int y, t_state *state)
{
	int relative_x;
	int relative_y;

	relative_x = x - state->scroll_x;
	relative_y = y - state->scroll_y;
	relative_x /= state->zoom_factor;
	relative_y /= state->zoom_factor;
		//printf("scrolled position ( %d | %d ) to ( %d | %d )\n", x, y, relative_x, relative_y);
	return ((t_point){relative_x, relative_y});
}

static int		valid_line(t_line *line)
{
	if (line->x1 < 0 || line->x1 >= EDT_WIN_WIDTH)
		return (0);
	if (line->x2 < 0 || line->x2 >= EDT_WIN_WIDTH)
		return (0);
	if (line->y1 < 0 || line->y1 >= EDT_WIN_HEIGHT)
		return (0);
	if (line->y2 < 0 || line->y2 >= EDT_WIN_HEIGHT)
		return (0);
	return (1);
}

static void		update_tail_to_buffer(SDL_Surface *buff, void *obj_ptr, int obj_type)
{
	t_line 		line;
	t_point		rel_pos;
	t_player	*player;
	t_enemy		*enemy;
	double 		rad;

	line.color = type_colors(obj_type);
	line.buff = buff;
	line.doom = doom_ptr();
	if (obj_type == PLAYER)
	{
		player = (t_player*)obj_ptr;
		rad = ((player->rot) * M_PI / 180);
		player->tail.x = (int)player->x + (int)(12.0 * -cos(rad));
		player->tail.y = (int)player->y + (int)(12.0 * -sin(rad));
		rel_pos = scrolled_position((int)player->x, (int)player->y, get_state());
		line.x1 = rel_pos.x;
		line.y1 = rel_pos.y;
		rel_pos = scrolled_position(player->tail.x, player->tail.y, get_state());
		line.x2 = rel_pos.x;
		line.y2 = rel_pos.y;
		if (valid_line(&line))
			render_line(&line);
			//puts("Drew tail for player object - aha");
	}
	else if (obj_type == ENEMY)
	{
		enemy = (t_enemy*)obj_ptr;
		rad = ((enemy->rot) * M_PI / 180);
		enemy->tail.x = enemy->x + (int)(12.0 * -cos(rad));
		enemy->tail.y = enemy->y + (int)(12.0 * -sin(rad));
		rel_pos = scrolled_position(enemy->x, enemy->y, get_state());
		line.x1 = rel_pos.x;
		line.y1 = rel_pos.y;
		rel_pos = scrolled_position(enemy->tail.x, enemy->tail.y, get_state());
		line.x2 = rel_pos.x;
		line.y2 = rel_pos.y;
		if (valid_line(&line))
			render_line(&line);
			//printf("rel_pos.x, rel_pos.y from tail .x (%d) .y(%d) = { %d, %d }\n", enemy->tail.x, enemy->tail.y, line.x2, line.y2);
			//puts("Drew tail for enemy object - yup");
	}
}

static void		draw_player(t_model *mdl, t_state *state)
{
	int 		relative_x;
	int 		relative_y;

	if (!((mdl->player.x >= state->scroll_x) &&
		(mdl->player.x <= state->scroll_x + (state->zoom_factor * EDT_WIN_WIDTH)) &&
		(mdl->player.y >= state->scroll_y) &&
		(mdl->player.y <= state->scroll_y + (state->zoom_factor * EDT_WIN_HEIGHT))))
		return ;
	relative_x = (int)mdl->player.x;
	relative_y = (int)mdl->player.y;
	relative_x -= state->scroll_x;
	relative_y -= state->scroll_y;
	relative_x /= state->zoom_factor;
	relative_y /= state->zoom_factor;
	circle_to_buffer(editor_back_buffer()->buff,(t_point){relative_x, relative_y}, 12 / state->zoom_factor, type_colors(PLAYER));
	update_tail_to_buffer(editor_back_buffer()->buff, (void*)&(mdl->player), PLAYER);
}

static void		draw_enemy(t_enemy *enemy, t_state *state)
{
	int 		relative_x;
	int 		relative_y;

	if (!((enemy->x >= state->scroll_x) &&
		  (enemy->x <= state->scroll_x + (state->zoom_factor * EDT_WIN_WIDTH)) &&
		  (enemy->y >= state->scroll_y) &&
		  (enemy->y <= state->scroll_y + (state->zoom_factor * EDT_WIN_HEIGHT))))
		return ;
	relative_x = enemy->x;
	relative_y = enemy->y;
	relative_x -= state->scroll_x;
	relative_y -= state->scroll_y;
	relative_x /= state->zoom_factor;
	relative_y /= state->zoom_factor;
	circle_to_buffer(editor_back_buffer()->buff,(t_point){relative_x, relative_y}, 12 / state->zoom_factor, type_colors(ENEMY));
	update_tail_to_buffer(editor_back_buffer()->buff, (void*)enemy, ENEMY);
}

static uint32_t	pickup_color(int flavor)
{
	if (flavor == PICKUP_HEALTH)
		return (COLOR_HEALTH_PICKUP);
	if (flavor == PICKUP_AMMO)
		return (COLOR_AMMO_PICKUP);
	if (flavor == PICKUP_WEAPON)
		return (COLOR_WEAPON_PICKUP);
	ft_putendl("Warning: Could not match pickup flavor to a color at pickup_color.");
	return (COLOR_LINE);
}

static void 	draw_pickup(t_pickup *pickup, t_state *state)
{
	int 		relative_x;
	int 		relative_y;
	int 		radius;

	if (!((pickup->loc.x >= state->scroll_x) &&
		  (pickup->loc.x <= state->scroll_x + (state->zoom_factor * EDT_WIN_WIDTH)) &&
		  (pickup->loc.y >= state->scroll_y) &&
		  (pickup->loc.y <= state->scroll_y + (state->zoom_factor * EDT_WIN_HEIGHT))))
		return ;
		//puts("Pickup drawing...");
	radius = 12 / get_state()->zoom_factor;
	relative_x = pickup->loc.x;
	relative_y = pickup->loc.y;
	relative_x -= state->scroll_x;
	relative_y -= state->scroll_y;
	relative_x /= state->zoom_factor;
	relative_y /= state->zoom_factor;
	square_to_buffer(editor_back_buffer()->buff, (t_point){relative_x, relative_y}, radius, pickup_color(pickup->flavor));
	if (pickup->flavor == PICKUP_HEALTH)
		cross_to_buffer(editor_back_buffer()->buff, (t_point){relative_x, relative_y}, radius / 2, pickup_color(pickup->flavor));
}

// TODO			BLIT BASED RENDERING IS SLOW!!!! FIX???
//					BEFORE OPTIMIZING CODE, TRY OPTIMIZING SURFACE WITH SDL_CONVERT
//				ALTERNATIVE SUGGESTION: DRAW DIRECTLY TO WINDOW BUFFER, SKIPPING BLIT

void 			draw_plantings_to_backbuffer(t_model *mdl, t_state *state)
{
	t_enemy		*enemy;
	t_pickup	*pickup;
	int 		count;

	editor_back_buffer()->rendering_on = 1;
	if (mdl->player.x != -1)
		draw_player(mdl, state);
	if (mdl->enemy_count != 0)
	{
		count = mdl->enemy_count;
		enemy = mdl->enemy_first;
		while (count--)
		{
			draw_enemy(enemy, state);
			enemy = enemy->next;
		}
	}
	if (mdl->pickup_count != 0)
	{
		count = mdl->pickup_count;
		pickup = mdl->pickup_first;
		while (count--)
		{
			draw_pickup(pickup, state);
			pickup = pickup->next;
		}
	}
		//puts("Drawing plantings to back buffer!");
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
		enemy = record_enemy(relative, tail, get_model());
	if (!clean_up)
		update_tail_to_buffer(editor_back_buffer()->buff, enemy, ENEMY);
	planting_logic()->planted_ticks = SDL_GetTicks();
	circle_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, 12 / get_state()->zoom_factor, type_colors(planting_logic()->plant_type));
	editor_back_buffer()->rendering_on = 1;
	if (clean_up == 1)
		planting_logic()->plant_type = ENEMY;
	if (clean_up == 2)
		redraw_editor_to_backbuffer(COLOR_LINE);
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
	redraw_editor_to_backbuffer(COLOR_LINE);
}

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