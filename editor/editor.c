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

t_2d_layer     *editor_back_buffer(void)
{
    static t_2d_layer *layer = NULL;
    if (!layer)
    {
        layer = (t_2d_layer*)malloc(sizeof(t_2d_layer));
        if (!layer)
            ft_die("Fatal error: Could not malloc layer at editor_front_buffer.");
        layer->buff = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT,
            32,SDL_GetWindowPixelFormat
            (singleton_doom_pointer(NULL)->edt->win));
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
            (singleton_doom_pointer(NULL)->edt->win));
        if (!layer->buff)
            ft_die("Fatal error: Could not malloc SDL_Surface at editor_front_buffer.");
        SDL_SetColorKey(layer->buff, SDL_TRUE, 0xff000000);
        layer->rendering_on = 0;
        flood_buffer(layer->buff, 0xff000000);
    }
    return (layer);
}

static void linedraw_to_buffer(t_linedraw *data, SDL_Surface *buff, uint32_t color)
{
    t_line          line;

    line.doom = singleton_doom_pointer(NULL);
    line.buff = buff;
    line.color = color;
    line.x1 = data->draw_from_x;
    line.y1 = data->draw_from_y;
    line.x2 = data->draw_to_x;
    line.y2 = data->draw_to_y;
    render_line(&line);
}

static void linedraw_to_wall(t_linedraw *data)
{
    t_wall *wall;

    wall = get_model()->walls;
    wall->id = get_model()->wall_count;
    wall->start.x = data->draw_from_x;
    wall->start.y = data->draw_from_y;
    wall->end.x = data->draw_to_x;
    wall->end.y = data->draw_to_y;
    get_model()->wall_count++;
    wall = (t_wall*)malloc(sizeof(t_wall));
    if (!wall)
        ft_die("Fatal error: Could not malloc t_wall at linedraw_to_wall.");
    if (get_model()->wall_count == 1)
        get_model()->wall_first = get_model()->walls;
    get_model()->walls->next = wall;
    get_model()->walls = wall;
    ft_putendl("Linedraw_to_wall: Added a wall to model->walls");
}

void 		polydraw_start(t_status *status)
{
    t_linedraw      *data;

    data = (t_linedraw*)status->data;
    assert(status->phase == 0);
    data->origin_id = get_model()->wall_count;
    data->drawing_underway = 1;
    data->draw_from_x = status->click_x;
    data->draw_from_y = status->click_y;
	status->phase = 1;
	ft_putendl("Polydraw start");
}

void 		polydraw_continue(t_status *status)
{
    t_linedraw      *data;

    data = (t_linedraw*)status->data;
	assert(status->phase == 1);
	assert(data->drawing_underway == 1);
	data->draw_to_x = status->click_x;
	data->draw_to_y = status->click_y;
    linedraw_to_wall(data);
    linedraw_to_buffer(data, editor_back_buffer()->buff, 0xffffffff);
    editor_back_buffer()->rendering_on = 1;
    *data = (t_linedraw){data->origin_id, 1, status->click_x, status->click_y, 0};
	//status->phase = 2;
	ft_putendl("Polydraw continue");
}

void 		polydraw_end(t_status *status)
{
    t_linedraw      *data;

    data = (t_linedraw*)status->data;
	assert(status->phase == 2);
	status->phase = 0;
	*data = (t_linedraw){0};
	// Requires expansion so that line is drawn to original location.
	ft_putendl("Polydraw end");
}

void 		polydraw_reset(t_status *status) {
    t_linedraw *data;
    t_wall *wall;
    int wc;

    // TODO
    //  SHOULD DESTROY THUS FAR CREATED WALLS IN THE ABORTED SEQUENCE
    //  ITERATE MDL->WALLS TO DATA->ORIGIN_ID
    //  ITERATE FROM THIS POINT UNTIL END OF MDL->WALLS
    //  REDRAW EACH WALL IN BLACK TO EDITOR BACK BUFFER
    data = (t_linedraw *) status->data;
    wc = data->origin_id;
    wall = get_model()->wall_first;
    while (wc--)
        wall = wall->next;
    wc = get_model()->wall_count - data->origin_id;
    while (wc--)
    {
        //wall_to_buffer(wall, editor_back_buffer()->buff, 0xff000000);
        // TODO: GENERATE ABOVE FUNCTION!
        printf("wall_id %d wiped\n", wall->id);
        wall = wall->next;
    }
	status->phase = 0;
    *data = (t_linedraw){0};
    flood_buffer(editor_front_buffer()->buff, 0xff000000);
    editor_front_buffer()->rendering_on = 0;
    editor_back_buffer()->rendering_on = 1;
	ft_putendl("Polydraw reset");
}

t_status	*polydraw_status()
{
	static t_status *status = NULL;
	if (!status)
	{
		status = (t_status*)malloc(sizeof(t_status));
		if (!status)
			ft_die("Fatal error: Could not malloc status for polydraw at polydraw_status");
		status->data = (t_linedraw*)malloc(sizeof(t_linedraw));
		if (!status->data)
            ft_die("Fatal error: Could not malloc data for polydraw at polydraw_status");
		status->phase = 0;
		status->phase_count = 3;
		status->phases = (gui_action*)malloc(sizeof(gui_action) * status->phase_count);
		status->phases[0] = polydraw_start;
		status->phases[1] = polydraw_continue;
		status->phases[2] = polydraw_end;
		status->reset = polydraw_reset;
		// TODO QUICK ITEMS:
		//  DISABLE WIPING OF DATA, INSTEAD SELECTIVELY OVERWRITE FIELDS
		//  TRACK HOW MANY WALLS TO DESTROY ON RESET
		//  TRACK WHICH WALL IS THE ORIGINAL WALL OF THE POLYDRAW SEQUENCE
		//  POLYDRAW_RESET WILL DESTROY CREATED WALLS

		// TODO LARGE ITEMS:
		//  WHEN WITHIN THRESHOLD PROXIMITY OF ORIGIN WALL, DO:
		//      MAGNETIZATION AND SEQUENCE ENDING LINKED TO THE PROXIMITY
		//      GUI HOVERING CIRCLE HIGHLIGHTING DRAWING TO FRONT BUFFER
    }
	return (status);
}

void 		polydraw_mouse_motion(int x, int y)
{
    t_linedraw      *data;

    // Check if polydraw_status->phase is not polydraw_continue() to early exit!
	if (polydraw_status()->phase != 1)
	    return ;
	// When the phase is polydraw_continue(), draw a line at the front buffer
	data = polydraw_status()->data;
	assert(data->drawing_underway);
	assert(data->draw_from_x >= 0 && data->draw_from_x < EDT_WIN_WIDTH &&
	        data->draw_from_y >= 0 && data->draw_from_y < EDT_WIN_HEIGHT);
	data->draw_to_x = x;
	data->draw_to_y = y;
	flood_buffer(editor_front_buffer()->buff, 0xff000000);
    linedraw_to_buffer(data, editor_front_buffer()->buff, 0xffffffff);
	editor_front_buffer()->rendering_on = 1;
}

void 		polydraw_left_click(int x, int y)
{
	//ft_putendl("Polydraw left_clicked");
	t_status		*status;

	status = polydraw_status();
	status->click_x = x;
	status->click_y = y;
	status->phases[status->phase](status);
}

void 		polydraw_right_click(int x, int y)
{
	//ft_putendl("Polydraw right_clicked");
	t_status		*status;

	status = polydraw_status();
    // Check if polydraw_status->phase is not polydraw_continue() to early exit!
    if (status->phase != 1)
        return ;
    // When the phase is polydraw_continue(), proceed a cycle of polydraw_continue() followed
    // by the polydraw_end(). Invoke them in order with the "reflection", to maintain
    // coherence and code style and logic unity with polydraw_left_click function.
	status->click_x = x;
	status->click_y = y;
	// This invokes polydraw_continue()
    status->phases[status->phase](status);
    status->phase++;
    assert(status->phase == 2);
    // This invokes polydraw_end()
    status->phases[status->phase](status);
}

void 		polydraw_middle_click(int x, int y)
{
    //ft_putendl("Polydraw middle_clicked");
    // Have mapped polydraw_status->reset (polydraw_reset) to polydraw_middle_click here.
    polydraw_status()->reset(polydraw_status());
}

t_gui		*mode_polydraw()
{
	static t_gui *polydraw = NULL;
	if (!polydraw)
	{
		polydraw = (t_gui*)malloc(sizeof(t_gui));
		if (!polydraw)
			ft_die("Fatal error: Could not malloc polydraw struct at mode_polydraw.");
		polydraw->left_click = polydraw_left_click;
		polydraw->right_click = polydraw_right_click;
		polydraw->middle_click = polydraw_middle_click;
		polydraw->has_motion = 1;
		polydraw->motion = polydraw_mouse_motion;
	}
	return (polydraw);
}

/* get_state returns the overall "State" of the editor. In the field state->gui,
 * you can assign a GUI mode, that are retrieved from their Singleton functions.
 * For example, for the get_state() initial state, state->gui = mode_polydraw();
 * assigns the GUI's action state to polydraw mode, which is a mode that draws
 * polygons using mouse clicks. By following the mode_polydraw() trail, you can
 * explore and find out how the polydraw_* functions are organized and coded. */

t_state		*get_state(void)
{
	static t_state *state = NULL;
	if (!state)
	{
		state = (t_state*)malloc(sizeof(t_state));
		if (!state)
			ft_die("Fatal error: Could not malloc state struct at get_state");
		state->gui = mode_polydraw();
	}
	return (state);
}

void 		init_edt(t_doom *doom)
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
}

void 		destroy_edt(t_doom *doom)
{
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

void 		edt_mouse_motion(t_doom *doom)
{
	t_state	*state;

	state = get_state();
	if (state->gui->has_motion)
		state->gui->motion(doom->event.motion.x, doom->event.motion.y);
}

void 		edt_mouse_down(t_doom *doom)
{
	t_state	*state;

	state = get_state();
	if (doom->event.button.button == SDL_BUTTON_LEFT)
		state->gui->left_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_MIDDLE)
		state->gui->middle_click(doom->event.button.x, doom->event.button.y);
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
		state->gui->right_click(doom->event.button.x, doom->event.button.y);
}

static SDL_Surface *mixing_surface()
{
    static SDL_Surface *buff = NULL;
    if (!buff)
    {
        buff = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT,
                                       32,SDL_GetWindowPixelFormat
                                               (singleton_doom_pointer(NULL)->edt->win));
        if (!buff)
            ft_die("Fatal error: Could not malloc SDL_Surface at mixing_surface.");
        flood_buffer(buff, 0xff000000);
    }
    return (buff);
}

void		edt_render(t_doom *doom)
{
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

#if 0
void 		init_edt(t_doom *doom, int argc, char **argv)
{
	int opened;

	doom->edt = (t_editor*)malloc(sizeof(t_editor));
	if (!doom->edt)
		ft_die("Fatal error: Mallocing level editor struct failed at init_edt.");
	doom->edt->parent = doom;
	doom->edt->win = SDL_CreateWindow("DoomNukem Level Editor", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 0);
	doom->edt->buff = SDL_GetWindowSurface(doom->edt->win);
	flood_buffer(doom->edt->buff, 0xff000000);

	doom->edt->walls = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->walls)
		ft_die("Fatal error: Mallocing walls struct failed at init_edt.");
	doom->edt->rooms = (t_room*)malloc(sizeof(t_room));
	if (!doom->edt->rooms)
		ft_die("Fatal error: Mallocing rooms struct failed at init_edt.");
	doom->edt->portals = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->portals)
		ft_die("Fatal error: Mallocing portals struct failed at init_edt.");
	doom->edt->enemies = (t_enemy*)malloc(sizeof(t_enemy));
	if (!doom->edt->enemies)
		ft_die("Fatal error: Mallocing enemies struct failed at init_edt.");
	doom->edt->wall_begin = NULL;
	doom->edt->portal_begin = NULL;
	doom->edt->enemy_first = NULL;
	doom->edt->room_first = NULL;
	doom->edt->wall_string = NULL;
	doom->edt->room_string = NULL;
	doom->edt->portal_string = NULL;
	doom->edt->join_string = NULL;
	doom->edt->player_string = NULL;
	doom->edt->enemy_string = NULL;
	doom->edt->map_path = NULL;
	doom->edt->portalization_a = NULL;
	doom->edt->portalization_b = NULL;
	doom->edt->new_portal = NULL;
	doom->edt->subselection_wall = NULL;
	doom->edt->hover_status = 0;
	doom->edt->hover_id = -1;
	doom->edt->selection_status = 0;
	doom->edt->selection_room_id = -1;
	doom->edt->subselection_id = -1;
	doom->edt->wall_count = 0;
	doom->edt->room_count = 0;
	doom->edt->portal_count = 0;
	doom->edt->enemy_count = 0;
	doom->edt->player_set = 0;
	doom->edt->enemy_set = 0;
	doom->edt->polygon_binding = 0;
	doom->edt->portalization_binding = 0;
	doom->edt->portalization_ending = 0;
	doom->edt->write_maps = 0;
	doom->edt->player.x = -1;
	doom->edt->player.y = -1;
	doom->edt->player.rot = -1;
	doom->edt->is_wall_start = 1;
	doom->edt->polygon_start_x = -1;
	doom->edt->polygon_start_y = -1;
	doom->edt->portal_x = -1;
	doom->edt->portal_y = -1;
	doom->edt->new_portal_x = -1;
	doom->edt->new_portal_y = -1;
	doom->edt->load_map = 0;
	doom->edt->write_maps = 0;
	doom->edt->overwrite_map = 0;
	create_room_polygon_map(doom->edt);
	create_grid_buffer(doom->edt);
	if (argc == 2)
	{
		opened = open(argv[1], O_RDONLY);
		if (opened > 1)
		{
			ft_putendl("Hive-DoomNukem: Loading map enabled. Map file detected.");
			doom->edt->load_map = 1;
			doom->edt->overwrite_map = 1;
			close(opened);
		}
		else
		{
			ft_putendl("Hive-DoomNukem: Saving maps enabled. Map file will be written, when closing Level Editor.");
			doom->edt->write_maps = 1;
		}
		doom->edt->map_path = argv[1];
	}
}

void 		destroy_edt(t_doom *doom)
{
	// INCOMPLETE!!! MUST DESTRYOY LINKED LIST STRUCTURES FROM BEGINNING TO END
	// - krusthol
	if (doom->edt->write_maps && doom->edt->wall_count > 0)
	{
		create_strings_from_state(doom->edt);
		write_mapfile(doom->edt);
	}
	if (doom->edt->overwrite_map)
		overwrite_mapfile(doom->edt);
	SDL_FreeSurface(doom->edt->buff);
	SDL_FreeSurface(doom->edt->front_buffer);
	SDL_FreeSurface(doom->edt->back_buffer);
	SDL_DestroyWindow(doom->edt->win);
	//ft_putendl("Window SDL destructed");
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	doom->edt->front_buffer = NULL;
	doom->edt->back_buffer = NULL;
	if (!doom->mdl)
	{
		free(doom->edt->walls);
		doom->edt->walls = NULL;
		doom->edt->wall_begin = NULL;
		doom->edt->subselection_wall = NULL;
		free(doom->edt->rooms);
		doom->edt->rooms = NULL;
		doom->edt->room_first = NULL;
		free(doom->edt->portals);
		doom->edt->portals = NULL;
		doom->edt->portal_begin = NULL;
		free(doom->edt->enemies);
		doom->edt->enemies = NULL;
		doom->edt->enemy_first = NULL;
	}
	if (doom->edt->wall_string)
		free(doom->edt->wall_string);
	if (doom->edt->room_string)
		free(doom->edt->room_string);
	if (doom->edt->portal_string)
		free(doom->edt->portal_string);
	if (doom->edt->enemy_string)
		free(doom->edt->enemy_string);
	if (doom->edt->player_string)
		free(doom->edt->player_string);
	if (doom->edt->join_string)
		free(doom->edt->join_string);
	doom->edt->wall_string = NULL;
	doom->edt->room_string = NULL;
	doom->edt->portal_string = NULL;
	doom->edt->enemy_string = NULL;
	doom->edt->player_string = NULL;
	doom->edt->join_string = NULL;
	doom->edt->portalization_a = NULL;
	doom->edt->portalization_b = NULL;
	doom->edt->new_portal = NULL;
	doom->edt->map_path = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

static void draw_portal_in_red_to_buffer(t_editor *edt, SDL_Surface *buff, t_wall *portal)
{
    t_line  line;

    line.buff = buff;
    line.color = 0xffff0000;
    line.doom = edt->parent;
    line.x1 = portal->start.x;
    line.y1 = portal->start.y;
    line.x2 = portal->end.x;
    line.y2 = portal->end.y;
    render_line(&line);
}

static void redraw_portals_in_red(t_editor *edt)
{
    int pc;
    t_wall *portal;

    pc = edt->portal_count;
    portal = edt->portal_begin;
    while (pc--)
    {
        draw_portal_in_red_to_buffer(edt, edt->buff, portal);
        portal = portal->next;
    }
    //ft_putendl("Redrew all portals in red.");
}

void        create_grid_buffer(t_editor *edt)
{
	int x;
	int y;

	edt->grid_buffer = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 32, SDL_GetWindowPixelFormat(edt->win));
	flood_buffer(edt->grid_buffer, 0x00000000);
	edt->grid_temp = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 32, SDL_GetWindowPixelFormat(edt->win));
	flood_buffer(edt->grid_temp, 0xff000000);
	SDL_SetColorKey(edt->grid_temp, SDL_TRUE, 0xff000000);
	if (edt->grid_buffer == NULL || edt->grid_temp == NULL)
		ft_die("Fatal error: SDL_CreateRGBSurface() failed at create_grid_buffer for grid_buffer or grid_temp.");
	x = 0;
	y = 0;
	while (y < EDT_WIN_HEIGHT)
	{
		while (x < EDT_WIN_WIDTH)
		{
			if (x % 50 == 0 || y % 50 == 0)
				set_pixel(edt->grid_buffer, x, y, 0xff222222);
			x++;
		}
		x = 0;
		y++;
	}
	ft_putendl("Created grid buffer!");
}

static void hover_highlight_room_to_buffer(int room_id, t_editor *edt)
{
	int count;
	t_line highlight;
	t_wall *wall;
	t_room *room;

	count = edt->room_count;
	room = edt->room_first;
	while (count-- && room->id != room_id)
		room = room->next;
	count = room->wall_count;
	wall = room->first_wall;
	// Create separate front_buffer?
	flood_buffer(edt->front_buffer, 0x00000000);
	flood_buffer(edt->back_buffer, 0xff000000);
	highlight.doom = edt->parent;
	highlight.buff = edt->front_buffer;
	while (count--)
	{
		highlight.x1 = wall->start.x;
		highlight.y1 = wall->start.y;
		highlight.x2 = wall->end.x;
		highlight.y2 = wall->end.y;
		highlight.color = 0xff00ff00;
		render_line(&highlight);
		wall = wall->next;
	}
	SDL_BlitSurface(edt->buff, NULL, edt->back_buffer, NULL);
	SDL_BlitSurface(edt->front_buffer, NULL, edt->buff, NULL);
	//SDL_UpdateWindowSurface(edt->win);
	//ft_putendl("Hover blitted.");
}

static void dehover(t_editor *edt)
{
	SDL_BlitSurface(edt->back_buffer, NULL, edt->buff, NULL);
	flood_buffer(edt->back_buffer, 0xff000000);
	flood_buffer(edt->front_buffer, 0x00000000);
	//SDL_UpdateWindowSurface(edt->win);
	//ft_putendl("Dehover blitted.");
}

static void check_hovering_over_room(SDL_Event *event, t_editor *edt)
{
	static int	last_hovered_room = -1;
	int 		queried_room;

	queried_room = room_id_from_pixel(edt->poly_map, event->motion.x, event->motion.y);
	if (queried_room != last_hovered_room)
	{
		if (queried_room != -1)
		{
			//printf("Hovering over room_id %d with the mouse\n", queried_room);
			if (edt->selection_room_id != queried_room)
			{
				edt->hover_status = 1;
				hover_highlight_room_to_buffer(queried_room, edt);
			}
		}
		else
		{
			if (edt->hover_status == 1)
			{
				edt->hover_status = 0;
				dehover(edt);
			}
		}
		last_hovered_room = queried_room;
		edt->hover_id = queried_room;
	}
}

void 		edt_mouse_motion(t_doom *doom)
{
	doom->edt->polygon_binding = 0;
	if (doom->edt->polygon_start_x > 0 && doom->edt->polygon_start_y > 0)
	{
		if (abs(doom->event.motion.x - doom->edt->polygon_start_x) < 16)
		{
			if (abs(doom->event.motion.y - doom->edt->polygon_start_y) < 16)
				doom->edt->polygon_binding = 1;
		}
	}
	check_hovering_over_room(&doom->event, doom->edt);
}

static void set_portalization_xy(t_editor *edt)
{
	if (edt->portalization_binding)
	{
		if (edt->portalization_a->end.x == edt->portalization_b->start.x &&
			edt->portalization_a->end.y == edt->portalization_b->start.y)
		{
			edt->portal_x = edt->portalization_a->end.x;
			edt->portal_y = edt->portalization_a->end.y;
			//ft_putendl("Set Portalization(TM).");
		}
		else
		{
			ft_putendl("Warning: Portalization A / B comparison error.");
			edt->portal_x = -1;
			edt->portal_y = -1;
			edt->portalization_binding = 0;
		}
	}
	else
	{
		edt->portal_x = -1;
		edt->portal_y = -1;
	}
}

static void test_begin_portalization(int x, int y, t_editor *edt)
{
	int 	wc;
	t_wall	*wall;

	wc = edt->wall_count;
	wall = edt->wall_begin;
	edt->portalization_binding = 0;
	while (wc--)
	{
		if ((abs(x - wall->start.x) < 16) && (abs(y - wall->start.y) < 16))
		{
			edt->portalization_binding = 1;
			edt->portalization_b = wall;
			//ft_putendl("Portalization_binding due to start.x, start.y.");
		}
		else if ((abs(x - wall->end.x) < 16) && (abs(y - wall->end.y) < 16))
		{
			edt->portalization_binding = 1;
			edt->portalization_a = wall;
			//ft_putendl("Portalization_binding due to end.x, end.y.");
		}
		wall = wall->next;
	}
	set_portalization_xy(edt);
}

static void test_end_portalization(int x, int y, t_editor *edt)
{
	edt->new_portal = NULL;
	if ((abs(x - edt->portalization_a->start.x) < 16) && (abs(y - edt->portalization_a->start.y) < 16))
	{
		edt->new_portal = edt->portalization_a;
		edt->new_portal_x = edt->portalization_a->start.x;
		edt->new_portal_y = edt->portalization_a->start.y;
	}
	else if ((abs(x - edt->portalization_b->end.x) < 16) && (abs(y - edt->portalization_b->end.y) < 16))
	{
		edt->new_portal = edt->portalization_b;
		edt->new_portal_x = edt->portalization_b->end.x;
		edt->new_portal_y = edt->portalization_b->end.y;
	}
	//ft_putendl("Tested end_portalization");
}

static void expand_polygon(int x, int y, t_editor *edt)
{
	t_wall *next_wall;

	if (edt->is_wall_start)
	{
		//ft_putstr("Testing begin Portalization(TM): ");
		test_begin_portalization(x, y, edt);
		edt->walls->id = edt->wall_count;
		if (edt->portalization_binding)
		{
			edt->walls->start.x = edt->portal_x;
			edt->walls->start.y = edt->portal_y;
			//ft_putendl("Portalization started.");
		}
		else
		{
			//ft_putendl("Negative.");
			edt->walls->start.x = x;
			edt->walls->start.y = y;
		}
		edt->polygon_start_x = x;
		edt->polygon_start_y = y;
		edt->is_wall_start = 0;
	}
	else if (edt->polygon_binding)
	{
		//ft_putendl("Polygon binding.");
		edt->walls->end.x = edt->polygon_start_x;
		edt->walls->end.y = edt->polygon_start_y;
		// DEPRECEATED. REPLACED WITH CENTRAL CREATE_STRINGS_FROM_STATE
		//expand_wall_string(edt);
		edt->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (edt->wall_count == 1)
			edt->wall_begin = edt->walls;
		edt->walls->next = next_wall;
		edt->walls = next_wall;
		edt->is_wall_start = 1;
		record_room(edt);
		print_walls(edt);
	}
	else
	{
		edt->walls->end.x = x;
		edt->walls->end.y = y;
		if (edt->portalization_ending)
		{
			//ft_putstr("Testing end Portalization(TM): ");
			test_end_portalization(x, y, edt);
			if (edt->new_portal)
			{
				edt->walls->end.x = edt->new_portal_x;
				edt->walls->end.y = edt->new_portal_y;
				record_portal(edt);
				edt->portalization_ending = 0;
				edt->portalization_a = NULL;
				edt->portalization_b = NULL;
				edt->is_wall_start = 1;
				//ft_putendl("Portal created.");
			}
			else
			{
				//ft_putendl("Negative.");
			}
		}
		// DEPRECEATED. REPLACED WITH CENTRAL CREATE_STRINGS_FROM_STATE
		// expand_wall_string(edt);
		edt->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (edt->wall_count == 1)
			edt->wall_begin = edt->walls;
		edt->walls->next = next_wall;
		edt->walls = next_wall;
		edt->walls->id = edt->wall_count;
		edt->walls->start.x = x;
		edt->walls->start.y = y;
		// PORTALIZATION SUCCEEDED, THEN WE ADD ONE MORE WALL TO COMPLETE THE ROOM POLYGONALLY
		if (edt->is_wall_start)
		{
			edt->walls->start.x = edt->new_portal->start.x;
			edt->walls->start.y = edt->new_portal->start.y;
			edt->walls->end.x = edt->new_portal->end.x;
			edt->walls->end.y = edt->new_portal->end.y;
			// DEPRECEATED. REPLACED WITH CENTRAL CREATE_STRINGS_FROM_STATE
			//expand_wall_string(edt);
			edt->wall_count++;
			next_wall = (t_wall*)malloc(sizeof(t_wall));
			if (!next_wall)
				ft_die("Fatal error: Could not malloc t_wall at record_wall.");
			edt->walls->next = next_wall;
			edt->walls = next_wall;
			edt->walls->id = edt->wall_count;
			record_room(edt);
		}
		print_walls(edt);
	}
}

static void dehighlight_room_selection(t_editor *edt)
{
	int count;
	t_line line;
	t_wall *wall;
	t_room *room;

	line.doom = edt->parent;
	line.buff = edt->buff;
	line.color = 0xffffffff;
	count = edt->room_count;
	room = edt->room_first;
	while (count-- && room->id != edt->selection_room_id)
		room = room->next;
	count = room->wall_count;
	wall = room->first_wall;
	while (count--)
	{
		line.x1 = wall->start.x;
		line.y1 = wall->start.y;
		line.x2 = wall->end.x;
		line.y2 = wall->end.y;
		render_line(&line);
		wall = wall->next;
	}
	edt->selection_status = 0;
	edt->selection_room_id = -1;
	ft_putendl("Deselected and dehighlighted a room from blue back to white.");
    redraw_portals_in_red(edt);
}

static void highlight_room_selection(t_editor *edt)
{
	int count;
	t_line line;
	t_wall *wall;
	t_room *room;

	line.doom = edt->parent;
	line.buff = edt->buff;
	line.color = 0xff0000ff;
	count = edt->room_count;
	room = edt->room_first;
	while (count-- && room->id != edt->hover_id)
		room = room->next;
	dehover(edt);
	edt->hover_status = 0;
	// Here, we dehighlight the old selection if there was one
	if (edt->selection_status == 1)
		dehighlight_room_selection(edt);
	edt->selection_status = 1;
	edt->selection_room_id = edt->hover_id;
	count = room->wall_count;
	wall = room->first_wall;
	while (count--)
	{
		line.x1 = wall->start.x;
		line.y1 = wall->start.y;
		line.x2 = wall->end.x;
		line.y2 = wall->end.y;
		render_line(&line);
		wall = wall->next;
	}
	ft_putendl("Selected and highlighted a room in blue.");
    redraw_portals_in_red(edt);
}

static void edt_left_click(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	if (doom->edt->hover_status == 1)
	{
		if (doom->edt->selection_room_id != doom->edt->hover_id)
			highlight_room_selection(doom->edt);
		return;
	}
	color = 0xffffffff;
	i = doom->event.motion.x;
	j = doom->event.motion.y * EDT_WIN_WIDTH;
	expand_polygon(doom->event.motion.x, doom->event.motion.y, doom->edt);
	if (doom->edt->polygon_binding)
	{
		i = doom->edt->polygon_start_x;
		j = doom->edt->polygon_start_y * EDT_WIN_WIDTH;
		color = 0xff00ff00;
	}
	if (doom->edt->portalization_binding)
	{
		i = doom->edt->portal_x;
		j = doom->edt->portal_y * EDT_WIN_WIDTH;
		color = 0xffff0000;
		doom->edt->portalization_binding = 0;
		doom->edt->portalization_ending = 1;
	}
	if (doom->edt->new_portal_x >= 0)
	{
		i = doom->edt->new_portal_x;
		j = doom->edt->new_portal_y * EDT_WIN_WIDTH;
		color = 0xffff0000;
		doom->edt->new_portal_x = -1;
		doom->edt->new_portal_y = -1;
	}
	pixels = doom->edt->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + EDT_WIN_WIDTH] = color;
	pixels[i + j - EDT_WIN_WIDTH] = color;
	Mix_PlayChannel( -1, doom->sounds->mcPlop, 0 );
}

static void edt_right_click(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	color = 0xff00ff00;
	if (doom->edt->player_set < 1)
		color = 0xffffff00;
	i = doom->event.motion.x;
	j = doom->event.motion.y * EDT_WIN_WIDTH;
	if (doom->edt->player_set < 1)
	{
		record_player(doom->event.motion.x, doom->event.motion.y, doom->edt);
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0);
		if (doom->edt->player_set == 1)
			return (circle_player(doom));
	}
	else
	{
		record_enemy(doom->event.motion.x, doom->event.motion.y, doom->edt);
		Mix_PlayChannel( -1, doom->sounds->mcSteam, 0);
		if (doom->edt->enemy_set == 0)
			return (circle_enemy(doom));
	}
	pixels = doom->edt->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + EDT_WIN_WIDTH] = color;
	pixels[i + j - EDT_WIN_WIDTH] = color;
}

void 		edt_mouse_down(t_doom *doom)
{
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
	{
		edt_right_click(doom);
	}
	else if (doom->event.button.button == SDL_BUTTON_LEFT)
	{
		edt_left_click(doom);
	}
}

static void print_room(t_doom *doom, SDL_Surface *buff, t_room *room, uint32_t color)
{
	int 	wc;
	t_wall	*wall;
	t_line	line;

	wc = room->wall_count;
	wall = room->first_wall;
	line.doom = doom;
	line.buff = buff;
	line.color = color;
	while (wc--)
	{
		line.x1 = wall->start.x;
		line.y1 = wall->start.y;
		line.x2 = wall->end.x;
		line.y2 = wall->end.y;
		render_line(&line);
		wall = wall->next;
	}
}

static void print_corners(SDL_Surface *buff, t_room *room, uint32_t color)
{
	int 	wc;
	t_wall	*wall;

	wc = room->wall_count;
	wall = room->first_wall;
	set_pixel(buff, wall->start.x, wall->start.y, color);
	set_pixel(buff, wall->start.x + 1, wall->start.y, color);
	set_pixel(buff, wall->start.x - 1, wall->start.y, color);
	set_pixel(buff, wall->start.x, wall->start.y + 1, color);
	set_pixel(buff, wall->start.x, wall->start.y - 1, color);
	while (wc--)
	{
		set_pixel(buff, wall->end.x, wall->end.y, color);
		set_pixel(buff, wall->end.x + 1, wall->end.y, color);
		set_pixel(buff, wall->end.x - 1, wall->end.y, color);
		set_pixel(buff, wall->end.x, wall->end.y + 1, color);
		set_pixel(buff, wall->end.x, wall->end.y - 1, color);
		wall = wall->next;
	}
}

static void highlight_subselection(t_editor *edt, uint32_t color)
{
	if (edt->subselection_wall != NULL)
	{
		ft_putendl("Highlighted subselection");
		set_pixel(edt->buff, edt->subselection_wall->start.x, edt->subselection_wall->start.y, color);
		set_pixel(edt->buff, edt->subselection_wall->start.x - 1, edt->subselection_wall->start.y, color);
		set_pixel(edt->buff, edt->subselection_wall->start.x + 1, edt->subselection_wall->start.y, color);
		set_pixel(edt->buff, edt->subselection_wall->start.x, edt->subselection_wall->start.y - 1, color);
		set_pixel(edt->buff, edt->subselection_wall->start.x, edt->subselection_wall->start.y + 1, color);
	}
}

static void wipe_print_selection_room(t_editor *edt)
{
	int count;
	t_room *room;

	room = edt->room_first;
	count = edt->room_count;
	while (count-- && edt->selection_room_id != room->id)
		room = room->next;
	if (room->id != edt->selection_room_id)
	{
		ft_putendl("Warning: Tried to wipe_print_selection_room without matching room found.");
		return ;
	}
	print_room(edt->parent, edt->buff, room, 0xff000000);
	print_corners(edt->buff, room, 0xff000000);
	circle_visual(edt->buff, &room->visual, 0xff000000);
}

static int report_connected_rooms(t_editor *edt, t_wall *portal, t_room *origin_room, int *flags)
{
	int rc;
	int wc;
	int extras;
	t_room *room;
	t_wall *wall;

	rc = edt->room_count;
	room = edt->room_first;
	extras = 0;
	while (rc--)
	{
		if (origin_room->id == room->id)
		{
			room = room->next;
			continue;
		}
		wc = room->wall_count;
		wall = room->first_wall;
		flags[room->id] = 0;
		while (wc--)
		{
			if (((portal->start.x == wall->start.x && portal->end.x == wall->end.x)
				&& (portal->end.y == wall->end.y && portal->start.y == wall->start.y))
				|| (portal->start.x == wall->end.x && portal->start.y == wall->end.y)
				&& (portal->end.x == wall->start.x && portal->end.y == wall->start.y))
			{
				flags[room->id] = 1;
			}
			wall = wall->next;
		}
		if (flags[room->id])
		{
			//printf("Room_id %d connected to origin room\n", room->id);
			extras++;
		}
		room = room->next;
	}
	return (extras);
}

static void raw_move_by_id(t_editor *edt, int room_id, int delta_x, int delta_y)
{
	int count;
	t_room *room;
	t_wall *wall;

	room = edt->room_first;
	count = edt->room_count;
	while (count-- && room_id != room->id)
		room = room->next;
	wipe_room_polygon_map(room, edt->parent);
	print_room(edt->parent, edt->buff, room, 0xff000000);
	print_corners(edt->buff, room, 0xff000000);
	circle_visual(edt->buff, &room->visual, 0xff000000);
	count = room->wall_count;
	wall = room->first_wall;
	while (count--)
	{
		wall->start.x += delta_x;
		wall->end.x += delta_x;
		wall->start.y += delta_y;
		wall->end.y += delta_y;
		wall = wall->next;
	}
	room->visual.x += delta_x;
	room->visual.y += delta_y;
	circle_visual(edt->buff, &room->visual, 0xffffffff);
	expand_room_polygon_map(room, edt->parent, edt->poly_map, edt->conversion_colors);
	print_corners(edt->buff, room, 0xffffffff);
	print_room(edt->parent, edt->buff, room, 0xff0000ff);
}

static void move_portals_of_room(t_editor *edt, t_room *room, int delta_x, int delta_y)
{
	int 	flags[512];
	int 	extras;
	int 	wc;
	int		pc;
	t_wall	*wall;
	t_wall	*portal;

	pc = edt->portal_count;
	portal = edt->portal_begin;
	while (pc--)
	{
		wc = room->wall_count;
		wall = room->first_wall;
		while (wc--)
		{
			if (portal->start.x == wall->start.x && portal->start.y == wall->start.y)
			{
				if (portal->end.x == wall->end.x && portal->end.y == wall->end.y)
				{
					extras = 0;
					while (extras < 512)
						flags[extras++] = 0;
					extras = report_connected_rooms(edt, portal, room, &flags);
					ft_putnbr(extras);
					ft_putendl(" extra rooms reported in total.");
					extras = 0;
					while (extras < 512)
					{
						if (flags[extras])
						{
							raw_move_by_id(edt, extras, delta_x, delta_y);
							ft_putendl("Raw moved an extra connected room.");
						}
						extras++;
					}
					portal->start.x += delta_x;
					portal->start.y += delta_y;
					portal->end.x += delta_x;
					portal->end.y += delta_y;
					//ft_putendl("Moved a portal of a room with delta values.");
				}
			}
			wall = wall->next;
		}
		portal = portal->next;
	}
	// This is all fine and dandy, but now, a chain reaction must occur. A connected room must also move!
	//ft_putendl("Ended checking for movable portals when moving a room.");
}

static void move_selection(t_editor *edt, int originator, int delta_x, int delta_y)
{
	int count;
	t_room *room;
	t_wall *wall;

	room = edt->room_first;
	count = edt->room_count;
	while (count-- && edt->selection_room_id != room->id)
		room = room->next;
	// Hold of room pointer gained. Must now delete room from polymap.
	wipe_room_polygon_map(room, edt->parent);
	// Must delete pixels from buffer with black overwrite.
	print_room(edt->parent, edt->buff, room, 0xff000000);
	print_corners(edt->buff, room, 0xff000000);
	circle_visual(edt->buff, &room->visual, 0xff000000);
	// Here, below, the actual data manipulation takes place. Portals are not updated, but must be!
	// Therefore, below, is a new function, for that, move_portals_of_room()
	highlight_subselection(edt, 0xff000000);
	if (originator)
		move_portals_of_room(edt, room, delta_x, delta_y);
	count = room->wall_count;
	wall = room->first_wall;
	while (count--)
	{
		wall->start.x += delta_x;
		wall->end.x += delta_x;
		wall->start.y += delta_y;
		wall->end.y += delta_y;
		wall = wall->next;
	}
	room->visual.x += delta_x;
	room->visual.y += delta_y;
	circle_visual(edt->buff, &room->visual, 0xffffffff);
	// Must recreate polygon to polymap.
	expand_room_polygon_map(room, edt->parent, edt->poly_map, edt->conversion_colors);
	// Must recreate pixels to buffer with default blue color, marking the persisting selection
	print_corners(edt->buff, room, 0xffffffff);
	print_room(edt->parent, edt->buff, room, 0xff0000ff);
	highlight_subselection(edt, 0xffff00ff);
	//SDL_UpdateWindowSurface(edt->win);
	// Must recreate roomstring. Must recreate wall strings.
	// UNDONE
	//ft_putendl("Moved selection at Editor.");
	//MUST ALSO MOVE CONNECTED ROOMS AND PORTALS AND RESPECT THE GRAPHICAL REPRESENTATIONS
	//MUST REDRAW PORTALS IN RED
	redraw_portals_in_red(edt);
}

static void move_subselection(t_editor *edt, int delta_x, int delta_y)
{
	int count;
	t_wall *wall;

	// THIS MESSES UP PORTALS AT THE MOMENT, BUT MUST BE ADAPTED TO LOOK AND UPDATE PORTALS AS WELL
	// THIS NEEDS TO UPDATE POLYMAP AS WELL
	if (edt->subselection_wall != NULL)
	{
	    //highlight_subselection(edt, 0xff000000);
		wipe_print_selection_room(edt);
		wall = edt->wall_begin;
		count = edt->wall_count;
		while (count--)
		{
			if (wall->end.x == edt->subselection_wall->start.x && wall->end.y == edt->subselection_wall->start.y)
			{
				wall->end.x += delta_x;
				wall->end.y += delta_y;
			}
			wall = wall->next;
		}
		edt->subselection_wall->start.x += delta_x;
		edt->subselection_wall->start.y += delta_y;
		ft_putendl("Moved subselection");
		move_selection(edt, 0, 0, 0);
	}
}

static void cycle_subselection(t_editor *edt)
{
	t_room 	*room;
	t_wall	*wall;
	int 	count;

	room = edt->room_first;
	count = edt->room_count;
	while (count-- && room->id != edt->selection_room_id)
		room = room->next;
	if (edt->subselection_id == -1)
	{
		edt->subselection_id = room->first_wall->id;
		edt->subselection_wall = room->first_wall;
		return ;
	}
	count = room->wall_count;
	wall = room->first_wall;
	while (count-- && wall->id != edt->subselection_id)
		wall = wall->next;
	if (wall->id == edt->subselection_id && count != 0)
	{
		edt->subselection_id = wall->next->id;
		edt->subselection_wall = wall->next;
	}
	else
	{
		edt->subselection_id = room->first_wall->id;
		edt->subselection_wall = room->first_wall;
	}
	ft_putendl("Cycling subselection at Editor, SPACE key was pressed.");
}

static int remove_selected_rooms_walls(t_editor *edt, t_room *room)
{
    int ret;
    int wc;
    int redistribute_from_id;
    t_wall *wall;
    t_wall *next;

    redistribute_from_id = room->first_wall->id - 1;
    wc = room->wall_count;
    edt->wall_count -= wc;
    printf("Deleting %d wall ids\n", wc);
    ret = wc;
    wall = room->first_wall;
    next = wall->next;
    printf("Destroying wall id(%d)\n", wall->id);
    free(wall);
    wc--;
    while (wc--)
    {
        wall = next;
        next = wall->next;
        printf("Destroying wall id(%d)\n", wall->id);
        free(wall);
    }
    // Took care of freeing memory used by deleted walls above
    printf("Wall id(%d) is the next wall to be linked!\n", next->id);
    // Taking care of redistributing ids for the wall ids below
    wc = edt->wall_count;
    wall = edt->wall_begin;
    while (wc-- && wall->id != redistribute_from_id)
        wall = wall->next;
    // Arriving at the correct wall to start redistributions from. We link it to the next surviving wall.
    printf("Wall id(%d) is the origin wall, to which we link the one wall that was to be linked!\n", wall->id);
    wall->next = next;
    wall = wall->next;
    // Now iterate through the surviving walls, redistributing the ids.
    while (wc--)
    {
        printf("Wall id(%d) previously\n", wall->id);
        wall->id = ++redistribute_from_id;
        printf("Wall id(%d) after change\n", wall->id);
        wall = wall->next;
    }
    return (ret);
}

static void remove_selected_room_data(t_editor *edt)
{
    int     reduction;
    int     rc;
    int     redistribute_from_id;
    t_room  *room;
    t_room  *next;

    rc = edt->room_count;
    room = edt->room_first;
    while (rc-- && room->id != edt->selection_room_id)
        room = room->next;

    // SOURCE OF WONKY BUSINESS ??? BELOW IS MESSING UP WALL DATA ??
    //output_walls(edt->wall_count, edt->wall_begin);
    //output_rooms(edt->room_count, edt->room_first);
    reduction = remove_selected_rooms_walls(edt, room);
    // Use next as temp to retain deletion room pointer
    next = room;
    while (rc--)
    {
        //ft_putendl("Minusing RC for reduction to fire...");
        room = room->next;
        room->first_wall_id -= reduction;
    }
    //output_walls(edt->wall_count, edt->wall_begin);
    output_rooms(edt->room_count, edt->room_first);
    // Gain back the deletion room pointer
    room = next;
    // DO THIS LATER, WALLS AND ROOMS NEED TO WORK PERFECTLY FIRST!!
    //remove_selected_rooms_portals(edt, room);

    // Saving the next valid room after soon-to-be-deleted room into "next"
    next = room->next;
    // Calculating the last valid ID after which the redistribution of room IDs will begin
    redistribute_from_id = room->id - 1;
    // Deallocating memory and deletion of the data.
    free(room);
    edt->room_count--;
    // Special case, deleting the first room
    if (redistribute_from_id == -1)
    {
        ft_putendl("Special case, reassigning room_first and exiting deletion function.");
        edt->room_first = next;
        rc = edt->room_count;
        room = edt->room_first;
        while (rc--)
        {
            room->id = ++redistribute_from_id;
            room = room->next;
        }
        return ;
    }
    // Iterating to the room that comes just before the room that will be deleted
    rc = edt->room_count;
    room = edt->room_first;
    while (rc-- && room->id != redistribute_from_id)
    {
        printf("CHECK ME rc = %d | Room id %d | redistribute_from_id %d \n", rc, room->id, redistribute_from_id);
        room = room->next;
    }
    // Here, arrived at the valid room. Swap its next pointer to the previously saved next room.
    printf("rc is %d | Room arrived at, id is %d | redistribute_from_id is %d\n", rc, room->id, redistribute_from_id);
    room->next = next;
    room = room->next;
    // Continue iterating, redistributing ids in a while loop
    while (rc--)
    {
        room->id = ++redistribute_from_id;
        //printf("rc is %d | new room_id was %d\n", rc, room->id);
        expand_room_polygon_map(room, edt->parent, edt->poly_map, &edt->conversion_colors);
        ft_putendl("Redrew to polymap with new color based off new ID.");
        room = room->next;
    }
    output_rooms(edt->room_count, edt->room_first);
}

static void remove_selected_room(t_editor *edt)
{
    int     rc;
    t_room  *room;

    printf("Room count before wipes: %d\n", edt->room_count);
    rc = edt->room_count;
    room = edt->room_first;
    while (rc-- && room->id != edt->selection_room_id)
        room = room->next;
    wipe_print_selection_room(edt);
    wipe_room_polygon_map(room, edt->parent);
    remove_selected_room_data(edt);
    edt->hover_status = 0;
    edt->hover_id = -1;
    edt->selection_room_id = -1;
    edt->selection_status = 0;
    ft_putendl("Wiped selected room from buffer and data.");
    printf("Room count after wipes: %d\n", edt->room_count);
}

void		edt_render(t_doom *doom)
{
    /* Manual debugging for room_id_query */
/*
    static int	mouse_x = 0;
    static int	mouse_y = 0;
    static int 	queried_room_id = -1;

    if (doom->keystates[SDL_SCANCODE_I])
    {
		SDL_GetMouseState(&mouse_x, &mouse_y);
		queried_room_id = room_id_from_pixel(doom->edt->poly_map, mouse_x, mouse_y);
		if (queried_room_id == -1)
			printf("At location %d, %d there was empty space and no room_id found\n", mouse_x, mouse_y);
		else
			printf("Room_id at location %d, %d was %d\n", mouse_x, mouse_y, queried_room_id);
	}*/
/* Manual debugging for polymap
static int	was_blitted = 0;
if (doom->keystates[SDL_SCANCODE_SPACE] && !was_blitted)
{
	// SAVE BUFFER OF THE WINDOW TO BACK BUFFER
	SDL_BlitSurface(doom->edt->buff, NULL, doom->edt->back_buffer, NULL);
	// COPY OVER THE WINDOWS BUFFER WITH POLY MAP BUFFER
	SDL_BlitSurface(doom->edt->poly_map, NULL, doom->edt->buff, NULL);
	SDL_UpdateWindowSurface(doom->edt->win);
	// LOCK BLITTING WITH SPACE HELD
	was_blitted = 1;
	ft_putendl("Activating poly_map view.");
}
if (!doom->keystates[SDL_SCANCODE_SPACE] && was_blitted)
{
	// WHEN SPACE NOT HELD ANYMORE, BLIT SAVED BACK BUFFER ONTO WINDOW BUFFER AGAIN
	SDL_BlitSurface(doom->edt->back_buffer, NULL, doom->edt->buff, NULL);
	SDL_UpdateWindowSurface(doom->edt->win);
	// NORMALIZE STATE
	was_blitted = 0;
	ft_putendl("Returning screen to normal.");
}
*/

	static int delta = 5;
	static int cycling_lock = 0;
	static int delete_lock = 0;

	if (doom->edt->selection_status && (doom->keystates[SDL_SCANCODE_LSHIFT] || doom->keystates[SDL_SCANCODE_RSHIFT]))
	{
		if (doom->keystates[SDL_SCANCODE_UP])
			move_subselection(doom->edt, 0, -delta);
		if (doom->keystates[SDL_SCANCODE_DOWN])
			move_subselection(doom->edt, 0, delta);
		if (doom->keystates[SDL_SCANCODE_LEFT])
			move_subselection(doom->edt, -delta, 0);
		if (doom->keystates[SDL_SCANCODE_RIGHT])
			move_subselection(doom->edt, delta, 0);
	}
	else if (doom->edt->selection_status && doom->keystates[SDL_SCANCODE_DELETE] && !delete_lock)
	{
		//ft_putendl("Delete detected.");
		delete_lock = 1;
		remove_selected_room(doom->edt);
	}
	else if (doom->edt->selection_status && doom->keystates[SDL_SCANCODE_SPACE] && !cycling_lock)
	{
		//ft_putendl("Space detected.");
		cycling_lock = 1;
		highlight_subselection(doom->edt, 0xffffffff);
		cycle_subselection(doom->edt);
		highlight_subselection(doom->edt, 0xffff00ff);
	}
	else if (doom->edt->selection_status)
	{
		if (doom->keystates[SDL_SCANCODE_UP])
			move_selection(doom->edt, 1, 0, -(delta*2));
		if (doom->keystates[SDL_SCANCODE_DOWN])
			move_selection(doom->edt, 1, 0, (delta*2));
		if (doom->keystates[SDL_SCANCODE_LEFT])
			move_selection(doom->edt, 1, -(delta*2), 0);
		if (doom->keystates[SDL_SCANCODE_RIGHT])
			move_selection(doom->edt, 1, (delta*2), 0);
	}
	if (delete_lock && doom->keystates[SDL_SCANCODE_DELETE] == 0)
		delete_lock = 0;
	if (cycling_lock && doom->keystates[SDL_SCANCODE_SPACE] == 0)
		cycling_lock = 0;
	// SAVE BUFFER AS IT WAS TO GRID TEMP
	flood_buffer(doom->edt->grid_temp, 0xff000000);
	SDL_BlitSurface(doom->edt->buff, NULL, doom->edt->grid_temp, NULL);
	// NOW, CLEAR THE ACTUAL WINDOW BUFFER FOR COMPOSING IT FROM TWO BLITS, GRID AND THE ACTUAL BUFFER (FROM THE TEMP)
	flood_buffer(doom->edt->buff, 0xff000000);
	SDL_BlitSurface(doom->edt->grid_buffer, NULL, doom->edt->buff, NULL);
	SDL_BlitSurface(doom->edt->grid_temp, NULL, doom->edt->buff, NULL);
	// UPDATE THIS COMPOSITED IMAGE TO SCREEN
	SDL_UpdateWindowSurface(doom->edt->win);
	// NOW, CLEAR AND SWAP BACK THE ACTUAL BUFFER TO WHERE IT BELONGS TO
	flood_buffer(doom->edt->buff, 0xff000000);
	SDL_BlitSurface(doom->edt->grid_temp, NULL, doom->edt->buff, NULL);
	// IN THE FUTURE, IF GRID IS TO STAY, REDUCE BLITS AND AUTO COMPOSE OF LAYERS BY DEFAULT ALWAYS!
}
#endif