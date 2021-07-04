#include "doom-nukem.h"

static t_effect	*effect_by_id(int id)
{
	t_effect 	*effect;
	int 		ec;

	ec = get_model()->effect_count;
	effect = get_model()->effect_first;
	while (ec--)
	{
		if (effect->id == id)
			return (effect);
		effect = effect->next;
	}
	ft_die("Fatal error: Could not find effect from model at effect_by_id.");
	return (NULL);
}

static void 	effect_plant_exit(int x, int y)
{
	t_model		*mdl;
	t_effect	*new_effect;

	mdl = get_model();
	if (mdl->effect_count > 0)
	{
		mdl->effect_first->loc.x = x;
		mdl->effect_first->loc.y = y;
		//puts("Replaced Level Exit Effector!");
	}
	else
	{
		mdl->effects->id = mdl->effect_count;
		mdl->effects->type_id = EFFECT_EXIT;
		mdl->effects->loc.x = x;
		mdl->effects->loc.y = y;
		mdl->effects->target.x = 0;
		mdl->effects->target.y = 0;
		mdl->effects->target_id = 0;
		mdl->effects->activated = 0;
		mdl->effects->active_sprite = NULL;
		new_effect = (t_effect*)malloc(sizeof(t_effect));
		if (!new_effect)
			ft_die("Fatal error: Could not malloc new_effect at effect_plant_exit!");
		mdl->effects->next = new_effect;
		mdl->effect_count++;
		if (mdl->effect_count == 1)
			mdl->effect_first = mdl->effects;
		mdl->effects = new_effect;
	}
}

static int		distance_to_wall(t_xy point, t_wall *wall, t_point *positive_hit)
{
	t_xy orig;
	t_xy vec;
	t_xy result;
	int	confirm;

	confirm = 0;
	orig.x = wall->start.x;
	orig.y = wall->start.y;
	vec.x = wall->end.x - wall->start.x;
	vec.y = wall->end.y - wall->start.y;
	result = vec2_point_to_line(point, orig, vec2_norm(vec));
	if (result.y >= wall->start.y && result.y <= wall->end.y)
	{
		if (result.x >= wall->start.x && result.x <= wall->end.x)
		{
			//draw_box(result, 5, 0xffff00ff, editor_back_buffer()->buff);
			confirm++;
		}
	}
	if (result.y >= wall->start.y && result.y <= wall->end.y)
	{
		if (result.x >= wall->end.x && result.x <= wall->start.x)
		{
			//draw_box(result, 5, 0xff00ffff, editor_back_buffer()->buff);
			confirm++;
		}
	}
	if (result.y >= wall->end.y && result.y <= wall->start.y)
	{
		if (result.x >= wall->start.x && result.x <= wall->end.x)
		{
			//draw_box(result, 5, 0xff00ff00, editor_back_buffer()->buff);
			confirm++;
		}
	}
	if (result.y >= wall->end.y && result.y <= wall->start.y)
	{
		if (result.x >= wall->end.x && result.x <= wall->start.x)
		{
			//draw_box(result, 5, 0xff0000ff, editor_back_buffer()->buff);
			confirm++;
		}
	}	
	editor_back_buffer()->rendering_on = 1;
	if (confirm)
	{
		positive_hit->x = result.x;
		positive_hit->y = result.y;
		return ((int)vec2_mag(vec2_sub(point, result)));
	}
	else
	{
		//draw_box(result, 5, 0xffffff00, editor_back_buffer()->buff);
		return (10000000);
	}
}

static int		closest_wall_id(t_model *mdl, t_point xy, t_point *winning_hit, t_point *positive_hit)
{
	t_wall	*wall;
	int		lowest_dist;
	int		dist;
	int		lowest_id;
	int		wc;

	wc = mdl->wall_count;
	wall = mdl->wall_first;
	lowest_dist = 200;
	lowest_id = -1;
	while (wc--)
	{
		dist = abs(distance_to_wall((t_xy){xy.x, xy.y}, wall, positive_hit));
		if (dist < lowest_dist)
		{
			//printf("dist to wall_id [%d] is less than to wall_id [%d]: [%d] vs [%d]\n", wall->id, lowest_id, dist, lowest_dist);
			winning_hit->x = positive_hit->x;
			winning_hit->y = positive_hit->y;			
			lowest_dist = dist;
			lowest_id = wall->id;
		}
		wall = wall->next;
	}
	return (lowest_id);
}

static int		already_postered_wall(int wall_id)
{
	t_effect	*effect;
	int			ec;

	effect = get_model()->effect_first;
	ec = get_model()->effect_count;
	while (ec--)
	{
		if (effect->type_id == EFFECT_POSTER && effect->target_id == wall_id)
			return (1);
		effect = effect->next;
	}
	return (0);
}

static int		effect_plant_poster(int x, int y)
{
	t_model		*mdl;
	t_effect	*new_effect;
	t_point		winning_hit;
	t_point		positive_hit;
	int			new_id;

	mdl = get_model(); 
	new_id = closest_wall_id(mdl, (t_point){x, y}, &winning_hit, &positive_hit);
	if (new_id != -1)
	{
		if (already_postered_wall(new_id))
		{
			puts("Disallowing placing more Poster Effectors on the same wall!");
			return (-1);
		}			
		mdl->effects->target.x = 50;
		mdl->effects->target.y = 50;
		mdl->effects->loc.x = winning_hit.x;
		mdl->effects->loc.y = winning_hit.y;
		mdl->effects->id = mdl->effect_count;
		mdl->effects->type_id = EFFECT_POSTER;
		mdl->effects->target_id = new_id;
		mdl->effects->activated = 0;
		mdl->effects->active_sprite = doom_ptr()->sprites->txt_poster_on;
		new_id = mdl->effects->id;
		new_effect = (t_effect*)malloc(sizeof(t_effect));
		if (!new_effect)
			ft_die("Fatal error: Could not malloc new_effect at effect_plant_poster!");
		mdl->effects->next = new_effect;
		mdl->effect_count++;
		if (mdl->effect_count == 1)
			mdl->effect_first = mdl->effects;
		mdl->effects = new_effect;
	}
	return (new_id);
}

static int		effect_plant_keypanel(int x, int y)
{
	t_model		*mdl;
	t_effect	*new_effect;
	t_point		winning_hit;
	t_point		positive_hit;
	int			new_id;

	mdl = get_model(); 
	new_id = closest_wall_id(mdl, (t_point){x, y}, &winning_hit, &positive_hit);
	if (new_id != -1)
	{
		mdl->effects->target.x = 50;
		mdl->effects->target.y = 50;
		mdl->effects->loc.x = winning_hit.x;
		mdl->effects->loc.y = winning_hit.y;
		mdl->effects->id = mdl->effect_count;
		mdl->effects->type_id = EFFECT_KEYPANEL;
		mdl->effects->target_id = new_id;
		mdl->effects->activated = 0;
		mdl->effects->active_sprite = doom_ptr()->sprites->txt_switch_on;
		new_id = mdl->effects->id;
		new_effect = (t_effect*)malloc(sizeof(t_effect));
		if (!new_effect)
			ft_die("Fatal error: Could not malloc new_effect at effect_plant_keypanel!");
		mdl->effects->next = new_effect;
		mdl->effect_count++;
		if (mdl->effect_count == 1)
			mdl->effect_first = mdl->effects;
		mdl->effects = new_effect;
	}
	return (new_id);
}

static int		light_pos(t_wall *wall)
{
	t_point diff;
	int		optimum;

	diff.x = wall->end.x - wall->start.x;
	diff.y = wall->end.y - wall->start.y;
	optimum = 2000 / (abs(diff.x) + abs(diff.y));
	if (optimum <= 1)
		return (2);
	if (optimum >= 100)
		return (99);
	return (optimum);
}

static int		already_lightknobbed_room(t_room *room)
{
	t_effect	*effect;
	int			ec;

	effect = get_model()->effect_first;
	ec = get_model()->effect_count;
	while (ec--)
	{
		if (effect->type_id == EFFECT_LIGHTKNOB)
		{
			if (room_by_wall_id(effect->target_id, get_model()) == room)
				return (1);
		}
		effect = effect->next;
	}
	return (0);
}

static int		effect_plant_lightknob(int x, int y)
{
	t_model		*mdl;
	t_effect	*new_effect;
	t_point		winning_hit;
	t_point		positive_hit;
	int			new_id;

	mdl = get_model(); 
	new_id = closest_wall_id(mdl, (t_point){x, y}, &winning_hit, &positive_hit);
	if (new_id != -1)
	{
		if (already_lightknobbed_room(room_by_wall_id(new_id, mdl)))
		{
			puts("Disallowing placing more Lightknob Effectors in the same room!");
			return (-1);
		}			
		mdl->effects->target.x = light_pos(wall_by_id(new_id));
		mdl->effects->target.y = 50;
		mdl->effects->loc.x = winning_hit.x;
		mdl->effects->loc.y = winning_hit.y;
		mdl->effects->id = mdl->effect_count;
		mdl->effects->type_id = EFFECT_LIGHTKNOB;
		mdl->effects->target_id = new_id;
		mdl->effects->activated = 0;
		mdl->effects->active_sprite = doom_ptr()->sprites->txt_switch_on;
		new_id = mdl->effects->id;
		new_effect = (t_effect*)malloc(sizeof(t_effect));
		if (!new_effect)
			ft_die("Fatal error: Could not malloc new_effect at effect_plant_lightswitch!");
		mdl->effects->next = new_effect;
		mdl->effect_count++;
		if (mdl->effect_count == 1)
			mdl->effect_first = mdl->effects;
		mdl->effects = new_effect;
	}
	return (new_id);
}

int				effect_dirs(int type)
{
	static int	dirs[3] = { DOWNWARD, UPWARD, UPWARD, UPWARD };

	return (dirs[type]);
}

uint32_t		effect_colors(int type)
{
	static uint32_t colors[4] = { COLOR_EFFECT_EXIT, COLOR_EFFECT_POSTER, COLOR_EFFECT_KEYPANEL, COLOR_EFFECT_LIGHTKNOB };

	return (colors[type]);
}

static void		parse_positive_two_digit_nbr(int *first, int *second, int n)
{
	int value;

	if (n > 99 || n < 10)
		return ;
	value = n;
	*second = value % 10;
	value = value / 10;
	*first = value;
}

static void		print_digits_for_effect_target_y(t_effect *effect, int y)
{
	int first;
	int second;
	t_point scroll;

	//rel = relative_position(effect->loc.x, effect->loc.y, get_state());
	scroll = scrolled_position(effect->loc.x, effect->loc.y, get_state());
	if (y < 1 || y > 99)
		return ;
	first = -1;
	second = -1;
	parse_positive_two_digit_nbr(&first, &second, y);
	if (second == 0)
		second = 10;
	if (y > 9)
		digit_to_buffer(editor_back_buffer()->buff, (t_point){scroll.x - 6, scroll.y + 12}, first * 10, COLOR_SELECTION_LINE);
	else
		digit_to_buffer(editor_back_buffer()->buff, (t_point){scroll.x, scroll.y + 12}, y * 10, COLOR_SELECTION_LINE);
	if (second != -1 && y > 9)
		digit_to_buffer(editor_back_buffer()->buff, (t_point){scroll.x + 6, scroll.y + 12}, second * 10, COLOR_SELECTION_LINE);	
	editor_back_buffer()->rendering_on = 1;
}

void			effect_plant(int x, int y)
{
	t_point		relative;
	int			result;
	t_effect	*new_effect;

	relative = relative_position(x, y, get_state());
	x = relative.x;
	y = relative.y;
	if (effect_logic()->plant_type == EFFECT_EXIT)
	{
		effect_plant_exit(x, y);
		effect_change_zoom(get_state());
	}		
	else if (effect_logic()->plant_type == EFFECT_POSTER)
	{
		result = effect_plant_poster(x, y);
		effect_change_zoom(get_state());
		if (result != -1)
		{
			//new_effect = effect_by_id(result);
			effect_logic()->last_plant_id = result;
			// TODO Instead figure out how to calculate target.X (reverse engineer effect_adjust for that?)
			effect_adjust(0, 0);
			//print_digits_for_effect_target_y(new_poster, new_poster->target.y);
		}				
	}
	else if (effect_logic()->plant_type == EFFECT_KEYPANEL)
	{
		result = effect_plant_keypanel(x, y);
		effect_change_zoom(get_state());
		if (result != -1)
		{
			effect_logic()->last_plant_id = result;
			effect_adjust(0, 0);
		}
	}
	else if (effect_logic()->plant_type == EFFECT_LIGHTKNOB)
	{
		result = effect_plant_lightknob(x, y);
		effect_change_zoom(get_state());
		if (result != -1)
		{
			effect_logic()->last_plant_id = result;
			effect_adjust(0, 0);
		}
	}
	get_state()->saving_choice = 0;	
}

t_logic 		*effect_logic(void)
{
	static		t_logic *logic = NULL;

	if (!logic)
	{
		logic = (t_logic*)malloc(sizeof(t_logic));
		if (!logic)
			ft_die("Fatal error: Could not malloc logic for effects at effect_logic");
		logic->plant_type = EFFECT_EXIT;
		logic->plant_dir = DOWNWARD;
		logic->plant = effect_plant;
		logic->swap_type = effect_swap_type;
		logic->sweep_counter = 0;
		logic->sweep[0].x = 0;
		logic->sweep[0].y = 0;
		logic->colors = effect_colors;
		logic->last_plant_id = -1;
	}
	return (logic);
}

static void		recalculate_effect_location(t_effect *effect)
{
	t_wall	*wall;
	t_xy	orig;
	t_xy	vec;
	t_xy	pct;
	t_xy	new;

	wall = wall_by_id(effect->target_id);
	orig.x = wall->start.x;
	orig.y = wall->start.y;
	vec.x = wall->end.x - wall->start.x;
	vec.y = wall->end.y - wall->start.y;
	pct = vec2_mul(vec2_norm(vec), (double)((double)effect->target.x / 100.0f) * vec2_mag(vec));
	new = vec2_add(orig, pct);
	effect->loc.x = (int)new.x;
	effect->loc.y = (int)new.y;
	effect_change_zoom(get_state());
	print_digits_for_effect_target_y(effect, effect->target.y);
}

void			effect_adjust(int x_or_y, int change)
{
	t_effect	*last;
	int			*target;

	if (get_state()->gui != mode_effect())
		return ;
	if (effect_logic()->last_plant_id == -1)
		return ;
	last = effect_by_id(effect_logic()->last_plant_id);
	if (x_or_y == 0)
		target = &(last->target.x);
	else
		target = &(last->target.y);
	*target += change;
	if (*target > 99)
		*target = 99;
	if (*target < 1)
		*target = 1;
	//puts("Adjusted Target with WSAD in Effects!");
	// printf("poster's new tx,ty = [%d,%d]\n", last->target.x, last->target.y);
	if (x_or_y == 0)
		recalculate_effect_location(last);
	else
	{
		effect_change_zoom(get_state());
		print_digits_for_effect_target_y(last, *target);
	}
}

void			effect_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	effect_change_zoom(state);
		//puts("Activating effect");
}

void			effect_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	effect_change_zoom(state);
		//puts("Deactivating effect");
}

void			effect_change_zoom(t_state *state)
{
	t_effect *highlighted_effect;

	redraw_editor_to_backbuffer(COLOR_LINE);
	if (effect_logic()->last_plant_id != -1)
	{
		highlighted_effect = effect_by_id(effect_logic()->last_plant_id);
		print_digits_for_effect_target_y(highlighted_effect, highlighted_effect->target.y);
	}
}

static void		effect_refresh_preview(void)
{
	int x;
	int y;

	SDL_GetMouseState(&x, &y);
	if (x > 0 && y > 0 && x < EDT_WIN_WIDTH && y < EDT_WIN_HEIGHT)
		effect_mouse_motion(x, y);
}

void 			effect_swap_type(void)
{
	if (effect_logic()->plant_type == EFFECT_EXIT && get_model()->effect_count > 0)
	{
		effect_logic()->plant_type = EFFECT_POSTER;
		effect_logic()->plant_dir = UPWARD;
	}
	else if (effect_logic()->plant_type == EFFECT_POSTER)
	{
		effect_logic()->plant_type = EFFECT_KEYPANEL;
		effect_logic()->plant_dir = UPWARD;
	}
	else if (effect_logic()->plant_type == EFFECT_KEYPANEL)
	{
		effect_logic()->plant_type = EFFECT_LIGHTKNOB;
		effect_logic()->plant_dir = UPWARD;
	}
	else if (effect_logic()->plant_type == EFFECT_LIGHTKNOB)
	{
		effect_logic()->plant_type = EFFECT_EXIT;
		effect_logic()->plant_dir = DOWNWARD;
	}
	effect_refresh_preview();
}

void 			effect_mouse_motion(int x, int y)
{
	static t_point	last_preview = {-1, -1};
	static int		last_dir = -1;

	if (last_preview.x != -1 && last_preview.y != -1)
	{
		preserving_triangle_to_buffer(doom_ptr()->edt->buff, last_preview,
			last_dir, 0xff000000);
	}
	unpreserving_triangle_to_buffer(doom_ptr()->edt->buff, (t_point){x, y},
		effect_logic()->plant_dir, effect_logic()->colors(effect_logic()->plant_type));
	last_preview.x = x;
	last_preview.y = y;
	last_dir = effect_logic()->plant_dir;
}

void 			effect_left_click(int x, int y)
{
	effect_logic()->plant(x,y);
}

void 			effect_right_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		effect_logic()->swap_type();
}

static void 	recalc_effect_ids(void)
{
	t_effect		*effect;
	int 		ec;
	int 		i;

	i = 0;
	effect = get_model()->effect_first;
	ec = get_model()->effect_count;
	while (ec--)
	{
		effect->id = i++;
		effect = effect->next;
	}
}

static void 	link_effect_by_id(int id, t_effect *linking_effect)
{
	t_effect	*effect;
	int			ec;

	if (id < 0)
		ft_die("Fatal error: Tried to link with invalid id at link_effect_by_id");
	effect = get_model()->effect_first;
	ec = get_model()->effect_count;
	while (ec--)
	{
		if (effect->id == id)
		{
			effect->next = linking_effect;
			return ;
		}
		effect = effect->next;
	}
	ft_putendl("Warning: Could not link effect by id, id did not match with an effect at link_effect_by_id!");
}

static void		remove_effect(int id)
{
	t_effect	*effect;
	int 		ec;

	if (id == 0)
		return ;
	effect = get_model()->effect_first;
	ec = get_model()->effect_count;
	while (ec--)
	{
		if (effect->id == id)
		{
			if (id == 0)
				get_model()->effect_first = effect->next;
			else
				link_effect_by_id(id - 1, effect->next);
			get_model()->effect_count--;
			free(effect);
			recalc_effect_ids();
			get_state()->saving_choice = 0;
			break ;
		}
		effect = effect->next;
	}
}

static void 	highlight_effect(t_effect *effect)
{
	t_point		rel_pos;

	if (effect->type_id == 0)
		return ;
	rel_pos = scrolled_position(effect->loc.x, effect->loc.y, get_state());
	unpreserving_triangle_to_buffer(editor_front_buffer()->buff, (t_point){rel_pos.x, rel_pos.y},
		effect_dirs(effect->type_id), COLOR_GRID_LINE);
	editor_front_buffer()->rendering_on = 1;
}

static int		which_overlapping_effect(int x, int y)
{
	t_point		rel;
	t_effect	*effect;
	int 		ec;

	rel = relative_position(x, y, get_state());
	effect = get_model()->effect_first;
	ec = get_model()->effect_count;
	while (ec--)
	{
		if (abs(rel.x - effect->loc.x) < 24 && abs(rel.y - effect->loc.y) < 24)
			return (effect->id);
		effect = effect->next;
	}
	return (-1);
}

void 			effect_middle_click(int x, int y)
{
	static int	last_id = -1;
	int 		curr_id;
	t_effect	*highlighted_effect;

	effect_change_zoom(get_state());
	wipe_editor_front_buffer(0x00000000);
	curr_id = which_overlapping_effect(x, y);
	if (curr_id == last_id && curr_id != 0)
	{
		effect_logic()->last_plant_id = -1;
		remove_effect(curr_id);
		last_id = -1;
		effect_change_zoom(get_state());
	}
	else
	{
		highlight_effect(effect_by_id(curr_id));
		last_id = curr_id;
		if (curr_id != -1 && curr_id != 0)
		{
			effect_logic()->last_plant_id = curr_id;
			highlighted_effect = effect_by_id(curr_id);
			print_digits_for_effect_target_y(highlighted_effect, highlighted_effect->target.y);
		}
	}
}
