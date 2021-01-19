#include "doom-nukem.h"

void				redraw_editor_to_backbuffer(uint32_t color)
{
	wipe_editor_back_buffer(0xff000000);
	if (get_model() == NULL)
		return ;
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, color);
	x_walls_to_buffer(get_model()->portal_count, get_model()->portal_first, editor_back_buffer()->buff, COLOR_PORTAL);
	print_mode_info(get_state()->gui);
	draw_scroll_bars_to_backbuffer(get_state());
	draw_plantings_to_backbuffer(get_model(), get_state());
	draw_selection_to_backbuffer(get_state());
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
	static SDL_Surface *select_xpm = NULL;
	static SDL_Surface *planting_xpm = NULL;
	static SDL_Surface *pickups_xpm = NULL;
	static SDL_Surface *effect_xpm = NULL;

	if (mode == mode_polydraw())
		return (polydraw_xpm == NULL ? polydraw_xpm = xpm2surface("img/edt/wall_drawing.xpm") : polydraw_xpm);
	else if (mode == mode_select())
		return (select_xpm == NULL ? select_xpm = xpm2surface("img/edt/select.xpm") : select_xpm);
	else if (mode == mode_effect())
		return (effect_xpm == NULL ? effect_xpm = xpm2surface("img/edt/effect.xpm") : effect_xpm);
	else if (mode == mode_planting())
		return (planting_xpm == NULL ? planting_xpm = xpm2surface("img/edt/planting.xpm") : planting_xpm);
	else if (mode == mode_pickups())
		return (pickups_xpm == NULL ? pickups_xpm = xpm2surface("img/edt/pickups.xpm") : pickups_xpm);
	ft_die("Fatal error: Could not return mode_xpm.");
	return (NULL);
}

SDL_Surface 		*mousehelp_xpm(void)
{
	static SDL_Surface *mousehelp_xpms = NULL;

	if (mousehelp_xpms == NULL)
		mousehelp_xpms = xpm2surface("img/edt/mousehelp.xpm");
	if (!mousehelp_xpms)
		ft_die("Fatal error: Could not return mousehelp_xpm.");
	return (mousehelp_xpms);
}

SDL_Surface 		*keyhelp_xpm(void)
{
	static SDL_Surface *keyhelp_xpms = NULL;

	if (keyhelp_xpms == NULL)
		keyhelp_xpms = xpm2surface("img/edt/keyhelp.xpm");
	if (!keyhelp_xpms)
		ft_die("Fatal error: Could not return keyhelp_xpm.");
	return (keyhelp_xpms);
}

// TODO ALTERNATIVE TO SDL_BLIT (POSSIBLE YAGNI?)
void                print_mode_info(t_gui *mode)
{
	SDL_Surface *mode_surface;
	SDL_Surface *zoom_surface;
	SDL_Surface *mousehelp_surface;
	SDL_Surface *keyhelp_surface;
	SDL_Rect 	place;

	mode_surface = mode_xpm(mode);
	zoom_surface = zoom_xpm(get_state()->zoom_factor);
	mousehelp_surface = mousehelp_xpm();
	keyhelp_surface = keyhelp_xpm();
	if (!mode_surface || !zoom_surface || !mousehelp_surface || !keyhelp_surface)
		ft_die("Fatal error: print_mode_info failed to retrieve mode/zoom/mouse/keyhelp surfaces.");
	place.w = keyhelp_surface->w;
	place.h = keyhelp_surface->h;
	place.x = zoom_surface->w + mode_surface->w + mousehelp_surface->w + 20;
	place.y = EDT_WIN_HEIGHT - (place.h + 2) - 10;
	SDL_BlitSurface(keyhelp_surface, NULL, editor_back_buffer()->buff, &place);
	place.x -= keyhelp_surface->w + 5;
	SDL_BlitSurface(mousehelp_surface, NULL, editor_back_buffer()->buff, &place);
	place.w = mode_surface->w;
	place.x = zoom_surface->w + 10;
	SDL_BlitSurface(mode_surface, NULL, editor_back_buffer()->buff, &place);
	place.w = zoom_surface->w;
	place.h = zoom_surface->h;
	place.x -= zoom_surface->w + 5;
	SDL_BlitSurface(zoom_surface, NULL, editor_back_buffer()->buff, &place);

	editor_back_buffer()->rendering_on = 1;
}
