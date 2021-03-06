#include "doom-nukem.h"
#include <stdio.h>

void 	debug_model_player(void)
{
	t_model	*mdl;

	mdl = get_model();
	printf("Player x: %d | y: %d | rot: %d | hp cur/max: %d/%d\n",
		   (int)mdl->player.x, (int)mdl->player.y, mdl->player.rot, mdl->player.hp.cur, mdl->player.hp.max);
}

void 	debug_model_enemies(void)
{
	int		ec;
	t_enemy	*enemy;

	ec = get_model()->enemy_count;
	if (ec == 0)
		return (ft_putendl("Outputting no data to console for model's enemies, enemy count was 0."));
	printf("Outputting data to console for model's %d enemies\n", ec);
	enemy = get_model()->enemy_first;
	while (ec--)
	{
		printf("Enemy id: %d | x: %d | y: %d | rot: %d | hp cur/max: %d/%d | wep.type_id: %d",
			   enemy->id, enemy->x, enemy->y, enemy->rot, enemy->hp.cur, enemy->hp.max, enemy->wep.type_id);
		if (enemy->next == NULL)
			printf(" | next: NULL\n");
		else
			printf(" | next_id: %d\n", enemy->next->id);
		enemy = enemy->next;
	}
	printf("mdl->enemies: ");
	if (get_model()->enemies == NULL)
		printf("NULL\n");
	else
	{
		enemy = get_model()->enemies;
		printf("Enemy id: %d | x: %d | y: %d | rot: %d | hp cur/max: %d/%d | wep.type_id: %d",
			   enemy->id, enemy->x, enemy->y, enemy->rot, enemy->hp.cur, enemy->hp.max, enemy->wep.type_id);
		if (enemy->next == NULL)
			printf(" | next: NULL\n");
		else
			printf(" | next_id: %d\n", enemy->next->id);
	}
}

void 	debug_model_walls(void)
{
	int		wc;
	t_wall	*wall;

	wc = get_model()->wall_count;
	if (wc == 0)
		return (ft_putendl("Outputting no data to console for model's walls, wall count was 0."));
	printf("Outputting data to console for model's %d walls\n", wc);
	wall = get_model()->wall_first;
	while (wc--)
	{
		printf("Wall id: %d | start: %d %d | end: %d %d",
			   wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		if (wall->next == NULL)
			printf(" | next: NULL\n");
		else
		{
			printf(" | next_id: %d\n", wall->next->id);
			if (get_model()->walls == wall->next)
				ft_putendl("Confirmed a link from wall->next to mdl->walls.");
			wall = wall->next;
		}
	}

	printf("mdl->walls: ");
	if (get_model()->walls == NULL)
		printf("NULL\n");
	else
	{
		wall = get_model()->walls;

		printf("Wall id: %d | start: %d %d | end: %d %d\n",
			   wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
	}
}

void	debug_model_rooms(void)
{
	t_room	*rooms;
	int		rc;

	if (get_model()->room_count != 0)
		rc = get_model()->room_count;
	rooms = get_model()->room_first;
	puts("Outputting data for model's rooms:");
	while (rc--)
	{
		printf("Room id: %d | first_wall_id: %d | wall_count: %d | floor_height: %d | roof_height: %d\n",
			   rooms->id, rooms->first_wall_id, rooms->wall_count, rooms->floor_height, rooms->roof_height);
		rooms = rooms->next;
	}
}

void	debug_model_portals(void)
{
	t_wall	*portals;
	int		pc;

	if (get_model()->portal_count == 0)
		return (ft_putendl("Outputting no data to console for model's portals, portal count was 0."));
	pc = get_model()->portal_count;
	portals = get_model()->portal_first;
	puts("Outputting data for model's portals:");
	while (pc--)
	{
		printf("Portal id: %d | start: %d %d | end: %d %d\n",
			   portals->id, portals->start.x, portals->start.y, portals->end.x, portals->end.y);
		portals = portals->next;
	}
}

void	debug_model_effects(void)
{
	char 		**types = { "EFFECT_EXIT", "EFFECT_KEY", "EFFECT_LIGHT" };
	t_effect	*effect;
	int			ec;

	if (get_model()->effect_count == 0)
		return (ft_putendl("Outputting no data to console for model's effects, effect count was 0."));
	ec = get_model()->effect_count;
	effect = get_model()->effect_first;
	puts("Outputting data for model's effects:");
	while (ec--)
	{
		printf("[Effect] id = %d | type_id = %d (%s) | loc.x = %d | loc.y = %d | target.x = %d | target.y = %d | target_id = %d\n",
			   effect->id, effect->type_id, &types[effect->type_id], effect->loc.x, effect->loc.y, effect->target.x, effect->target.y, effect->target_id);
		effect = effect->next;
	}
}

void	debug_model_pickups(void)
{
	char 		**flavors = { "PICKUP_HEALTH", "PICKUP_AMMO", "PICKUP_WEAPON" };
	t_pickup	*pickups;
	int			pc;

	if (get_model()->pickup_count == 0)
		return (ft_putendl("Outputting no data to console for model's pickups, pickup count was 0."));
	pc = get_model()->pickup_count;
	pickups = get_model()->pickup_first;
	puts("Outputting data for model's pickups:");
	while (pc--)
	{
		printf("Pickup id: %d | loc.x: %d | loc.y: %d | flavor: %s | weapon_type_id: %d\n",
			   pickups->id, pickups->loc.x, pickups->loc.y, &flavors[pickups->flavor], pickups->weapon_type_id);
		pickups = pickups->next;
	}
}