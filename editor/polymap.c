#include "doom-nukem.h"

int				room_id_from_polymap(SDL_Surface *polymap, int x, int y)
{
	unsigned int	*pixels;
	int 			location;
	int 			limit;

	location = x + polymap->w * y;
	limit = polymap->w * polymap->h;
	pixels = polymap->pixels;
	if (location >= limit || location < 0)
	{
		printf("Warning: room_id_from_polymap tried to get room id at location %d, %d which is outside buffer memory area. Operation was blocked.\n", x, y);
		return (-1);
	}
	if (pixels[location] == 0xffffffff)
		return (-1);
	return ((int)pixels[location]);
}

static uint32_t  convert_to_color(int room_id, uint32_t *conversion_colors)
{
    return (conversion_colors[room_id]);
}

static void init_debug_convs(uint32_t *debug_convs)
{
	int i;

	puts("Visual aid for debugging polymaps initialized, 6 unique debug colors for room ids 0-5!");
	debug_convs[0] = 0xffff0000;
	debug_convs[1] = 0xff00ff00;
	debug_convs[2] = 0xff0000ff;
	debug_convs[3] = 0xffffff00;
	debug_convs[4] = 0xff00ffff;
	debug_convs[5] = 0xffff00ff;
	i = 6;
	while (i < 512)
	{
		debug_convs[i] = (uint32_t) i;
		i++;
	}
}

static void	init_conv_colors(uint32_t *conv_colors)
{
	int i;

	i = 0;
	while (i < 512)
	{
		conv_colors[i] = (uint32_t) i;
		i++;
	}
}

uint32_t 	*get_debug_convs(void)
{
	static uint32_t *debug_convs = NULL;

	if (!debug_convs)
	{
		debug_convs = (uint32_t*)malloc(sizeof(uint32_t) * 512);
		init_debug_convs(debug_convs);
	}
	return (debug_convs);
}

uint32_t	*get_conv_colors(void)
{
	static uint32_t *conv_colors = NULL;

	if (!conv_colors)
	{
		conv_colors = (uint32_t*)malloc(sizeof(uint32_t) * 512);
		init_conv_colors(conv_colors);
	}
	return (conv_colors);
}

/*
void        create_room_polygon_map(t_editor *edt)
{
    edt->poly_map = SDL_CreateRGBSurfaceWithFormat(0, GAME_POLYMAP_WIDTH, GAME_POLYMAP_HEIGHT, 32, SDL_GetWindowPixelFormat(edt->win));
    edt->back_buffer = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 32, SDL_GetWindowPixelFormat(edt->win));
	edt->front_buffer = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 32, SDL_GetWindowPixelFormat(edt->win));
	printf("Pixel format for edt->win is %s\n", SDL_GetPixelFormatName(SDL_GetWindowPixelFormat(edt->win)));
    flood_buffer(edt->buff, 0xff000000);
    flood_buffer(edt->poly_map, 0xffffffff);
    flood_buffer(edt->back_buffer, 0xff000000);
    flood_buffer(edt->front_buffer, 0x00000000);
	SDL_SetColorKey(edt->front_buffer, SDL_TRUE, 0x00000000);
    if (edt->poly_map == NULL || edt->back_buffer == NULL || edt->front_buffer == NULL)
        ft_die("Fatal error: SDL_CreateRGBSurface() failed at create_room_polygon_map for poly_map or front_buffer or back_buffer.");
    ft_putendl("Created room polygon map and front and back buffer!");
    init_conversion_colors(edt->conversion_colors);
}*/

static int  find_nearest_x(int farthest, t_room *room)
{
    int     wc;
    int     smallest;
    t_wall  *wall;

    //ft_putendl("finding nearest x");
    smallest = farthest;
    wc = room->wall_count;
    wall = room->first_wall;
    while (wc--)
    {
        if (wall->start.x < smallest)
            smallest = wall->start.x;
        if (wall->end.x < smallest)
            smallest = wall->end.x;
        wall = wall->next;
    }
    return (smallest - 1);
}

static int  find_nearest_y(int farthest, t_room *room)
{
    int     wc;
    int     smallest;
    t_wall  *wall;

    //ft_putendl("finding nearest y");
    smallest = farthest;
    wc = room->wall_count;
    wall = room->first_wall;
    while (wc--)
    {
        if (wall->start.y < smallest)
            smallest = wall->start.y;
        if (wall->end.y < smallest)
            smallest = wall->end.y;
        wall = wall->next;
    }
    return (smallest - 1);
}

static int  find_farthest_x(t_room *room)
{
    int     wc;
    int     biggest;
    t_wall  *wall;

    //ft_putendl("finding farthest x");
    biggest = 0;
    wc = room->wall_count;
    wall = room->first_wall;
    while (wc--)
    {
        if (wall->start.x > biggest)
            biggest = wall->start.x;
        if (wall->end.x > biggest)
            biggest = wall->end.x;
        wall = wall->next;
    }
    return (biggest + 1);
}

static int find_farthest_y(t_room *room)
{
    int     wc;
    int     biggest;
    t_wall  *wall;

    //ft_putendl("finding farthest y");
    biggest = 0;
    wc = room->wall_count;
    wall = room->first_wall;
    while (wc--)
    {
        if (wall->start.y > biggest)
            biggest = wall->start.y;
        if (wall->end.y > biggest)
            biggest = wall->end.y;
        wall = wall->next;
    }
    return (biggest + 1);
}

static int  inside_room(int x, int y, t_room *room)
{
    int     cn;
    int     wc;
    double  vt;
    t_point v0;
    t_point v1;
    t_wall  *wall;

    cn = 0;
    wc = room->wall_count;
    wall = room->first_wall;

    while (wc--)
    {
        v0.x = wall->start.x;
        v0.y = wall->start.y;
        v1.x = wall->end.x;
        v1.y = wall->end.y;
        if ((v0.y <= y) && (v1.y > y) || ((v0.y > y) && (v1.y <= y)))
        {
            vt = (double)(y - v0.y) / (v1.y - v0.y);
            if (x < v0.x + vt * (v1.x - v0.x))
                cn++;
        }
        wall = wall->next;
    }
    // Odd or even bitwise magic.
    return (cn&1);
}
/*
void 		wipe_room_polygon_map(t_room *room, t_doom *doom)
{
	int bounding_x_upper_limit;
	int bounding_y_upper_limit;
	int x_start;
	int x;
	int y;

	bounding_x_upper_limit = find_farthest_x(room);
	bounding_y_upper_limit = find_farthest_y(room);
	x_start = find_nearest_x(bounding_x_upper_limit, room);
	y = find_nearest_y(bounding_y_upper_limit, room);
	x = x_start;
	while (y < bounding_y_upper_limit)
	{
		while (x < bounding_x_upper_limit)
		{
			if (inside_room(x, y, room))
			{
				set_pixel(doom->edt->poly_map, x, y, 0xffffffff);
			}
			x++;
		}
		x = x_start;
		y++;
	}
}*/

// TODO OPTIMIZE AWAY FROM SET_PIXEL

void		add_room_polymap(t_room *room, SDL_Surface *polymap, uint32_t *conv_colors)
{
    int bound_x;
    int bound_y;
    int x_start;
    int x;
    int y;

    bound_x = find_farthest_x(room);
    bound_y = find_farthest_y(room);
    x_start = find_nearest_x(bound_x, room);
    y = find_nearest_y(bound_y, room);
    x = x_start;
    	//printf("add_room_polymap | x, y = %d, %d | bound_x, y = %d, %d\n", x, y, bound_x, bound_y);
    while (y < bound_y)
    {
        while (x < bound_x)
        {
            if (inside_room(x, y, room))
				set_pixel(polymap, x, y, conv_colors[room->id]);
            x++;
        }
        x = x_start;
        y++;
    }
		//printf("%x color from conv_colors[room->id]\n", conv_colors[room->id]);
}

void 		repaint_polymap(t_model *mdl)
{
	t_room	*room;
	int 	rc;

	room = mdl->room_first;
	rc = mdl->room_count;
	flood_buffer(mdl->poly_map, 0xffffffff);
	while (rc--)
	{
		add_room_polymap(room, mdl->poly_map, get_conv_colors());
		room = room->next;
	}
		//puts("Repainted polymap succesfully!");
}
