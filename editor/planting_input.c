#include "doom-nukem.h"

// TODO FEATURE DEBT: ADD LIVE PREVIEW TO SWEEPING TAIL

static void		preview_tail(int x, int y, int unpreview, t_point *sweep)
{
	static t_line	line;
	int 			rot;
	double 			rad;
	int 			tx;
	int 			ty;

	if (unpreview && line.buff == doom_ptr()->edt->buff)
	{
		line.color = 0xff000000;
		preserve_render_line(&line);
			//puts("Unpreviewing sweeping tail line!");
		return ;
	}
	rot = tail_degree_rot((t_point){x, y}, &sweep[1]);
	line.buff = doom_ptr()->edt->buff;
	line.color = type_colors(planting_logic()->plant_type);
	rad = ((rot) * M_PI / 180);
	tx = x + (int)(12.0 / get_state()->zoom_factor * -cos(rad));
	ty = y + (int)(12.0 / get_state()->zoom_factor * -sin(rad));
	line.x1 = x;
	line.y1 = y;
	line.x2 = tx;
	line.y2 = ty;
	preserve_render_line(&line);
		//puts("Previewing sweeping tail line!");
}

void 			planting_mouse_motion(int x, int y)
{
	static t_point	last_preview = {-1, -1};
	t_point			*sweep;

	if (planting_logic()->sweep_counter == 0)
	{
		sweep = planting_logic()->sweep;
		if (last_preview.x != -1 && last_preview.y != -1)
			preview_tail(x, y, 1, sweep);
		sweep[1].x = sweep[0].x;
		sweep[1].y = sweep[0].y;
		sweep[0].x = x;
		sweep[0].y = y;
		if (last_preview.x != -1 && last_preview.y != -1)
			preview_tail(x, y, 0, sweep);
		planting_logic()->sweep_counter = 6;
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
		preserving_circle_to_buffer(doom_ptr()->edt->buff, last_preview,12 / get_state()->zoom_factor, type_colors(planting_logic()->plant_type));
	unpreserving_circle_to_buffer(doom_ptr()->edt->buff, (t_point){x, y}, 12 / get_state()->zoom_factor, type_colors(planting_logic()->plant_type));
	last_preview.x = x;
	last_preview.y = y;
}

static int		which_overlapping_planting(int x, int y)
{
	t_point		rel;
	t_enemy		*enemy;
	int 		ec;

	rel = relative_position(x, y, get_state());
	if (abs((int)get_model()->player.x - rel.x) < 24 && abs((int)get_model()->player.y - rel.y) < 24)
		return (-2);
	enemy = get_model()->enemy_first;
	ec = get_model()->enemy_count;
	while (ec--)
	{
		if (abs(rel.x - enemy->x) < 24 && abs(rel.y - enemy->y) < 24)
			return (enemy->id);
		enemy = enemy->next;
	}
	return (-1);
}

static void 	recalc_enemy_ids(void)
{
	t_enemy		*enemy;
	int 		ec;
	int 		i;

	i = 0;
	enemy = get_model()->enemy_first;
	ec = get_model()->enemy_count;
	while (ec--)
	{
		enemy->id = i++;
		enemy = enemy->next;
	}
}

static void 	link_enemy_by_id(int id, t_enemy *linking_enemy)
{
	t_enemy	*enemy;
	int		ec;

	if (id < 0)
		ft_die("Fatal error: Tried to link with invalid id at link_enemy_by_id");
	enemy = get_model()->enemy_first;
	ec = get_model()->enemy_count;
	while (ec--)
	{
		if (enemy->id == id)
		{
			enemy->next = linking_enemy;
			return ;
		}
		enemy = enemy->next;
	}
	ft_putendl("Warning: Could not link enemy by id, id did not match with an enemy at link_enemy_by_id!");
}

static void 	remove_enemy(int id)
{
	t_enemy		*enemy;
	int 		ec;

		//puts("Removing a planting! Double click detected!");
	enemy = get_model()->enemy_first;
	ec = get_model()->enemy_count;
	while (ec--)
	{
		if (enemy->id == id)
		{
			if (id == 0)
				get_model()->enemy_first = enemy->next;
			else
				link_enemy_by_id(id - 1, enemy->next);
			get_model()->enemy_count--;
			free(enemy);
			recalc_enemy_ids();
			break ;
		}
		enemy = enemy->next;
	}
}

static t_enemy	*enemy_by_id(int id)
{
	t_enemy *enemy;
	int 	ec;

	ec = get_model()->enemy_count;
	enemy = get_model()->enemy_first;
	while (ec--)
	{
		if (enemy->id == id)
			return (enemy);
		enemy = enemy->next;
	}
	ft_die("Fatal error: Could not find enemy from model at enemy_by_id.");
	return (NULL);
}

static void 	rotate_enemy(int id, int x, int y)
{
	t_enemy *enemy;
	t_point	rel;
	t_point tail;

	enemy = enemy_by_id(id);
	rel = scrolled_position(enemy->x, enemy->y, get_state());
	tail.x = x;
	tail.y = y;
	enemy->rot = tail_degree_rot(rel, &tail);
}

static void 	temp_highlight_enemy(t_enemy *enemy)
{
	t_line 		line;
	t_point		rel_pos;

	line.color = COLOR_GRID_LINE;
	line.buff = doom_ptr()->edt->buff;
	rel_pos = scrolled_position(enemy->x, enemy->y, get_state());
	circle_to_buffer(line.buff, rel_pos, 12 / get_state()->zoom_factor, COLOR_GRID_LINE);
	line.x1 = rel_pos.x;
	line.y1 = rel_pos.y;
	rel_pos = scrolled_position(enemy->tail.x, enemy->tail.y, get_state());
	line.x2 = rel_pos.x;
	line.y2 = rel_pos.y;
	if (valid_line(&line))
		render_line(&line);
	get_state()->cooldown = DOUBLE_CLICK_COOLDOWN;
}

void 			planting_left_click(int x, int y)
{
	static int	last_id = -1;
	int 		curr_id;

	curr_id = which_overlapping_planting(x, y);

	if (curr_id == -2)
	{
		last_id = -1;
		return ;
	}
	else if (curr_id == -1)
		planting_logic()->plant(x, y);
	else if (curr_id == last_id && get_state()->cooldown)
	{
		remove_enemy(curr_id);
		planting_change_zoom(get_state());
		last_id = -1;
	}
	else if (curr_id == last_id)
	{
		rotate_enemy(curr_id, x, y);
		planting_change_zoom(get_state());
		last_id = -1;
	}
	else
	{
		last_id = curr_id;
		temp_highlight_enemy(enemy_by_id(curr_id));
	}
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
