/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c		                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 15:11:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/06/11 15:12:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

// TODO BIG NOTE TO SELF
//  EDITOR MUST GUARANTEE CLOCKWISE ARRANGEMENT OF WALL NODES IN ROOMS
//  https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order/

// COMPLETED
//		EXPLORE DIFFERENT RESOLUTIONS IN EDITOR WINDOW // DONE
//		LET USER SCROLL UP AND DOWN IN THE EDITOR WITH ARROW KEYS // DONE
//		Stage 1: Offset_x, Offset_y, Live in same memory space as Zoom_factor // DONE
//		Stage 2: These offsets can be between 0-(Max limit depending on Zoom-factor) both X, Y
//		Stage 3: For Zoom Factor 1: Between 0 and 3*EDT_WINDOW_WIDTH / HEIGHT, X / Y
//								 2: Between 0 and 2*EDT_WINDOW_WIDTH / HEIGHT, X / Y
//								 4: Between 0 and 0, X, Y // ... DONE
//		Stage 4: For rendering the correct space displace all walls' X&Y values by the offsets // DONE
//		Stage 5: Test if this works as imagined // DONE
//		Stage 6: Debug the program to detect where it is crashing and why, and fix that // TECH DEBT
//		I think it is crashing because sometimes it infinite loops during Cohen-Sutherland when
//		the clipped line crosses two segments? Possibly? It should be protected, aborted, modified to
//		escape such scenario or resolve it correctly...
//		Solution for now: Add a limit of 10 loops before autoexiting the infinite looping part. // DONE
//		Stage 7: Adjust the sensitivity and acceleration of the keyboard control // DONE
//		Stage 8: Add visual scrollbars to let user know the amount of scrolling applied // DONE
//			Technical debt: Can be done with XPM graphics propably later, implemented with linedraws.


// TODO NEXT FEATURE
//		MODE TO ADD ENEMY AND PLAYER OBJECTS
//		ENABLE RECORDING OF ROOMS AGAIN
//		ENABLE SAVING TO MAPFILE AGAIN
//		MODE TO PORTALIZE A WALL AND EXTEND A NEW ROOM FROM IT

/* trigger_protection() is a notifier function called by set_pixel_safe() when
 * overwriting to a buffer over a set_protected_color() has occured. Triggers
 * are accumulated by the calls 'trigger_protection(0)'. Decoupled from this,
 * elsewhere in the code a has_protection = 1 enabled eg. mode_polydraw()
 * can implement a periodic or event based check wheter overwriting happened
 * by invoking 'trigger_protection(1)'. If yes, this protection ATM (stc)
 * calls a clean slate for editor back buffer by a wipe and a walls redraw  */

// TODO: Utilize trigger_protection to detect illegal wall draws or room draws
//  (creation of intersection (overlapping) walls should not be possible

// TODO MOVE THIS SOMEWHERE COMMONPLACE
void 				trigger_protection(int clear)
{
	static int triggers = 0;

	if (clear)
	{
		if (triggers == 0)
			return ;
		triggers = 0;
			//ft_putendl("Triggered clearance commisioned.");
		wipe_editor_back_buffer(0xff000000);
		x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, 0xffffffff);
		return ;
	}
	triggers++;
}

/* mode_polydraw() returns as a a singleton the properties for the polydrawing mode.
 * It has a mouse motion function enabled, and the clicks. It protects overwriting
 * to white color and redraws when necessary (if overwriting has occured).
 * It is a t_gui struct that lives in get_state()->gui. The polydraw_* functions
 * are elsewhere and live in polydraw_status(), that handles the Status of the mode
 * during its activation. When polydrawing ends (is replaced by another t_gui mode
 * or the editor exits) it should know how to reset and clean itself up by
 * polydraw_status()->reset(polydraw_status());*/

// TODO MOVE THIS SOMEWHERE MODE_* PLACE

static t_gui		*mode_planting()
{
	static t_gui	*planting = NULL;

	if (!planting)
	{
		planting = (t_gui*)malloc(sizeof(t_gui));
		if (!planting)
			ft_die("Fatal error: Could not malloc t_gui at mode_planting.");
		planting->activate = planting_activate;
		planting->deactivate = planting_deactivate;
		planting->change_zoom = planting_change_zoom;
		planting->left_click = planting_left_click;
		planting->right_click = planting_right_click;
		planting->middle_click = planting_middle_click;
		planting->motion = planting_mouse_motion;
		planting->has_motion = 1;
		set_protected_color(0xffffffff);
	}
	return (planting);
}
static t_gui        *mode_polydraw()
{
	static t_gui	*polydraw = NULL;

	if (!polydraw)
	{
		polydraw = (t_gui*)malloc(sizeof(t_gui));
		if (!polydraw)
			ft_die("Fatal error: Could not malloc t_gui at mode_polydraw.");
        polydraw->activate = polydraw_activate;
		polydraw->deactivate = polydraw_deactivate;
        polydraw->change_zoom = polydraw_change_zoom;
		polydraw->left_click = polydraw_left_click;
		polydraw->right_click = polydraw_right_click;
		polydraw->middle_click = polydraw_middle_click;
		polydraw->motion = polydraw_mouse_motion;
		polydraw->has_motion = 1;
		set_protected_color(0xffffffff);
	}
	return (polydraw);
}

// TODO MOVE ZOOM_XPM, NODE_XPM AND PRINT_MODE_INFO SOMEWHERE COMMONPLACE

SDL_Surface			*zoom_xpm(int factor)
{
	static SDL_Surface *normal_zoom = NULL;
	static SDL_Surface *double_zoom = NULL;
	static SDL_Surface *triple_zoom = NULL;
	static SDL_Surface *quad_zoom = NULL;
	if (factor == 1)
		return (normal_zoom == NULL ? normal_zoom = xpm2surface("img/edt/normal_zoom.xpm") : normal_zoom);
	if (factor == 2)
		return (double_zoom == NULL ? double_zoom = xpm2surface("img/edt/double_zoom.xpm") : double_zoom);
	if (factor == 3)
		return (triple_zoom == NULL ? triple_zoom = xpm2surface("img/edt/triple_zoom.xpm") : triple_zoom);
	if (factor == 4)
		return (quad_zoom == NULL ? quad_zoom = xpm2surface("img/edt/quad_zoom.xpm") : quad_zoom);
	ft_die("Fatal error: Could not return zoom_xpm.");
	return (NULL);
}

SDL_Surface			*mode_xpm(t_gui *mode)
{
	static SDL_Surface *polydraw_xpm = NULL;
	static SDL_Surface *planting_xpm = NULL;

	if (mode == mode_polydraw())
		return (polydraw_xpm == NULL ? polydraw_xpm = xpm2surface("img/edt/wall_drawing.xpm") : polydraw_xpm);
	if (mode == mode_planting())
		return (planting_xpm == NULL ? planting_xpm = xpm2surface("img/edt/planting.xpm") : planting_xpm);
	ft_die("Fatal error: Could not return mode_xpm.");
	return (NULL);
}

// TODO: Consider alternative to SDL_Blit / SDL_Rect ?
void                print_mode_info(t_gui *mode)
{
	SDL_Surface *mode_surface;
    SDL_Surface *zoom_surface;
    SDL_Rect 	place;

	mode_surface = mode_xpm(mode);
    zoom_surface = zoom_xpm(get_state()->zoom_factor);
    if (!mode_surface || !zoom_surface)
    	ft_die("Fatal error: print_mode_info failed to retrieve mode/zoom surfaces.");
    place.w = mode_surface->w;
    place.h = mode_surface->h;
	place.x = zoom_surface->w + 10;
	place.y = EDT_WIN_HEIGHT - (place.h + 2) - 10;
	SDL_BlitSurface(mode_surface, NULL, editor_back_buffer()->buff, &place);
	place.w = zoom_surface->w;
	place.h = zoom_surface->h;
	place.x -= zoom_surface->w + 5;
	SDL_BlitSurface(zoom_surface, NULL, editor_back_buffer()->buff, &place);
    editor_back_buffer()->rendering_on = 1;
}

/* get_state returns the overall "State" of the editor. In the field state->gui,
 * you can assign a GUI mode, that are retrieved from their Singleton functions.
 * For example, for the get_state() initial state, state->gui = mode_polydraw();
 * assigns the GUI's action state to polydraw mode, which is a mode that draws
 * polygons using mouse clicks. By following the mode_polydraw() trail, you can
 * explore and find out how the polydraw_* functions are organized and coded. */

// TODO MOVE THIS SOMEWHERE GET_STATE RELATED PLACE
t_state				*get_state(void)
{
	static t_state	*state = NULL;

	if (!state)
	{
		state = (t_state*)malloc(sizeof(t_state));
		if (!state)
			ft_die("Fatal error: Could not malloc state struct at get_state");
		state->gui = mode_planting();//Usually mode_polydraw()
		state->zoom_factor = 1;
		state->scroll_x = 0;
		state->scroll_y = 0;
		state->gui->activate(state);
		//print_mode_info(state->gui);
		//draw_scroll_bars_to_backbuffer(state);
	}
	    //printf("States address returned: %p\n", (void*)state);
	return (state);
}

void				init_edt(t_doom *doom)
{
	doom->edt = (t_editor*)malloc(sizeof(t_editor));
	if (!doom->edt)
		ft_die("Fatal error: Mallocing level editor struct failed at init_edt.");
	doom->edt->parent = doom;
	doom->edt->win = SDL_CreateWindow("DoomNukem Level Editor", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 0);
	if (!doom->edt->win)
		ft_die("Fatal error: Could not create DoomNukem Level Editor window.");
	doom->edt->buff = SDL_GetWindowSurface(doom->edt->win);
	if (!doom->edt->buff)
		ft_die("Fatal error: Could not retrieve buffer of Level Editor window.");
	flood_buffer(doom->edt->buff, 0xff000000);
	doom->edt_state = get_state();
}

void				destroy_edt(t_doom *doom)
{
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

static void 		edt_swap_mode(t_state *state)
{
	//ft_putendl("Deactivating GUI mode");
	state->gui->deactivate(state);
	if (state->gui == mode_polydraw())
		state->gui = mode_planting();
	else if (state->gui == mode_planting())
		state->gui = mode_polydraw();
	//ft_putendl("Activating GUI mode");
	state->gui->activate(state);
}

// TODO MOVE THESE FIVE BELOW FUNCTIONS TO EDT_INPUT
void				edt_mouse_motion(t_doom *doom)
{
	t_state			*state;

	state = doom_ptr()->edt_state;
	if (state->gui->has_motion)
		state->gui->motion(doom->event.motion.x, doom->event.motion.y);
}

void				edt_mouse_down(t_doom *doom)
{
	t_state			*state;

	state = doom_ptr()->edt_state;
	if (doom->event.type == SDL_MOUSEWHEEL)
	{
		edt_swap_mode(state);
		return ;
	}
	if (doom->event.button.button == SDL_BUTTON_LEFT)
		state->gui->left_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_MIDDLE)
		state->gui->middle_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
		state->gui->right_click(doom->event.button.x, doom->event.button.y);
}

static void			edt_outward_zoom(void)
{
		//printf("Zoomed editor outward\n");
	if (get_state()->zoom_factor < 4)
		get_state()->zoom_factor *= 2;
	get_state()->gui->change_zoom(get_state());
	print_mode_info(get_state()->gui);
}

static void			edt_inward_zoom(void)
{
		//printf("Zoomed editor inward\n");
	if (get_state()->zoom_factor > 1)
		get_state()->zoom_factor /= 2;
	get_state()->gui->change_zoom(get_state());
	print_mode_info(get_state()->gui);
}

static void 		redraw_walls_to_backbuffer(uint32_t color)
{
	wipe_editor_back_buffer(0xff000000);
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, color);
	print_mode_info(get_state()->gui);
}
/*
 * Call this function like accelerate_scroll(get_state(), SDL_SCANCODE_LEFT) to scroll left,
 * and call this function like accelerate_scroll(NULL, 0) when no key is pressed to decelerate.
 */
static void			accelerate_scroll(t_state *state, SDL_Scancode direction)
{
	static int acceleration = 0;

	if (acceleration > 0)
		acceleration--;
	if (state == NULL && direction == 0)
	{
		acceleration /= 2;
		return;
	}
	acceleration += 2 * state->zoom_factor;
	if (direction == SDL_SCANCODE_RIGHT)
	{
		state->scroll_x += acceleration;
			//ft_putendl("Increasing Scroll_X!");
	}
	else if (direction == SDL_SCANCODE_LEFT)
	{
		state->scroll_x -= acceleration;
			//ft_putendl("Decreasing Scroll_X!");
	}
	else if (direction == SDL_SCANCODE_DOWN)
	{
		state->scroll_y += acceleration;
			//ft_putendl("Increasing Scroll_Y!");
	}
	else if (direction == SDL_SCANCODE_UP)
	{
		state->scroll_y -= acceleration;
			//ft_putendl("Decreasing Scroll_Y!");
	}
}

// TODO: Systematize and change from constants to dynamic formulas,
//  if having Norminette trouble or flexibility requirements increase

static void			confine_scroll(t_state *state)
{
	static int previous_x;
	static int previous_y;

	if (state->scroll_x == previous_x && state->scroll_y == previous_y)
		return ;
	if (state->scroll_x < 0)
		state->scroll_x = 0;
	if (state->scroll_y < 0)
		state->scroll_y = 0;
	if (state->zoom_factor == 1)
	{
		if (state->scroll_x > EDT_WIN_WIDTH * 3)
			state->scroll_x = EDT_WIN_WIDTH * 3;
		if (state->scroll_y > EDT_WIN_HEIGHT * 3)
			state->scroll_y = EDT_WIN_HEIGHT * 3;
	}
	else if (state->zoom_factor == 2)
	{
		if (state->scroll_x > EDT_WIN_WIDTH * 2)
			state->scroll_x = EDT_WIN_WIDTH * 2;
		if (state->scroll_y > EDT_WIN_HEIGHT * 2)
			state->scroll_y = EDT_WIN_HEIGHT * 2;
	}
	else if (state->zoom_factor == 4)
	{
		state->scroll_x = 0;
		state->scroll_y = 0;
	}
	previous_x = state->scroll_x;
	previous_y = state->scroll_y;
		//printf("Confined Scroll X, Y {%d, %d}\n", state->scroll_x, state->scroll_y);
	redraw_walls_to_backbuffer(0xffffffff);
	draw_scroll_bars_to_backbuffer(state);
}
/*
 * Looks horrible, but it simply handles the 8 possible directions with a waterfall early exit.
 * If both left and right pressed, right dominates. If both up and down pressed, up dominates.
 * If none of right, left, up, down pressed, decceleration happens.
 */
static void 		handle_keyboard_scrolling(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_RIGHT] && doom->keystates[SDL_SCANCODE_UP])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_RIGHT);
		accelerate_scroll(get_state(), SDL_SCANCODE_UP);
	}
	else if (doom->keystates[SDL_SCANCODE_RIGHT] && doom->keystates[SDL_SCANCODE_DOWN])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_RIGHT);
		accelerate_scroll(get_state(), SDL_SCANCODE_DOWN);
	}
	else if (doom->keystates[SDL_SCANCODE_LEFT] && doom->keystates[SDL_SCANCODE_UP])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_LEFT);
		accelerate_scroll(get_state(), SDL_SCANCODE_UP);
	}
	else if (doom->keystates[SDL_SCANCODE_LEFT] && doom->keystates[SDL_SCANCODE_DOWN])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_LEFT);
		accelerate_scroll(get_state(), SDL_SCANCODE_DOWN);
	}
	else if (doom->keystates[SDL_SCANCODE_RIGHT])
		accelerate_scroll(get_state(), SDL_SCANCODE_RIGHT);
	else if (doom->keystates[SDL_SCANCODE_LEFT])
		accelerate_scroll(get_state(), SDL_SCANCODE_LEFT);
	else if (doom->keystates[SDL_SCANCODE_UP])
		accelerate_scroll(get_state(), SDL_SCANCODE_UP);
	else if (doom->keystates[SDL_SCANCODE_DOWN])
		accelerate_scroll(get_state(), SDL_SCANCODE_DOWN);
	else
		accelerate_scroll(NULL, 0);
	confine_scroll(get_state());
}

static void         edt_keystate_input(t_doom *doom) {
	static int lock_w = 0;
	static int lock_z = 0;
	static int lock_x = 0;
	static int lock_s = 0;

	handle_keyboard_scrolling(doom);

	if (lock_w && !doom->keystates[SDL_SCANCODE_W])
		lock_w = 0;
	else if (doom->keystates[SDL_SCANCODE_W] && !lock_w)
	{
		debug_model_walls();
		lock_w = 1;
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

    if (lock_s && !doom->keystates[SDL_SCANCODE_S])
		lock_s = 0;
	else if (doom->keystates[SDL_SCANCODE_S] && !lock_s)
	{
		edt_swap_mode(get_state());
		lock_s = 1;
	}
}

void				edt_render(t_doom *doom)
{
	edt_keystate_input(doom);
    if (editor_front_buffer()->rendering_on)
    {
        flood_buffer(mixing_surface(), 0xff000000);
        SDL_BlitSurface(editor_back_buffer()->buff, NULL, mixing_surface(), NULL);
        SDL_BlitSurface(editor_front_buffer()->buff, NULL, mixing_surface(), NULL);
        flood_buffer(doom->edt->buff, 0xff000000);
        SDL_BlitSurface(mixing_surface(), NULL, doom->edt->buff, NULL);
        editor_front_buffer()->rendering_on = 0;
        editor_back_buffer()->rendering_on = 0;
    }
    else if (editor_back_buffer()->rendering_on)
    {
        flood_buffer(doom->edt->buff, 0xff000000);
        SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
        editor_back_buffer()->rendering_on = 0;
    }
	SDL_UpdateWindowSurface(doom->edt->win);
}