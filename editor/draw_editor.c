#include "doom-nukem.h"

void				redraw_editor_to_backbuffer(uint32_t color)
{
	wipe_editor_back_buffer(0xff000000);
	if (get_model() == NULL)
		return ;
	x_walls_to_buffer(get_model()->wall_count, get_model()->wall_first, editor_back_buffer()->buff, color);
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

	if (mode == mode_polydraw())
		return (polydraw_xpm == NULL ? polydraw_xpm = xpm2surface("img/edt/wall_drawing.xpm") : polydraw_xpm);
	else if (mode == mode_select())
		return (select_xpm == NULL ? select_xpm = xpm2surface("img/edt/select.xpm") : select_xpm);
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

/*
void	print_characters(t_editor *edt)
{
	t_enemy *enemy;
	t_line	line;
	int 	ec;

	if (edt->player_set == 1)
	{
		line.x1 = edt->player.x;
		line.y1 = edt->player.y;
		line.x2 = edt->player.tail.x;
		line.y2 = edt->player.tail.y;
		line.color = 0xffffff00;
		line.buff = edt->buff;
		render_line(&line);
	}
	ec = edt->enemy_count;
	if (ec == 0)
		return ;
	enemy = edt->enemy_first;
	while (ec--)
	{
		line.x1 = enemy->x;
		line.y1 = enemy->y;
		line.x2 = enemy->tail.x;
		line.y2 = enemy->tail.y;
		line.color = 0xff00ff00;
		line.buff = edt->buff;
		render_line(&line);
		enemy = enemy->next;
	}
}

void	print_portals(t_editor *edt)
{
	int		pc;
	t_wall	*portal;
	t_line	line;

	pc = edt->portal_count;
	if (pc == 0)
		return ;
	portal = edt->portal_begin;
	//printf("Portals | portal_count %d\n------------------\n\n", pc);
	while (pc--)
	{
		//printf("Portal id: %d | start: %d, %d | end: %d, %d\n",
		//	   portal->id, portal->start.x, portal->start.y, portal->end.x, portal->end.y);
		line.x1 = portal->start.x;
		line.y1 = portal->start.y;
		line.x2 = portal->end.x;
		line.y2 = portal->end.y;
		line.color = 0xffff0000;
		line.buff = edt->buff;
		render_line(&line);
		portal = portal->next;
	}
}

void	print_walls(t_editor *edt)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = edt->wall_count;
	wall = edt->wall_begin;
	//printf("Walls | wall_count %d\n------------------\n\n", wc);
	while (wc--)
	{
		//printf("Wall id: %d | start: %d, %d | end: %d, %d\n",
		//		wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		line.x1 = wall->start.x;
		line.y1 = wall->start.y;
		line.x2 = wall->end.x;
		line.y2 = wall->end.y;
		line.color = 0xffffffff;
		line.buff = edt->buff;
		render_line(&line);
		wall = wall->next;
	}
	print_characters(edt);
	print_portals(edt);
}

void	circle_rooms(t_doom *doom)
{
	int		rc;
	t_room	*room;

	if (doom->edt->room_count == 0)
		return;
	rc = doom->edt->room_count;
	room = doom->edt->room_first;
	while (rc--)
	{
		room->visual.x = 0;
		room->visual.y = 0;
		find_visual_xy(doom->edt, room);
		if (room->visual.x > 0 && room->visual.y > 0)
			circle_room(doom, room);
		room = room->next;
	}
}

void 	circle_visual(SDL_Surface *buff, t_point *visual, uint32_t color)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = buff->pixels;
	radius = 3;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)visual->x + x + (((int)visual->y + y) * EDT_WIN_WIDTH)] = color;
			x++;
		}
		y++;
		x = -radius;
	}
}

void	circle_room(t_doom *doom, t_room *room)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 3;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)room->visual.x + x + (((int)room->visual.y + y) * EDT_WIN_WIDTH)] = 0xffffffff;
			x++;
		}
		y++;
		x = -radius;
	}
}

void	circle_player(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)doom->edt->player.x + x + (((int)doom->edt->player.y + y) * EDT_WIN_WIDTH)] = 0xffffff00;
			x++;
		}
		y++;
		x = -radius;
	}
}

void	circle_enemy(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)doom->edt->last_enemy.x + x + (((int)doom->edt->last_enemy.y + y) * EDT_WIN_WIDTH)] = 0xff00ff00;
			x++;
		}
		y++;
		x = -radius;
	}
}
*/
