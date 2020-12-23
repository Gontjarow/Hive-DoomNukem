/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_pickup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:20:47 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/23 04:46:30 by msuarez-         ###   ########.fr       */
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

// STILL WORK IN PROGRESS -> HANDLE DELETION OF A CERTAIN PICKUP ON THE LINKED LIST!!!

t_pickup		*delete_first(t_doom *doom)
{
	t_pickup	*head;
	t_pickup	*temp;

	head = doom->mdl->pickup_first;
	if (head == NULL)
		return (NULL);
	// Move the head pointer to the next node
	temp = head;
	head = head->next;
    free(temp);
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

// DELETION OF THE HEALTH PICKUPS: YOU CAN PICKUP ALL OF THE NODES AFTER THE HEAD
// AND IF YOU GET THE HEAD AS LAST, IT WORKS PERFECTLY. BUT IF YOU PICKUP THE
// HEAD FIRST OR IF YOU PICKUP SOMETHING FIRST AND THEN THE HEAD
// IT SEGFAULTS! FIX IT! :)

static void		handle_health_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	printf("Pickup ID that u touched: %d\n", pickup->id);
	pc = doom->mdl->pickup_count;
	current = doom->mdl->pickup_first;
	printf("before if\n");
	if (current->id == pickup->id)
	{
		printf("Inside if\n");
		pickup = delete_first(doom);
		printf("after delete_first\n");
	}
	else
	{
		printf("Inside else\n");
		pickup = delete_node(current, prev, pickup);
		printf("after delete_node\n");
	}
	Mix_PlayChannel(3, doom->sounds->mcHealthPickup, 0);
	doom->mdl->pickup_count--;
	doom->mdl->player.hp.cur += 1;
	if (doom->mdl->player.hp.cur > 100)
		doom->mdl->player.hp.cur = 100;
	printf("you touched a pickup!\n");
}

// void		handle_ammo_pickup(t_doom *doom, t_pickup *pickup)
// {
	
// }

// void		handle_weapon_pickup(t_doom *doom, t_pickup *pickup)
// {
	
// }

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
		// THIS PART IS COMMENTED OUT BECAUSE HANDLE_HEALTH_PICKUP IS INCOMPLETE!!! by: msuarez-
		printf("Checking all pickups\n");
		if (pickup->flavor == PICKUP_HEALTH && player_collision_with_pickup
			(doom, pickup) == -1 && doom->mdl->player.hp.cur < 100)
		{
			handle_health_pickup(doom, pickup);
		}

		// if (pickup->flavor == PICKUP_AMMO)
		// 	handle_ammo_pickup(doom, pickup);
		// if (pickup->flavor == PICKUP_WEAPON)
		// 	handle_weapon_pickup(doom, pickup);
		pickup = pickup->next;
	}
	printf("\n\n");
}
