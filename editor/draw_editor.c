/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_editor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:16:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 17:29:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void					redraw_editor_to_backbuffer(uint32_t color)
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

// TODO SWAP TO ALLOWED FUNCTIONS FROM SDL_BLIT AND SDL_RECT AND NORMINETTIZE
void					print_mode_info(t_gui *mode)
{
	SDL_Surface			*mode_surface;
	SDL_Surface			*zoom_surface;
	SDL_Surface			*mousehelp_surface;
	SDL_Surface			*keyhelp_surface;
	SDL_Rect			place;

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
