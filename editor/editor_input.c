#include "doom-nukem.h"

void			edt_mouse_motion(t_doom *doom)
{
	t_state		*state;

	state = get_state();
	if (state->gui->has_motion)
		state->gui->motion(doom->event.motion.x, doom->event.motion.y);
}

void			edt_mouse_down(t_doom *doom)
{
	t_state		*state;

	state = get_state();
	if (doom->event.type == SDL_MOUSEWHEEL)
	{
		edt_cycle_mode(state);
		return ;
	}
	if (doom->event.button.button == SDL_BUTTON_LEFT)
		state->gui->left_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_MIDDLE)
		state->gui->middle_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
		state->gui->right_click(doom->event.button.x, doom->event.button.y);
}

void			edt_keystate_input(t_doom *doom)
{
	static int	lock_w = 0;
	static int	lock_e = 0;
	static int	lock_r = 0;
	static int	lock_d = 0;
	static int 	lock_f = 0;
	static int	lock_z = 0;
	static int	lock_x = 0;
	static int	lock_p = 0;
	static int	lock_g = 0;
	static int 	lock_t = 0;
	static int 	lock_b = 0;
	static int	lock_del = 0;
	static int	scancodes[8] = { SDL_SCANCODE_1 , SDL_SCANCODE_2 , SDL_SCANCODE_3,
	SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7,
	SDL_SCANCODE_8, SDL_SCANCODE_9};

	handle_keyboard_scrolling(doom);

	if (lock_w && !doom->keystates[SDL_SCANCODE_W])
		lock_w = 0;
	else if (doom->keystates[SDL_SCANCODE_W] && !lock_w)
	{
		debug_model_player();
		debug_model_enemies();
		debug_model_walls();
		debug_model_rooms();
		lock_w = 1;
	}

	if (lock_e)
		lock_e--;
	else if (doom->keystates[SDL_SCANCODE_E] && !lock_e)
	{
		select_roof(-1);
		lock_e = 4;
	}

	if (lock_r)
		lock_r--;
	else if (doom->keystates[SDL_SCANCODE_R] && !lock_r)
	{
		select_roof(1);
		lock_r = 4;
	}

	if (lock_d)
		lock_d--;
	else if (doom->keystates[SDL_SCANCODE_D] && !lock_d)
	{
		select_floor(-1);
		lock_d = 4;
	}

	if (lock_f)
		lock_f--;
	else if (doom->keystates[SDL_SCANCODE_F] && !lock_f)
	{
		select_floor(1);
		lock_f = 4;
	}

	if (lock_z && !doom->keystates[SDL_SCANCODE_Z])
		lock_z = 0;
	else if (doom->keystates[SDL_SCANCODE_Z] && !lock_z)
	{
		edt_inward_zoom();
		lock_z = 1;
	}

	if (lock_x && !doom->keystates[SDL_SCANCODE_X])
		lock_x = 0;
	else if (doom->keystates[SDL_SCANCODE_X] && !lock_x)
	{
		edt_outward_zoom();
		lock_x = 1;
	}

	if (lock_p && !doom->keystates[SDL_SCANCODE_P])
		lock_p = 0;
	else if (doom->keystates[SDL_SCANCODE_P] && !lock_p)
	{
		debug_model_pickups();
		lock_p = 1;
	}

	if (lock_g && !doom->keystates[SDL_SCANCODE_G])
		lock_g = 0;
	else if (doom->keystates[SDL_SCANCODE_G] && !lock_g)
	{
		get_state()->grid_on = !(get_state()->grid_on);
			//printf("get_state->grid_on is %d\n", get_state()->grid_on);
		get_state()->gui->change_zoom(get_state());
		lock_g = 1;
	}

	if (lock_t && !doom->keystates[SDL_SCANCODE_T])
		lock_t = 0;
	else if (doom->keystates[SDL_SCANCODE_T] && !lock_t)
	{
		if (get_state()->grid_size < 64)
			get_state()->grid_size *= 2;
			//printf("get_state->grid_size is %d\n", get_state()->grid_size);
		get_state()->gui->change_zoom(get_state());
		lock_t = 1;
	}

	if (lock_b && !doom->keystates[SDL_SCANCODE_B])
		lock_b = 0;
	else if (doom->keystates[SDL_SCANCODE_B] && !lock_b)
	{
		if (get_state()->grid_size > 16)
			get_state()->grid_size /= 2;
			//printf("get_state->grid_size is %d\n", get_state()->grid_size);
		get_state()->gui->change_zoom(get_state());
		lock_b = 1;
	}

	int i = -1;
	while (i++ < 8)
	{
		if (doom->keystates[scancodes[i]])
		{
			get_state()->selected_weapon_type = (i + 1);
			if (get_state()->gui == mode_pickups())
				pickups_refresh_preview();
			break ;
		}
	}

	if (lock_del && !doom->keystates[SDL_SCANCODE_DELETE])
		lock_del = 0;
	else if (doom->keystates[SDL_SCANCODE_DELETE] && !lock_del)
	{
		if (get_state()->gui == mode_select())
		{
				//puts("Select Room mode, DEL key pressed!");
			select_delete_room();
		}
		lock_del = 1;
	}
}

