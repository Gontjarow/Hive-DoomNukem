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
	t_enemy		*enemy;
	int 		ec;

	if (abs((int)get_model()->player.x - x) < 24 && abs((int)get_model()->player.y - y) < 24)
		return (-2);
	enemy = get_model()->enemy_first;
	ec = get_model()->enemy_count;
	while (ec--)
	{
		if (abs(x - enemy->x) < 24 && abs(y - enemy->y) < 24)
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
	else if (curr_id == last_id)
	{
		remove_enemy(curr_id);
		planting_change_zoom(get_state());
		last_id = -1;
	}
	else
		last_id = curr_id;
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
