/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 19:07:12 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/03 16:32:38 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		validate_player_position(t_doom *doom, t_coord old)
{
	int		location_id;

	location_id = check_location(doom, doom->mdl->player.x,
					doom->mdl->player.y);
	if (location_id == -1 || location_id == UINT_ERROR_CONSTANT ||
		player_collision_with_enemies(doom) == -1)
	{
		doom->mdl->player.x = old.x;
		doom->mdl->player.y = old.y;
	}
}
