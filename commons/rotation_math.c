/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 15:19:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 16:24:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int				check_sprite_facing(t_enemy *enemy, t_model *mdl)
{
	int		collides[4];
	t_point	epts[4];
	t_point	player_pt;
	t_point enemy_pt;
	int 	body;

	player_pt.x = (int)mdl->player.x;
	player_pt.y = (int)mdl->player.y;
	enemy_pt.x = enemy->x;
	enemy_pt.y = enemy->y;
	body = 15;

	epts[FRONTLEFT].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot + 45)));
	epts[FRONTLEFT].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot + 45)));
	epts[FRONTRIGHT].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot - 45)));
	epts[FRONTRIGHT].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot - 45)));
	epts[BACKLEFT].x = enemy->x + (int)(body * sin(deg_to_rad(enemy->rot - 45)));
	epts[BACKLEFT].y = enemy->y + (int)(body * sin(deg_to_rad(enemy->rot - 45)));
	epts[BACKRIGHT].x = enemy->x + (int)(body * sin(deg_to_rad(enemy->rot + 45)));
	epts[BACKRIGHT].y = enemy->y + (int)(body * sin(deg_to_rad(enemy->rot + 45)));
	collides[FRONT] = do_intersect(epts[FRONTLEFT], epts[FRONTRIGHT], player_pt, enemy_pt);
	if (!collides[FRONT])
		collides[LEFT] = do_intersect(epts[FRONTLEFT], epts[BACKLEFT], player_pt, enemy_pt);
	else
		return (FRONT);
	if (!collides[FRONT] && !collides[LEFT])
		collides[BACK] = do_intersect(epts[BACKLEFT], epts[BACKRIGHT], player_pt, enemy_pt);
	else
		return (LEFT);
	if (!collides[FRONT] && !collides[LEFT] && !collides[BACK])
		collides[RIGHT] = do_intersect(epts[FRONTRIGHT], epts[BACKRIGHT], player_pt, enemy_pt);
	else
		return (BACK);
	if (collides[RIGHT])
		return (RIGHT);
	ft_putendl("Warning: Nothing collides as facing for enemy sprite.");
	return (-1);
}

double			angle_abc(t_point a, t_point b, t_point c)
{
	t_point		ab;
	t_point		cb;
	double		dot_product;
	double		cross_product;

	ab.x = b.x - a.x;
	ab.y = b.y - a.y;
	cb.x = b.x - c.x;
	cb.y = b.y - c.y;
	dot_product = ((double)(ab.x * cb.x) + (double)(ab.y * cb.y));
	cross_product = ((double)(ab.x * cb.y) - (double)(ab.y * cb.x));
	return (atan2(cross_product, dot_product));
}

int 			tail_degree_rot(t_point location, t_point *tail)
{
	double		result;
	int			x;
	int 		y;

	x = tail->x - location.x;
	y = tail->y - location.y;
	result = atan2(y, x) * 180.0 / M_PI;;
	result += 180.0;
	return ((int)result);
}
