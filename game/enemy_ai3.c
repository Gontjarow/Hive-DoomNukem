/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/13 15:16:38 by msuarez-          #+#    #+#             */
/*   Updated: 2021/06/13 15:26:39 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void			check_aggro(t_doom *doom, t_enemy *enemy)
{
	int dist;

	if (check_location(doom, doom->mdl->player.x, doom->mdl->player.y)
		== check_location(doom, enemy->x, enemy->y))
	{
		dist = calc_distance(enemy, doom);
		if (dist <= enemy->ai.max_dis)
			enemy->ai.aggro = ACTIVE;
	}
}

static float	fix_angle(float angle)
{
	if (angle < 0)
		angle += 360;
	if (angle > 360)
		angle -= 360;
	return (angle);
}

static void		assign_orient(t_enemy *enemy, float angle)
{
	if ((angle >= 0 && angle <= 45) || (angle <= 360 && angle >= 315))
		enemy->anim.orient = FRONT;
	else if (angle < 315 && angle >= 225)
		enemy->anim.orient = RIGHT;
	else if (angle > 45 && angle <= 135)
		enemy->anim.orient = LEFT;
	else
		enemy->anim.orient = BACK;
}

void			check_sprite_orient(t_doom *doom, t_enemy *enemy)
{
	int		off;
	t_xy	delta;
	float	angle;

	if (enemy->hp.cur > 0)
	{
		delta.y = doom->mdl->player.y - enemy->y;
		delta.x = doom->mdl->player.x - enemy->x;
		angle = atan2(delta.y, delta.x);
		if (angle < 0)
			angle = fabs(angle);
		else
			angle = 2 * M_PI - angle;
		angle *= (180 / M_PI);
		off = 180 - enemy->rot;
		if ((off > 0 && off <= 90) || (off > 90 && off <= 180))
			angle -= off;
		else if ((off > -180 && off <= -90) || (off > -90 && off < 0))
		{
			off = enemy->rot;
			angle -= 180 - off;
		}
		angle = fix_angle(angle);
		assign_orient(enemy, angle);
	}
}
