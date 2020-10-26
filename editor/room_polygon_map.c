#include "doom_nukem.h"

static uint32_t  convert_to_color(int room_id, t_editor *edt)
{
    return (edt->conversion_colors[room_id]);
}

void        init_conversion_colors(uint32_t conversion_colors[512])
{
    int i;

    conversion_colors[0] = 0xffff0000;
    conversion_colors[1] = 0xff00ff00;
    conversion_colors[2] = 0xff0000ff;
    conversion_colors[3] = 0xffffff00;
    conversion_colors[4] = 0xff00ffff;
    conversion_colors[5] = 0xffff00ff;
    i = 6;
    while (i < 512)
    {
        conversion_colors[i] = (uint32_t) i;
        i++;
    }
    ft_putendl("Conversion colors initialized. Fixed 6 primary and primary blended colors then computational direct conversion.");
}

void        create_room_polygon_map(t_editor *edt)
{
    edt->poly_map = SDL_CreateRGBSurfaceWithFormat(0, 5000, 5000, 32, SDL_GetWindowPixelFormat(edt->win));
    edt->back_buffer = SDL_CreateRGBSurfaceWithFormat(0, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 32, SDL_GetWindowPixelFormat(edt->win));
    flood_window(edt->buff, 0xff000000);
    if (edt->poly_map == NULL || edt->back_buffer == NULL)
        ft_die("Fatal error: SDL_CreateRGBSurface() failed at create_room_polygon_map for poly_map or back_buffer.");
    ft_putendl("Created room polygon map and back buffer!");
    init_conversion_colors(edt->conversion_colors);
}

static int  find_nearest_x(int farthest, t_room *room)
{
    int     wc;
    int     smallest;
    t_wall  *wall;

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

void        expand_room_polygon_map(t_room *room, t_doom *doom)
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
                set_pixel(doom->edt->poly_map, x, y, convert_to_color(room->id, doom->edt));
            }
            x++;
        }
        x = x_start;
        y++;
    }
}
