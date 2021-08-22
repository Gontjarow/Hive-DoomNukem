/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 19:11:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/08/22 15:05:58 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (doom->event.wheel.y > 0)
			edt_cycle_mode_forward(state);
		else
			edt_cycle_mode_backward(state);
		return ;
	}
	if (doom->event.button.button == SDL_BUTTON_LEFT)
		state->gui->left_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_MIDDLE)
		state->gui->middle_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
		state->gui->right_click(doom->event.button.x, doom->event.button.y);
}

static void		handle_regular_numbers(t_doom *doom)
{
	int i;
	static int	scancodes[3] = { SDL_SCANCODE_1 , SDL_SCANCODE_2 , SDL_SCANCODE_3 };

	i = 0;
	while (i < 3)
	{
		if (doom->keystates[scancodes[i++]])
		{
			if (get_state()->gui == mode_select())
			{
				if (select_logic()->selected_portal_id == -1)
					return ;
				if (i == 0 || i > 3)
					return ;
				portal_by_id(select_logic()->selected_portal_id)->portal_type = i;
				printf("Selected portal_type [%d] for portal_id [%d] (1=REGULAR 2=DOOR 3=WINDOW)\n", i, select_logic()->selected_portal_id);
				select_change_zoom(get_state());
				return ;
			}
			if (get_state()->gui == mode_planting())
			{
				if (i == 0 || i > 3)
					return ;
				get_state()->selected_ai_type = i - 1;
				printf("Selected ai_type [%d] for planting (1=RANGED 2=MELEE 3=BOSS)\n", i);
				planting_change_zoom(get_state());
				return ;
			}
			get_state()->selected_weapon_type = i;
			printf("Selected type [%d] for pickups (1=SHOTGUN 2=ASSAULT RIFLE 3=JET PACK)\n", get_state()->selected_weapon_type);
			if (get_state()->gui == mode_pickups())
				pickups_refresh_preview();
			break ;
		}
	}
}

// TODO: Instead of sprite_id, use for room texture_id

static void 	handle_shifted_numbers(t_doom *doom)
{
	int i;
	static int	scancodes[9] = { SDL_SCANCODE_1 , SDL_SCANCODE_2 , SDL_SCANCODE_3,
								   SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7,
								   SDL_SCANCODE_8, SDL_SCANCODE_9 };

	i = 0;
	while (i < 9)
	{
		if (doom->keystates[scancodes[i++]])
		{
			get_state()->selected_sprite_id = (i);
			printf("Shift selected [%d] using handle_shifted_numbers in editor_input!\n", get_state()->selected_sprite_id);
			break ;
		}
	}
}

void			edt_keystate_input(t_doom *doom)
{
	static int	lock_i = 0;
	static int	lock_q = 0;
	static int	lock_w = 0;
	static int	lock_s = 0;
	static int	lock_a = 0;
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
	static int	lock_c = 0;
	static int	lock_del = 0;

	handle_keyboard_scrolling(doom);

	if (lock_q && !doom->keystates[SDL_SCANCODE_Q])
		lock_q = 0;
	else if (doom->keystates[SDL_SCANCODE_Q] && !lock_q)
	{
		get_state()->give_ceiling_to_rooms = !(get_state()->give_ceiling_to_rooms);
		if (get_state()->give_ceiling_to_rooms)
			printf("Editor assigning Rooms to have ceilings (toggled room->has_ceiling = 1)\n");
		else
			printf("Editor assigning Rooms to not have ceilings (toggled room->has_ceiling = 0)\n");
		lock_q = 1;
	}

	if (lock_i && !doom->keystates[SDL_SCANCODE_I])
		lock_i = 0;
	if (doom->keystates[SDL_SCANCODE_I] && !lock_i)
	{
		debug_model_player();
		debug_model_enemies();
		debug_model_walls();
		debug_model_rooms();
		debug_model_effects();
		debug_model_chain();
		debug_model_pickups();
		lock_i = 1;
	}
	if (lock_w)
		lock_w--;
	else if (doom->keystates[SDL_SCANCODE_W] && !lock_w)
	{
		effect_adjust(1, 1);
		lock_w = 1;
	}
	if (lock_s)
		lock_s--;
	else if (doom->keystates[SDL_SCANCODE_S] && !lock_s)
	{
		effect_adjust(1, -1);
		lock_s = 1;
	}
		
	if (lock_a)
		lock_a--;
	else if (doom->keystates[SDL_SCANCODE_A] && !lock_a)
	{
		effect_adjust(0, -1);
		lock_a = 1;
	}

	if (lock_d)
		lock_d--;
	else if (doom->keystates[SDL_SCANCODE_D] && !lock_d)
	{
		effect_adjust(0, 1);
		select_floor(-1);
		lock_d = 1;
		if (get_state()->gui == mode_select())
			lock_d = 4;
	}

	if (lock_c && !doom->keystates[SDL_SCANCODE_C])
		lock_c = 0;
	else if (doom->keystates[SDL_SCANCODE_C] && !lock_c)
	{
		edt_chaining_input_loop(doom);
		lock_c = 1;
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
		get_state()->gui->change_zoom(get_state());
		lock_g = 1;
	}

	if (lock_t && !doom->keystates[SDL_SCANCODE_T])
		lock_t = 0;
	else if (doom->keystates[SDL_SCANCODE_T] && !lock_t)
	{
		if (get_state()->grid_size < 64)
			get_state()->grid_size *= 2;
		get_state()->gui->change_zoom(get_state());
		lock_t = 1;
	}

	if (lock_b && !doom->keystates[SDL_SCANCODE_B])
		lock_b = 0;
	else if (doom->keystates[SDL_SCANCODE_B] && !lock_b)
	{
		if (get_state()->grid_size > 16)
			get_state()->grid_size /= 2;
		get_state()->gui->change_zoom(get_state());
		lock_b = 1;
	}

	if (lock_del && !doom->keystates[SDL_SCANCODE_DELETE])
		lock_del = 0;
	else if (doom->keystates[SDL_SCANCODE_DELETE] && !lock_del)
	{
		if (get_state()->gui == mode_select())
		{
			select_delete_room();
		}
		lock_del = 1;
	}

	if (doom->keystates[SDL_SCANCODE_LSHIFT] || doom->keystates[SDL_SCANCODE_RSHIFT])
		return (handle_shifted_numbers(doom));
	else
		return (handle_regular_numbers(doom));
}
