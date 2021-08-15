/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 15:19:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/08/15 20:40:26 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static int		closer_to_player_compare(int a, int b, t_point pta, t_point ptb)
{
	t_model *mdl;
	t_point	ptp;
	int 	dista;
	int 	distb;

	mdl = get_model();
	ptp.x = (int)mdl->player.x;
	ptp.y = (int)mdl->player.y;
	dista = abs(ptp.x - pta.x) + abs(ptp.y - pta.y);
	distb = abs(ptp.x - ptb.x) + abs(ptp.y - ptb.y);
	if (dista > distb)
		return (b);
	return (a);
}

// static int		closer_to_player(int a, int b, t_enemy *enemy, t_model *mdl)
// {
// 	t_point	epts[4];
// 	t_point enemy_pt;
// 	int 	body;
// 	t_xy	box;


// 	enemy_pt.x = enemy->x;
// 	enemy_pt.y = enemy->y;
// 	body = 15;
// 	if (a == FRONT && b == BACK)
// 	{
// 		epts[FRONT].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot)));
// 		epts[FRONT].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot)));
// 		epts[BACK].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot + 180)));
// 		epts[BACK].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot + 180)));
// 		box.x = epts[FRONT].x * mdl->parent->minimap->scale; box.y = epts[FRONT].y * mdl->parent->minimap->scale;
// 		draw_box(box, 2, 0xff00ff, mdl->parent->minimap->buff);
// 		box.x = epts[BACK].x * mdl->parent->minimap->scale; box.y = epts[BACK].y * mdl->parent->minimap->scale;
// 		draw_box(box, 2, 0xffff00, mdl->parent->minimap->buff);
// 		return (closer_to_player_compare(FRONT, BACK, epts[FRONT], epts[BACK]));
// 	}
// 	if (a == RIGHT && b == LEFT)
// 	{
// 		epts[LEFT].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot - 90)));
// 		epts[LEFT].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot - 90)));
// 		epts[RIGHT].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot + 90)));
// 		epts[RIGHT].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot + 90)));
// 		box.x = epts[RIGHT].x * mdl->parent->minimap->scale; box.y = epts[RIGHT].y * mdl->parent->minimap->scale;
// 		draw_box(box, 2, 0x00ffff, mdl->parent->minimap->buff);
// 		box.x = epts[LEFT].x * mdl->parent->minimap->scale; box.y = epts[LEFT].y * mdl->parent->minimap->scale;
// 		draw_box(box, 2, 0x00ff00, mdl->parent->minimap->buff);
// 		return (closer_to_player_compare(RIGHT, LEFT, epts[FRONT], epts[BACK]));
// 	}
// 	ft_putendl("Warning: Closer_to_player failure, defaulting FRONT!");
// 	return (FRONT);
// }

// int				check_sprite_facing(t_enemy *enemy, t_model *mdl)
// {
// 	int		collides[4];
// 	t_point	epts[4];
// 	t_point	player_pt;
// 	t_point enemy_pt;
// 	t_xy	box;
// 	int 	body;

// 	player_pt.x = (int)mdl->player.x;
// 	player_pt.y = (int)mdl->player.y;
// 	enemy_pt.x = enemy->x;
// 	enemy_pt.y = enemy->y;
// 	body = 15;
// 	epts[FRONTRIGHT].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot + 45)));
// 	epts[FRONTRIGHT].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot + 45)));
// 	epts[FRONTLEFT].x = enemy->x - (int)(body * cos(deg_to_rad(enemy->rot - 45)));
// 	epts[FRONTLEFT].y = enemy->y - (int)(body * sin(deg_to_rad(enemy->rot - 45)));
// 	epts[BACKLEFT].x = enemy->x + (int)(body * cos(deg_to_rad(enemy->rot - 45)));
// 	epts[BACKLEFT].y = enemy->y + (int)(body * sin(deg_to_rad(enemy->rot - 45)));
// 	epts[BACKRIGHT].x = enemy->x + (int)(body * cos(deg_to_rad(enemy->rot + 45)));
// 	epts[BACKRIGHT].y = enemy->y + (int)(body * sin(deg_to_rad(enemy->rot + 45)));
// 	box.x = epts[FRONTLEFT].x * mdl->parent->minimap->scale; box.y = epts[FRONTLEFT].y * mdl->parent->minimap->scale;
// 	draw_box(box, 2, 0xff00ff, mdl->parent->minimap->buff);
// 	box.x = epts[FRONTRIGHT].x * mdl->parent->minimap->scale; box.y = epts[FRONTRIGHT].y * mdl->parent->minimap->scale;
// 	draw_box(box, 2, 0x00ffff, mdl->parent->minimap->buff);
// 	box.x = epts[BACKLEFT].x * mdl->parent->minimap->scale; box.y = epts[BACKLEFT].y * mdl->parent->minimap->scale;
// 	draw_box(box, 2, 0xffff00, mdl->parent->minimap->buff);
// 	box.x = epts[BACKRIGHT].x * mdl->parent->minimap->scale; box.y = epts[BACKRIGHT].y * mdl->parent->minimap->scale;
// 	draw_box(box, 2, 0x00ff00, mdl->parent->minimap->buff);
// 	t_xy_line line;
// 	line.color = 0xffffff;
// 	line.start = (t_xy){mdl->player.x * mdl->parent->minimap->scale, mdl->player.y * mdl->parent->minimap->scale};
// 	line.stop = (t_xy){enemy->x * mdl->parent->minimap->scale, enemy->y * mdl->parent->minimap->scale};
// 	drawline(line, mdl->parent->minimap->buff);
// 	line.start = (t_xy){epts[BACKLEFT].x * mdl->parent->minimap->scale, epts[BACKLEFT].y * mdl->parent->minimap->scale};
// 	line.stop = (t_xy){epts[BACKRIGHT].x * mdl->parent->minimap->scale, epts[BACKRIGHT].y * mdl->parent->minimap->scale};
// 	drawline(line, mdl->parent->minimap->buff);
// 	closer_to_player(FRONT, BACK, enemy, mdl);
// 	closer_to_player(RIGHT, LEFT, enemy, mdl);
// 	SDL_UpdateWindowSurface(mdl->parent->minimap->win);
// 	SDL_Delay(33);
// 	collides[FRONT] = do_intersect(epts[FRONTLEFT], epts[FRONTRIGHT], player_pt, enemy_pt);
// 	collides[BACK] = do_intersect(epts[BACKLEFT], epts[BACKRIGHT], player_pt, enemy_pt);
// 	collides[RIGHT] = do_intersect(epts[FRONTRIGHT], epts[BACKRIGHT], player_pt, enemy_pt);
// 	collides[LEFT] = do_intersect(epts[FRONTLEFT], epts[BACKLEFT], player_pt, enemy_pt);
// 	if (collides[FRONT] && collides[BACK])
// 		return (closer_to_player(FRONT, BACK, enemy, mdl));
// 	if (collides[RIGHT] && collides[LEFT])
// 		return (closer_to_player(RIGHT, LEFT, enemy, mdl));
// 	if (collides[FRONT])
// 		return (FRONT);
// 	else if (collides[BACK])
// 		return (BACK);
// 	else if (collides[LEFT])
// 		return (LEFT);
// 	else if (collides[RIGHT])
// 		return (RIGHT);
// 	ft_putendl("Warning: Nothing collides as facing for enemy sprite.");
// 	return (-1);
// }

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
