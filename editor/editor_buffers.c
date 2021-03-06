#include "doom-nukem.h"

void 			wipe_editor_back_buffer(uint32_t color)
{
	flood_buffer(editor_back_buffer()->buff, color);
	editor_back_buffer()->rendering_on = 1;
}

void 			wipe_editor_front_buffer(uint32_t color)
{
	flood_buffer(editor_front_buffer()->buff, color);
	editor_front_buffer()->rendering_on = 0;
	editor_back_buffer()->rendering_on = 1;
}

t_2d_layer     *editor_back_buffer(void)
{
	static t_2d_layer *layer = NULL;
	if (!layer)
	{
		layer = (t_2d_layer*)malloc(sizeof(t_2d_layer));
		if (!layer)
			ft_die("Fatal error: Could not malloc layer at editor_front_buffer.");
		layer->buff = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT,
                    32,SDL_GetWindowPixelFormat(doom_ptr()->edt->win));
		if (!layer->buff)
			ft_die("Fatal error: Could not malloc SDL_Surface at editor_front_buffer.");
		layer->rendering_on = 1;
		flood_buffer(layer->buff, 0xff000000);
	}
	return (layer);
}

t_2d_layer     *editor_front_buffer(void)
{
	static t_2d_layer *layer = NULL;
	if (!layer)
	{
		layer = (t_2d_layer*)malloc(sizeof(t_2d_layer));
		if (!layer)
			ft_die("Fatal error: Could not malloc layer at editor_front_buffer.");
		layer->buff = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT,
													 32,SDL_GetWindowPixelFormat
															 (doom_ptr()->edt->win));
		if (!layer->buff)
			ft_die("Fatal error: Could not malloc SDL_Surface at editor_front_buffer.");
		SDL_SetColorKey(layer->buff, SDL_TRUE, 0xff000000);
		layer->rendering_on = 0;
		flood_buffer(layer->buff, 0xff000000);
	}
	return (layer);
}

SDL_Surface *mixing_surface()
{
	static SDL_Surface *buff = NULL;
	if (!buff)
	{
		buff = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT,
											  32,SDL_GetWindowPixelFormat
													  (doom_ptr()->edt->win));
		if (!buff)
			ft_die("Fatal error: Could not malloc SDL_Surface at mixing_surface.");
		flood_buffer(buff, 0xff000000);
	}
	return (buff);
}