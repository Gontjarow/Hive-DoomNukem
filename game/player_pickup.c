/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_pickup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:20:47 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/06 16:21:18 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static int		player_collision_with_pickup(t_doom *doom, t_pickup *pickup)
{
	int		dx;
	int		dy;
	int		ec;
	int		distance;

	dx = (int)doom->mdl->player.x - pickup->loc.x;
	dy = (int)doom->mdl->player.y - pickup->loc.y;
	distance = sqrt(dx * dx + dy * dy);
	if ((distance < 10 + PICKING_RADIUS))
		return (-1);
	return (0);
}

t_pickup		*delete_first(t_doom *doom)
{
	t_pickup	*head;
	t_pickup	*temp;

	head = doom->mdl->pickup_first;
	if (head == NULL)
		return (NULL);
	temp = head;
	head = head->next;
    free(temp);
	doom->mdl->pickup_first = head;
	return (head);
}

t_pickup		*delete_node(t_pickup *current, t_pickup *prev, t_pickup *pickup)
{
	while (current != NULL && current->id != pickup->id)
	{
		prev = current;
		current = current->next;
	}
    prev->next = current->next;
	free(current);
	pickup = prev;
	return (pickup);
}

static void		handle_health_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	// printf("Pickup ID that u touched: %d\n", pickup->id);
	pc = doom->mdl->pickup_count;
	current = doom->mdl->pickup_first;
	if (current->id == pickup->id)
		pickup = delete_first(doom);
	else
		pickup = delete_node(current, prev, pickup);
	Mix_PlayChannel(3, doom->sounds->mcHealthPickup, 0);
	doom->mdl->pickup_count--;
	doom->mdl->player.hp.cur += 50;
	if (doom->mdl->player.hp.cur > 100)
		doom->mdl->player.hp.cur = 100;
}

void		handle_ammo_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	pc = doom->mdl->pickup_count;
	current = doom->mdl->pickup_first;
	if (current->id == pickup->id)
		pickup = delete_first(doom);
	else
		pickup = delete_node(current, prev, pickup);
	Mix_PlayChannel(3, doom->sounds->mcAmmoPickup, 0);
	doom->mdl->pickup_count--;
	doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_max;
}

void		handle_weapon_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	pc = doom->mdl->pickup_count;
	current = doom->mdl->pickup_first;
	if (current->id == pickup->id)
		pickup = delete_first(doom);
	else
		pickup = delete_node(current, prev, pickup);
	// Mix_PlayChannel(3, doom->sounds->mcHealthPickup, 0);		// need to add weapon pickup sound
	doom->mdl->pickup_count--;
	/*
		Placeholder for weapon addition handling
	*/
}

void			handle_pickup(t_doom *doom)
{
	int			pc;
	t_pickup	*pickup;

	pc = doom->mdl->pickup_count;
	if (pc == 0)
		return ;
	pickup = doom->mdl->pickup_first;
	while (pc--)
	{
		// printf("Checking all pickups\n");
		if (pickup->flavor == PICKUP_HEALTH && player_collision_with_pickup
			(doom, pickup) == -1 && doom->mdl->player.hp.cur < 100)
			handle_health_pickup(doom, pickup);
		
		// Waiting for editor additional pickups

		// if (pickup->flavor == PICKUP_AMMO && player_collision_with_pickup
		// 	(doom, pickup) == -1)
		// 	handle_ammo_pickup(doom, pickup);
		// if (pickup->flavor == PICKUP_WEAPON && player_collision_with_pickup
		// 	(doom, pickup) == -1)
		// 	handle_weapon_pickup(doom, pickup);
		pickup = pickup->next;
	}
}
