#include "doom-nukem.h"

int				room_id_from_pixel(SDL_Surface *buff, int x, int y)
{
	unsigned int	*pixels;
	int 			location;
	int 			limit;

	location = x + buff->w * y;
	limit = buff->w * buff->h;
	pixels = buff->pixels;
	if (location >= limit || location < 0)
	{
		printf("Warning: room_id_from_pixel tried to get pixel at %d, %d which is outside buffer memory area. Operation was blocked.\n", x, y);
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

void        init_conversion_colors(uint32_t conversion_colors[512])
{
    int i;

    /* Visual aid for debugging
    conversion_colors[0] = 0xffff0000;
    conversion_colors[1] = 0xff00ff00;
    conversion_colors[2] = 0xff0000ff;
    conversion_colors[3] = 0xffffff00;
    conversion_colors[4] = 0xff00ffff;
    conversion_colors[5] = 0xffff00ff;
    */
	ft_putendl("Conversion colors initialized.");
    i = 0;
    while (i < 512)
    {
        conversion_colors[i] = (uint32_t) i;
        i++;
    }
}

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
}

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
}

void        expand_room_polygon_map(t_room *room, t_doom *doom, SDL_Surface *poly_map, uint32_t *conversion_colors)
{
    // WORK IN PROGRESS
    int bounding_x_upper_limit;
    int bounding_y_upper_limit;
    int x_start;
    int x;
    int y;
    //static int times = 0;

    bounding_x_upper_limit = find_farthest_x(room);
    bounding_y_upper_limit = find_farthest_y(room);
    x_start = find_nearest_x(bounding_x_upper_limit, room);
    y = find_nearest_y(bounding_y_upper_limit, room);
    x = x_start;
    printf("erpm x = %d, y = %d | bound_x = %d, bound_y = %d\n", x, y, bounding_x_upper_limit, bounding_y_upper_limit);
    while (y < bounding_y_upper_limit)
    {
        while (x < bounding_x_upper_limit)
        {
            //if (times < 10)
            //    printf("Testing inside_room at expand_room_polygon_map %d\n", times++);
            if (inside_room(x, y, room))
            {
                set_pixel(poly_map, x, y, conversion_colors[room->id]);
                // printf("set_pixel(poly_map, %d, %d, %d)\n", x, y, conversion_colors[room->id]);
                //printf("set_pixel(poly_map, x, y, conversion_colors[room->id])\n");
            }
            x++;
        }
        x = x_start;
        y++;
    }
}
