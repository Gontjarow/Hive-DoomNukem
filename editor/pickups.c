#include "doom-nukem.h"

//			COMPLETED PICKUPS.C ADD HEALTH PICKUP / GREEN RECT / PLUS SIGN INSIDE // DONE
//				MISSING PERSISTENCE // DONE
//					CORRECT FOR PLANTING X, Y WITH SCROLL && ZOOMING // DONE
//					SYNC PLANTING X, Y WITH DRAWING X, Y // DONE
//				MISSING PLUS SIGN // DONE
//				MISSING ENCODING FROM MODEL TO MAP // DONE
//				MISSING DECODING FROM MAP TO MODEL // DONE
//				REORGANIZE PLANTING.C FILE // DONE
//				SEPARATE MODEL AND COMMONS FROM COMMONS TO DATA_MODEL AND COMMONS // DONE

//			COMPLETED PICKUPS.C - AMMO PICKUPS AND WEAPON PICKUPS
//				AMMO PICKUP / YELLOW RECT / NUMBER INSIDE // DONE
//				GUN PICKUP / GREEN RECT / NUMBER INSIDE // DONE
//				MISSING PERSISTENCE // DONE
//					CORRECT FOR PLANTING X, Y WITH SCROLL && ZOOMING // DONE
//					SYNC PLANTING X, Y WITH DRAWING X, Y // DONE
// 				MISSING NUMBER INSIDE // DONE
//					IMPLEMENT A CUSTOM NUMBER PRINTER WITH TWO SIZES // DONE
//				MISSING SELECTION OF WEAPON TYPE // DONE
//					IMPLEMENT AT GET_STATE() LEVEL WITH KEYS 1..9 // DONE

//			COMPLETED FOR PICKUPS.C - AMMO PICKUPS AND WEAPON PICKUPS
//					MISSING INDICATOR OF CURRENT PLANT TYPE
//					MISSING INDICATOR OF CURRENT WEAPON TYPE
//						IMPLEMENT WITH PREVIEW // DONE
//						DRAW ALSO A NUMBER WITH THE PREVIEWING // DONE
//						IMPLEMENT ALSO A PRESERVING/UNPRESERVING SQUARE/DIGIT // DONE

void			pickups_plant_health(int x, int y)
{
	int			radius;
	t_model		*mdl;
	t_pickup	*new_pickup;
	t_point		relative;

	radius = PICKUP_RADIUS / get_state()->zoom_factor;
	relative = relative_position(x, y, get_state());
	editor_back_buffer()->rendering_on = 1;
	square_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, radius, COLOR_HEALTH_PICKUP);
	cross_to_buffer(editor_back_buffer()->buff, (t_point) {x, y}, radius / 2, COLOR_HEALTH_PICKUP);
		//puts("Pickups debug: Mock drawing a square directly to buffer!");
	mdl = get_model();
	mdl->pickups->id = mdl->pickup_count;
	mdl->pickups->flavor = PICKUP_HEALTH;
	mdl->pickups->weapon_type_id = 0;
	mdl->pickups->loc.x = relative.x;
	mdl->pickups->loc.y = relative.y;
	mdl->pickups->active_sprite = doom_ptr()->sprites->txt_health_pickup;
	new_pickup = (t_pickup*)malloc(sizeof(t_pickup));
	if (!new_pickup)
		ft_die("Fatal error: Could not malloc new_pickup at pickups_plant_health!");
	mdl->pickups->next = new_pickup;
	mdl->pickup_count++;
	if (mdl->pickup_count == 1)
		mdl->pickup_first = mdl->pickups;
		//printf("Pickup id: %d | loc.x: %d | loc.y: %d | flavor: %d | weapon_type_id: %d\n",
		//mdl->pickups->id, mdl->pickups->loc.x, mdl->pickups->loc.y, mdl->pickups->flavor, mdl->pickups->weapon_type_id);
	mdl->pickups = new_pickup;
		//debug_model_pickups();
}

void			pickups_plant_ammo(int x, int y)
{
	int			radius;
	t_model		*mdl;
	t_pickup	*new_pickup;
	t_point		relative;

	radius = PICKUP_RADIUS / get_state()->zoom_factor;
	relative = relative_position(x, y, get_state());
	editor_back_buffer()->rendering_on = 1;
	square_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, radius, COLOR_AMMO_PICKUP);
	digit_to_buffer(editor_back_buffer()->buff, (t_point) {x, y}, radius == PICKUP_RADIUS ? get_state()->selected_weapon_type * 10 : get_state()->selected_weapon_type, COLOR_AMMO_PICKUP);
		//puts("Pickups debug: Mock drawing a square directly to buffer!");
	mdl = get_model();
	mdl->pickups->id = mdl->pickup_count;
	mdl->pickups->flavor = PICKUP_AMMO;
	mdl->pickups->weapon_type_id = get_state()->selected_weapon_type;
	mdl->pickups->loc.x = relative.x;
	mdl->pickups->loc.y = relative.y;
	if (mdl->pickups->weapon_type_id == ASSAULT_RIFLE)
		mdl->pickups->active_sprite = doom_ptr()->sprites->txt_assault_ammo_pickup;
	else if (mdl->pickups->weapon_type_id == SHOTGUN)
		mdl->pickups->active_sprite = doom_ptr()->sprites->txt_shotgun_ammo_pickup;
	else
		mdl->pickups->active_sprite = NULL;
	new_pickup = (t_pickup*)malloc(sizeof(t_pickup));
	if (!new_pickup)
		ft_die("Fatal error: Could not malloc new_pickup at pickups_plant_ammo!");
	mdl->pickups->next = new_pickup;
	mdl->pickup_count++;
	if (mdl->pickup_count == 1)
		mdl->pickup_first = mdl->pickups;
	//printf("Pickup id: %d | loc.x: %d | loc.y: %d | flavor: %d | weapon_type_id: %d\n",
	//mdl->pickups->id, mdl->pickups->loc.x, mdl->pickups->loc.y, mdl->pickups->flavor, mdl->pickups->weapon_type_id);
	mdl->pickups = new_pickup;
	//debug_model_pickups();
}

void			pickups_plant_weapon(int x, int y)
{
	int			radius;
	t_model		*mdl;
	t_pickup	*new_pickup;
	t_point		relative;

	radius = PICKUP_RADIUS / get_state()->zoom_factor;
	relative = relative_position(x, y, get_state());
	editor_back_buffer()->rendering_on = 1;
	square_to_buffer(editor_back_buffer()->buff, (t_point){x, y}, radius, COLOR_WEAPON_PICKUP);
	digit_to_buffer(editor_back_buffer()->buff, (t_point) {x, y}, radius == PICKUP_RADIUS ? get_state()->selected_weapon_type * 10 : get_state()->selected_weapon_type, COLOR_WEAPON_PICKUP);
		//puts("Pickups debug: Mock drawing a square directly to buffer!");
	mdl = get_model();
	mdl->pickups->id = mdl->pickup_count;
	mdl->pickups->flavor = PICKUP_WEAPON;
	mdl->pickups->weapon_type_id = get_state()->selected_weapon_type;
	mdl->pickups->loc.x = relative.x;
	mdl->pickups->loc.y = relative.y;
	mdl->pickups->active_sprite = NULL; // TODO: Where is this sprite?
	new_pickup = (t_pickup*)malloc(sizeof(t_pickup));
	if (!new_pickup)
		ft_die("Fatal error: Could not malloc new_pickup at pickups_plant_weapon!");
	mdl->pickups->next = new_pickup;
	mdl->pickup_count++;
	if (mdl->pickup_count == 1)
		mdl->pickup_first = mdl->pickups;
	//printf("Pickup id: %d | loc.x: %d | loc.y: %d | flavor: %d | weapon_type_id: %d\n",
	//mdl->pickups->id, mdl->pickups->loc.x, mdl->pickups->loc.y, mdl->pickups->flavor, mdl->pickups->weapon_type_id);
	mdl->pickups = new_pickup;
	//debug_model_pickups();
}

uint32_t		pickups_colors(int type)
{
	static uint32_t colors[3] = { COLOR_HEALTH_PICKUP, COLOR_AMMO_PICKUP, COLOR_WEAPON_PICKUP };

	return (colors[type]);
}

t_logic 		*pickups_logic(void)
{
	static		t_logic *logic = NULL;

	if (!logic)
	{
		logic = (t_logic*)malloc(sizeof(t_logic));
		if (!logic)
			ft_die("Fatal error: Could not malloc logic for planting at pickups_logic");
		logic->plant_type = PICKUP_HEALTH;
		logic->plant = pickups_plant;
		logic->swap_type = pickups_swap_type;
		logic->sweep_counter = 0;
		logic->sweep[0].x = 0;
		logic->sweep[0].y = 0;
		logic->colors = pickups_colors;
	}
	return (logic);
}

void 			pickups_swap_type(void)
{
	if (pickups_logic()->plant_type == PICKUP_HEALTH)
		pickups_logic()->plant_type = PICKUP_AMMO;
	else if (pickups_logic()->plant_type == PICKUP_AMMO)
		pickups_logic()->plant_type = PICKUP_WEAPON;
	else if (pickups_logic()->plant_type == PICKUP_WEAPON)
		pickups_logic()->plant_type = PICKUP_HEALTH;
	pickups_refresh_preview();
}

void 			pickups_refresh_preview(void)
{
	int x;
	int y;

	SDL_GetMouseState(&x, &y);
	if (x > 0 && y > 0 && x < EDT_WIN_WIDTH && y < EDT_WIN_HEIGHT)
	{
		pickups_mouse_motion(x, y);
			//puts("Updated pickups weapon type preview after swapping weapon type in editor!");
	}
}

void			pickups_plant(int x, int y)
{
	if (pickups_logic()->plant_type == PICKUP_HEALTH)
		pickups_plant_health(x, y);
	else if (pickups_logic()->plant_type == PICKUP_AMMO)
		pickups_plant_ammo(x, y);
	else if (pickups_logic()->plant_type == PICKUP_WEAPON)
		pickups_plant_weapon(x, y);
	get_state()->saving_choice = 0;
}

void			pickups_activate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	pickups_change_zoom(state);
		//puts("Activating pickups");
}

void			pickups_deactivate(t_state *state)
{
	state->job_running = 0;
	state->job_abort = 0;
	state->thread_hit = 0;
	state->thread_color = 0xffff0000;
	state->thread_permission = 0;
	state->thread_target_id = -1;
	pickups_change_zoom(state);
		//puts("Deactivating pickups");
}

void			pickups_change_zoom(t_state *state)
{
	redraw_editor_to_backbuffer(COLOR_LINE);
}

void 			pickups_mouse_motion(int x, int y)
{
	static t_point	last_preview = {-1, -1};

	if (last_preview.x != -1 && last_preview.y != -1)
	{
		preserving_square_to_buffer(doom_ptr()->edt->buff, last_preview, PICKUP_RADIUS / get_state()->zoom_factor,
									pickups_logic()->colors(pickups_logic()->plant_type));
		// Here, check if its better to do "preserving" variations of cross_to_buffer and digit_to_buffer
		if (pickups_logic()->plant_type == PICKUP_HEALTH)
			preserving_cross_to_buffer(doom_ptr()->edt->buff, last_preview, PICKUP_RADIUS / get_state()->zoom_factor / 2, 0xff000000);
		else
			digit_to_buffer_ptr(doom_ptr()->edt->buff, last_preview, get_state()->selected_weapon_type * 10, 0xff000000, preserve_render_line);
	}
	// Here, check if its better to do "unpreserving" variations of cross_to_buffer and digit_to_buffer
	unpreserving_square_to_buffer(doom_ptr()->edt->buff, (t_point){x, y}, PICKUP_RADIUS / get_state()->zoom_factor, pickups_logic()->colors(pickups_logic()->plant_type));
	if (pickups_logic()->plant_type == PICKUP_HEALTH)
		unpreserving_cross_to_buffer(doom_ptr()->edt->buff, (t_point){x, y}, PICKUP_RADIUS / get_state()->zoom_factor / 2, pickups_logic()->colors(PICKUP_HEALTH));
	else
		digit_to_buffer_ptr(doom_ptr()->edt->buff, (t_point){x, y}, get_state()->selected_weapon_type * 10, pickups_logic()->colors(pickups_logic()->plant_type), unpreserve_render_line);
	last_preview.x = x;
	last_preview.y = y;
}

static void 	link_pickup_by_id(int id, t_pickup *linking_pickup)
{
	t_pickup	*pickup;
	int			pc;

	if (id < 0)
		ft_die("Fatal error: Tried to link with invalid id at link_pickup_by_id");
	pickup = get_model()->pickup_first;
	pc = get_model()->pickup_count;
	while (pc--)
	{
		if (pickup->id == id)
		{
			pickup->next = linking_pickup;
			return ;
		}
		pickup = pickup->next;
	}
	ft_putendl("Warning: Could not link pickup by id, id did not match with an enemy at link_pickup_by_id!");
}

static void 	recalc_pickup_ids(void)
{
	t_pickup	*pickup;
	int 		pc;
	int 		i;

	i = 0;
	pickup = get_model()->pickup_first;
	pc = get_model()->pickup_count;
	while (pc--)
	{
		pickup->id = i++;
		pickup = pickup->next;
	}
}

static int 		remove_pickup(int id)
{
	t_pickup	*pickup;
	int 		pc;

	puts("Removing a pickup! Double click detected!");
	pickup = get_model()->pickup_first;
	pc = get_model()->pickup_count;
	while (pc--)
	{
		if (pickup->id == id)
		{
			if (id == 0)
				get_model()->pickup_first = pickup->next;
			else
				link_pickup_by_id(id - 1, pickup->next);
			get_model()->pickup_count--;
			free(pickup);
			recalc_pickup_ids();
			get_state()->saving_choice = 0;
			break ;
		}
		pickup = pickup->next;
	}
}

static int		which_overlapping_pickups(int x, int y)
{
	t_point		rel;
	t_pickup	*pickup;
	int 		pc;

	rel = relative_position(x, y, get_state());
	if (abs((int)get_model()->player.x - rel.x) < 24 && abs((int)get_model()->player.y - rel.y) < 24)
		return (-2);
	pickup = get_model()->pickup_first;
	pc = get_model()->pickup_count;
	while (pc--)
	{
		if (abs(rel.x - pickup->loc.x) < 24 && abs(rel.y - pickup->loc.y) < 24)
			return (pickup->id);
		pickup = pickup->next;
	}
	return (-1);
}

void 			pickups_left_click(int x, int y)
{
	static int	last_id = -1;
	int 		curr_id;

	curr_id = which_overlapping_pickups(x, y);

	if (curr_id == -2)
	{
		last_id = -1;
		return ;
	}
	else if (curr_id == -1)
		pickups_logic()->plant(x, y);
	else if (curr_id == last_id)
	{
		remove_pickup(curr_id);
		pickups_change_zoom(get_state());
		last_id = -1;
	}
	else
		last_id = curr_id;
}

void 			pickups_right_click(int x, int y)
{
	if (x >= 0 && y >= 0)
		pickups_logic()->swap_type();
}

void 			pickups_middle_click(int x, int y)
{
	if (x == 12345678)
		y = 1;
	puts("Pickups mouse middle click!");
}
