#include "doom-nukem.h"

static void		tail_to_buffer(SDL_Surface *buff, void *obj_ptr, int obj_type)
{
	t_line 		line;
	t_point		rel_pos;
	t_player	*player;
	t_enemy		*enemy;

	line.color = type_colors(obj_type);
	line.buff = buff;
	if (obj_type == PLAYER)
	{
		player = (t_player*)obj_ptr;
		rel_pos = scrolled_position((int)player->x, (int)player->y, get_state());
		line.x1 = rel_pos.x;
		line.y1 = rel_pos.y;
		rel_pos = scrolled_position(player->tail.x, player->tail.y, get_state());
		line.x2 = rel_pos.x;
		line.y2 = rel_pos.y;
		if (valid_line(&line))
			render_line(&line);
	}
	else if (obj_type == ENEMY)
	{
		enemy = (t_enemy*)obj_ptr;
		rel_pos = scrolled_position(enemy->x, enemy->y, get_state());
		line.x1 = rel_pos.x;
		line.y1 = rel_pos.y;
		rel_pos = scrolled_position(enemy->tail.x, enemy->tail.y, get_state());
		line.x2 = rel_pos.x;
		line.y2 = rel_pos.y;
		if (valid_line(&line))
			render_line(&line);
	}
}

static void			update_tail(void *obj_ptr, int obj_type)
{
	t_player	*player;
	t_enemy		*enemy;
	double 		rad;

	if (obj_type == PLAYER)
	{
		player = (t_player*)obj_ptr;
		rad = ((player->rot) * M_PI / 180);
		player->tail.x = (int)player->x + (int)(12.0 * -cos(rad));
		player->tail.y = (int)player->y + (int)(12.0 * -sin(rad));
	}
	else if (obj_type == ENEMY)
	{
		enemy = (t_enemy*)obj_ptr;
		rad = ((enemy->rot) * M_PI / 180);
		enemy->tail.x = enemy->x + (int)(12.0 * -cos(rad));
		enemy->tail.y = enemy->y + (int)(12.0 * -sin(rad));
	}
}

void		draw_player(t_model *mdl, t_state *state)
{
	int		relative_x;
	int		relative_y;

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
	update_tail((void*)&(mdl->player), PLAYER);
	tail_to_buffer(editor_back_buffer()->buff, (void*)&(mdl->player), PLAYER);
}

void		draw_enemy(t_enemy *enemy, t_state *state)
{
	int		relative_x;
	int		relative_y;

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
	digit_to_buffer(editor_back_buffer()->buff, (t_point){relative_x, relative_y - 5}, enemy->wep.type_id * 10, type_colors(ENEMY));
	digit_to_buffer(editor_back_buffer()->buff, (t_point){relative_x - 5, relative_y + 5}, enemy->ai.type_id, type_colors(ENEMY));
	//digit_to_buffer(editor_back_buffer()->buff, (t_point){relative_x + 5, relative_y + 5}, enemy->sprite_id, type_colors(ENEMY));
	update_tail((void*)enemy, ENEMY);
	//tail_to_buffer(editor_back_buffer()->buff, (void*)enemy, ENEMY);
}


void	 	draw_pickup(t_pickup *pickup, t_state *state)
{
	int		relative_x;
	int		relative_y;
	int		radius;

	if (!((pickup->loc.x >= state->scroll_x) &&
		  (pickup->loc.x <= state->scroll_x + (state->zoom_factor * EDT_WIN_WIDTH)) &&
		  (pickup->loc.y >= state->scroll_y) &&
		  (pickup->loc.y <= state->scroll_y + (state->zoom_factor * EDT_WIN_HEIGHT))))
		return ;
	//puts("Pickup drawing...");
	radius = PICKUP_RADIUS / get_state()->zoom_factor;
	relative_x = pickup->loc.x;
	relative_y = pickup->loc.y;
	relative_x -= state->scroll_x;
	relative_y -= state->scroll_y;
	relative_x /= state->zoom_factor;
	relative_y /= state->zoom_factor;
	square_to_buffer(editor_back_buffer()->buff, (t_point){relative_x, relative_y}, radius, pickup_color(pickup->flavor));
	if (pickup->flavor == PICKUP_HEALTH)
		cross_to_buffer(editor_back_buffer()->buff, (t_point){relative_x, relative_y}, radius / 2, pickup_color(pickup->flavor));
	else
		digit_to_buffer(editor_back_buffer()->buff, (t_point){relative_x, relative_y}, radius == PICKUP_RADIUS ? pickup->weapon_type_id * 10 : pickup->weapon_type_id, pickup_color(pickup->flavor));
}

void			update_tail_to_buffer(SDL_Surface *buff, void *obj_ptr, int obj_type)
{
	t_line 		line;
	t_point		rel_pos;
	t_player	*player;
	t_enemy		*enemy;
	double 		rad;

	line.color = type_colors(obj_type);
	line.buff = buff;
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
// TODO			BLIT BASED RENDERING IS SLOW!!!! FIX???
//					BEFORE ABANDONING SDL_BLIT CODE, TRY OPTIMIZING SDL_SURFACE WITH SDL_CONVERT
//				ALTERNATIVELY: DRAW DIRECTLY TO INTENDED PIXELBUFFER, SKIPPING BLIT
void 			draw_selection_to_backbuffer(t_state *state)
{
	t_room		*room;
	t_point		adjusted;

	if (state->gui != mode_select())
		return ;
	if (select_logic()->selected_room_id == -1)
		return ;
	room = room_by_id(select_logic()->selected_room_id);
	room_walls_to_buffer(room, editor_back_buffer()->buff, COLOR_SELECTION_LINE);
	if (room->visual.x == -1 && room->visual.y == -1)
		find_visual_xy(room);
	adjusted = scrolled_position(room->visual.x, room->visual.y, get_state());
	square_to_buffer(editor_back_buffer()->buff, adjusted, 10, COLOR_SELECTION_LINE);
	adjusted.y -= 20;
	number_to_buffer(editor_back_buffer()->buff, adjusted, room->roof_height, COLOR_SELECTION_LINE);
	adjusted.y += 40;
	number_to_buffer(editor_back_buffer()->buff, adjusted, room->floor_height, COLOR_SELECTION_LINE);
}
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