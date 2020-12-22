/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_pickup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:20:47 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/22 20:53:18 by msuarez-         ###   ########.fr       */
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

static t_pickup	*delete_head(t_doom *doom, t_pickup *current)
{
	current = current->next;
	return (current);
}

static int		delete_tail(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;

	return (1);
}

// STILL WORK IN PROGRESS -> HANDLE DELETION OF A CERTAIN PICKUP ON THE LINKED LIST!!!

static void		handle_health_pickup(t_doom *doom, t_pickup *pickup)
{
	t_pickup	*current;
	t_pickup	*prev;
	int			pc;

	printf("Pickup ID that u touched: %d\n", pickup->id);
	current = doom->mdl->pickup_first;
	if (pickup->id == 0)
	{
		current = current->next;
		pickup = current;
		return ;
	}
	else if (pickup->id == doom->mdl->pickup_count - 1)
	{

	}
	pc = doom->mdl->pickup_count;
    while (pc--)
    {
		if (pickup->id == 0)
		{															//HEAD
			pickup = delete_head(doom, current);
			break;
		}
		else if (current->id == doom->mdl->pickup_count - 1)		//TAIL
			pickup = delete_tail(doom, current);
		if (current->id == pickup->id)
			break ;
		prev = current;
		current = current->next;
    }
    prev->next = current->next;
	pickup = prev;
	Mix_PlayChannel(3, doom->sounds->mcHealthPickup, 0);
	doom->mdl->pickup_count--;
	doom->mdl->player.hp.cur += 50;
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
	
		// if (pickup->flavor == PICKUP_HEALTH && player_collision_with_pickup
		// 	(doom, pickup) == -1 && doom->mdl->player.hp.cur < 100)
		// 	handle_health_pickup(doom, pickup);

		// if (pickup->flavor == PICKUP_AMMO)
		// 	handle_ammo_pickup(doom, pickup);
		// if (pickup->flavor == PICKUP_WEAPON)
		// 	handle_weapon_pickup(doom, pickup);
		pickup = pickup->next;
	}
}
