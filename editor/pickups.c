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

//		TODO FOR PICKUPS.C - AMMO PICKUPS AND WEAPON PICKUPS
//			AMMO PICKUP / YELLOW RECT / NUMBER INSIDE // DONE
//			GUN PICKUP / GREEN RECT / NUMBER INSIDE // DONE
//				MISSING PERSISTENCE // DONE
//					CORRECT FOR PLANTING X, Y WITH SCROLL && ZOOMING // DONE
//					SYNC PLANTING X, Y WITH DRAWING X, Y // DONE
//				MISSING NUMBER INSIDE // DONE
//					IMPLEMENT A CUSTOM NUMBER PRINTER WITH TWO SIZES // DONE
//				MISSING SELECTION OF WEAPON TYPE // DONE
//					IMPLEMENT AT GET_STATE() LEVEL WITH KEYS 1..9 // DONE
//				MISSING INDICATOR OF CURRENT PLANT TYPE
//				MISSING INDICATOR OF CURRENT WEAPON TYPE
//					IMPLEMENT WITH PREVIEW // <-- YOU ARE HERE!!
//				MISSING BOX TO INFORM THE EDITOR OF WEAPON CHOICE KEYS 1..X
//				GRID SYSTEM ON TOP OF EVERYTHING ELSE
//					MODULO OFFSETTED BY SCROLL_X, SCROLL_Y, STEPPING DIVIDED BY ZOOMFACTOR
//					DRAW VERY LAST ON SCREEN AREA EXCLUDING INFO BOXES, BUT ONLY ON BLACK, STRAIGHT TO BUFFER
//					TOGGLE WITH G, VARY STEPPING WITH AN OPTION
//					DECOUPLED FROM EVERYTHING SO ITS AN EXTRA BONUS FEATURE WITHOUT COMPLEXITY OVERHEAD
//				PICKUPS DOUBLE CLICK DELETE
//				ROOMS SELECTION DEL KEY DELETE

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
			ft_die("Fatal error: Could not malloc logic for planting at planting_logic");
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
}

void			pickups_plant(int x, int y)
{
	if (pickups_logic()->plant_type == PICKUP_HEALTH)
		pickups_plant_health(x, y);
	else if (pickups_logic()->plant_type == PICKUP_AMMO)
		pickups_plant_ammo(x, y);
	else if (pickups_logic()->plant_type == PICKUP_WEAPON)
		pickups_plant_weapon(x, y);
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
		preserving_square_to_buffer(doom_ptr()->edt->buff, last_preview, PICKUP_RADIUS / get_state()->zoom_factor, pickups_logic()->colors(pickups_logic()->plant_type));
	unpreserving_square_to_buffer(doom_ptr()->edt->buff, (t_point){x, y}, PICKUP_RADIUS / get_state()->zoom_factor, pickups_logic()->colors(pickups_logic()->plant_type));
	last_preview.x = x;
	last_preview.y = y;
}

void 			pickups_left_click(int x, int y)
{
	pickups_logic()->plant(x, y);
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