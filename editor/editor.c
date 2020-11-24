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

// TODO:
//  LET USER KNOW WHICH MODE IS ACTIVE
//  LET USER ZOOM AND SCROLL FREELY IN THE EDITOR

// TODO NEXT FEATURE:
//  MODE TO ADD ENEMY AND PLAYER OBJECTS
//  ENABLE SAVING TO MAPFILE AGAIN
//  MODE TO PORTALIZE A WALL AND EXTEND A NEW ROOM FROM IT

/* trigger_protection() is a notifier function called by set_pixel_safe() when
 * overwriting to a buffer over a set_protected_color() has occured. Triggers
 * are accumulated by the calls 'trigger_protection(0)'. Decoupled from this,
 * elsewhere in the code a has_protection = 1 enabled eg. mode_polydraw()
 * can implement a periodic or event based check wheter overwriting happened
 * by invoking 'trigger_protection(1)'. If yes, this protection ATM (stc)
 * calls a clean slate for editor back buffer by a wipe and a walls redraw  */

// TODO: Utilize trigger_protection to detect illegal wall draws or room draws
//  (creation of intersection (overlapping) walls should not be possible

// MOVE THIS SOMEWHERE COMMONPLACE
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

// MOVE THIS SOMEWHERE MODE_* PLACE
static t_gui        *mode_polydraw()
{
	static t_gui	*polydraw = NULL;

	if (!polydraw)
	{
		polydraw = (t_gui*)malloc(sizeof(t_gui));
		if (!polydraw)
			ft_die("Fatal error: Could not malloc polydraw struct at mode_polydraw.");
        polydraw->activate = polydraw_activate;
        polydraw->change_zoom = polydraw_change_zoom;
		polydraw->left_click = polydraw_left_click;
		polydraw->right_click = polydraw_right_click;
		polydraw->middle_click = polydraw_middle_click;
		polydraw->has_motion = 1;
		polydraw->motion = polydraw_mouse_motion;
		set_protected_color(0xffffffff);
	}
	return (polydraw);
}

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

	if (mode == mode_polydraw())
		return (polydraw_xpm == NULL ? polydraw_xpm = xpm2surface("img/edt/wall_drawing.xpm") : polydraw_xpm);
	ft_die("Fatal error: Could not return mode_xpm.");
	return (NULL);
}

void                print_mode_info(t_gui *mode)
{
	SDL_Surface *mode_surface;
    SDL_Surface *zoom_surface;
    SDL_Rect 	place;

	if (mode == mode_polydraw())
        mode_surface = mode_xpm(mode);
    zoom_surface = zoom_xpm(get_state()->zoom_factor);
    if (!mode_surface || !zoom_surface)
    	ft_die("Fatal error: print_mode_info failed to retrieve mode/zoom surfaces.");
    place.w = mode_surface->w;
    place.h = mode_surface->h;
	place.x = zoom_surface->w + 10;
	place.y = EDT_WIN_HEIGHT - (place.h + 2);
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
		state->gui = mode_polydraw();
		state->gui->activate(state);
		state->zoom_factor = 1;
		print_mode_info(state->gui);
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

// TODO MOVE THESE THREE TO EDT_INPUT
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

static void         edt_keystate_input(t_doom *doom) {
	static int lock_w = 0;
	static int lock_p = 0;
	static int lock_z = 0;
	static int lock_x = 0;

	if (lock_p && !doom->keystates[SDL_SCANCODE_P])
		lock_p = 0;
	else if (doom->keystates[SDL_SCANCODE_P] && !lock_p)
	{
		wipe_editor_back_buffer(0xff000000);
		x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, 0xffffffff);
		print_mode_info(get_state()->gui);
		lock_p = 1;
	}

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