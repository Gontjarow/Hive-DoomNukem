#include "doom_nukem.h"
#include <stdio.h>

void    output_walls(int wall_count, t_wall *walls)
{
    printf("Outputting data to console for %d walls\n", wall_count);
    while (wall_count--)
    {
        printf("Wall id: %d | start: %d %d | end: %d %d\n",
               walls->id, walls->start.x, walls->start.y, walls->end.x, walls->end.y);
        walls = walls->next;
    }
}

void    output_rooms(int room_count, t_room *rooms)
{
    printf("Outputting data to console for %d rooms\n", room_count);
    while (room_count--)
    {
        printf("Room id: %d | first_wall_id: %d | wall_count: %d | floor_height: %d | roof_height: %d\n",
                rooms->id, rooms->first_wall_id, rooms->wall_count, rooms->floor_height, rooms->roof_height);
        rooms = rooms->next;
    }
}