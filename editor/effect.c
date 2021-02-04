#include "doom-nukem.h"

static void 	effect_plant_exit(int x, int y)
{
	t_model		*mdl;
	t_effect	*new_effect;
	t_point		relative;

	relative = relative_position(x, y, get_state());
	editor_back_buffer()->rendering_on = 1;
	unpreserving_triangle_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, DOWNWARD, COLOR_EFFECT_EXIT);
	mdl = get_model();
	mdl->effects->id = mdl->effect_count;
	mdl->effects->type_id = EFFECT_EXIT;
	mdl->effects->loc.x = relative.x;
	mdl->effects->loc.y = relative.y;
	mdl->effects->target.x = 0;
	mdl->effects->target.y = 0;
	mdl->effects->target_id = 0;
	new_effect = (t_effect*)malloc(sizeof(t_effect));
	if (!new_effect)
		ft_die("Fatal error: Could not malloc new_effect at effect_plant_exit!");
	mdl->effects->next = new_effect;
	mdl->effect_count++;
	if (mdl->effect_count == 1)
		mdl->effect_first = mdl->effects;
	mdl->effects = new_effect;
	debug_model_effects();
}

static void 	effect_plant_key(int x, int y)
{
	//plant key effector
}

static void 	effect_plant_light(int x, int y)
{
	//plant light effector
}

uint32_t		effect_colors(int type)
{
	static uint32_t colors[3] = { COLOR_EFFECT_EXIT, COLOR_EFFECT_KEY, COLOR_EFFECT_LIGHT };

	return (colors[type]);
}

void			effect_plant(int x, int y)
{
	if (effect_logic()->plant_type == EFFECT_EXIT)
		effect_plant_exit(x, y);
	else if (effect_logic()->plant_type == EFFECT_KEY)
		effect_plant_key(x, y);
	else if (effect_logic()->plant_type == EFFECT_LIGHT)
		effect_plant_light(x, y);
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
	}
	return (logic);
}

void			effect_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	select_change_zoom(state);
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
	select_change_zoom(state);
		//puts("Deactivating effect");
}

void			effect_change_zoom(t_state *state)
{
	redraw_editor_to_backbuffer(COLOR_LINE);
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
	if (effect_logic()->plant_type == EFFECT_EXIT)
	{
		effect_logic()->plant_type = EFFECT_KEY;
		effect_logic()->plant_dir = UPWARD;
	}
	else if (effect_logic()->plant_type == EFFECT_KEY)
	{
		effect_logic()->plant_type = EFFECT_LIGHT;
		effect_logic()->plant_dir = DOWNWARD;
	}
	else if (effect_logic()->plant_type == EFFECT_LIGHT)
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

void 			effect_middle_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		puts("Effect middle click..!");
}

