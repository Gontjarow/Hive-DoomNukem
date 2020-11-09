#include "doom-nukem.h"
/*
static void		link_rooms(t_editor *edt)
{
	t_room *room;
	int 	rc;

	rc = edt->room_count;
	room = edt->room_first;
	while (rc--)
	{
		room->first_wall = wall_by_id(edt, room->first_wall_id);
		room = room->next;
	}
}

void			transfer_model_to_editor(t_doom *doom)
{
	int	ec;

	ft_putendl("Transferring model to editor.");
	doom->edt->walls = doom->mdl->walls;
	doom->edt->rooms = doom->mdl->rooms;
	doom->edt->enemies = doom->mdl->enemies;
	doom->edt->portals = doom->mdl->portals;
	doom->edt->enemy_first = doom->mdl->enemy_first;
	doom->edt->portal_begin = doom->mdl->portal_first;
	doom->edt->wall_begin = doom->mdl->wall_first;
	doom->edt->room_first = doom->mdl->room_first;
	doom->edt->wall_count = doom->mdl->wall_count;
	doom->edt->room_count = doom->mdl->room_count;
	doom->edt->enemy_count = doom->mdl->enemy_count;
	doom->edt->portal_count = doom->mdl->portal_count;
	doom->edt->player = doom->mdl->player;
	doom->edt->player_set = 1;
	circle_player(doom);
	link_rooms(doom->edt);
	if (doom->map->player_string)
		doom->edt->player_string = ft_strdup(doom->map->player_string);
	if (doom->map->wall_string)
		doom->edt->wall_string = ft_strdup(doom->map->wall_string);
	if (doom->map->room_string)
		doom->edt->room_string = ft_strdup(doom->map->room_string);
	if (doom->map->portal_string)
		doom->edt->portal_string = ft_strdup(doom->map->portal_string);
	if (doom->map->enemy_string)
		doom->edt->enemy_string = ft_strdup(doom->map->enemy_string);
	print_walls(doom->edt);
	circle_rooms(doom);
	ec = doom->edt->enemy_count;
	if (ec == 0)
		return ;
	doom->edt->enemies = doom->edt->enemy_first;
	doom->edt->last_enemy.x = doom->edt->enemies->x;
	doom->edt->last_enemy.y = doom->edt->enemies->y;
	while (ec--)
	{
		circle_enemy(doom);
		doom->edt->enemies = doom->edt->enemies->next;
		doom->edt->last_enemy.x = doom->edt->enemies->x;
		doom->edt->last_enemy.y = doom->edt->enemies->y;
	}
	doom->edt->enemies = doom->mdl->enemies;
}*/

