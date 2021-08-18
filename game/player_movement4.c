/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 20:10:40 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 20:12:43 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	update_player_tail(t_doom *doom, double rad)
{
	t_point	p;

	p.x = doom->mdl->player.x + 10 * -cos(rad);
	p.y = doom->mdl->player.y + 10 * -sin(rad);
	doom->mdl->player.tail.x = p.x;
	doom->mdl->player.tail.y = p.y;
}

void	init_player_z(t_doom *doom)
{
	int	room_id;

	room_id = room_id_from_polymap(doom->mdl->poly_map, doom->mdl->player.x,
			doom->mdl->player.y);
	doom->mdl->player.z = room_by_id(room_id)->floor_height
		+ doom->mdl->player.height;
}
