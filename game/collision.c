/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/08 18:55:57 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int		player_collision_with_enemies(t_doom *doom)
{
	t_enemy	*enemy;
	int	dx;
	int	dy;
	int	ec;
	int	distance;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return (0);
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		dx = (int)doom->mdl->player.x - enemy->x;
		dy = (int)doom->mdl->player.y - enemy->y;
		distance = sqrt(dx * dx + dy * dy);
		if ((distance < 10 + 10) && enemy->hp.cur > 0)
			return (-1);
		enemy = enemy->next;
	}
	return (0);
}

int		enemy_collision_with_enemies(t_doom *doom, t_enemy *enemy)
{
	t_enemy *next_enemy;
	int	dx;
	int	dy;
	int	ec2;
	int	distance;

	ec2 = doom->mdl->enemy_count;
	next_enemy = doom->mdl->enemy_first;
	if (ec2 > 1)
	{
		while (ec2 - 1)
		{
			if (next_enemy->id == enemy->id && next_enemy->next != NULL)
			{
				next_enemy = next_enemy->next;
			}
			dx = next_enemy->x - enemy->x;
			dy = next_enemy->y - enemy->y;
			distance = sqrt(dx * dx + dy * dy);
			if ((distance < 10 + 10) && next_enemy->hp.cur > 0)
				return (-1);
			ec2--;
			next_enemy = next_enemy->next;
		}
	}
	return (0);
}

int	point_circle(double px, double py, double cx, double cy)
{
	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	double	dist_x;
	double	dist_y;
	double	dist;

	dist_x = px - cx;
	dist_y = py - cy;
	dist = sqrt((dist_x * dist_x) + (dist_y * dist_y));
	// if the distance is less than the circle's
	// radius the point is inside!
	if (dist <= 10) 
    	return (1);
  return (0);
}

double	dist(double px, double py, double x1, double y1)
{
	double	distance;

	distance = sqrt(((px - x1) * (px - x1)) + ((py - y1) * (py - y1)));
	return (distance);
}

int	line_point(double x1, double y1, double x2, double y2, double px, double py)
{
	double	d1;
	double	d2;
	double	len;
	double	buffer;
	// get distance from the point to the two ends of the line
	d1 = dist(px, py, x1, y1);
	d2 = dist(px, py, x2, y2);

	// get the length of the line
	len = dist(x1, y1, x2, y2);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	buffer = 0.001;    // higher # = less accurate

	// if the two distances are equal to the line's
	// length, the point is on the line!
	// note we use the buffer here to give a range,
	// rather than one #
	if (d1 + d2 >= len - buffer && d1 + d2 <= len + buffer)
		return (1);
	return (0);
}

int		check_hit(t_doom *doom)
{
	int		ec;
	int		inside1;
	int		inside2;
	int		on_segment;
	double	dist_x;
	double	dist_y;
	double	len;
	double	dot;
	double	closest_x;
	double	closest_y;
	double	distance;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return (-1);
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		// printf("enemy hp: %d\nenemy id: %d\n", enemy->hp.cur, enemy->id);
		inside1 = point_circle(doom->mdl->player.x, doom->mdl->player.y, enemy->x, enemy->y);
		inside2 = point_circle(doom->mdl->player.bullet_pos_x, doom->mdl->player.bullet_pos_y, enemy->x, enemy->y);
		if ((inside1 || inside2) && enemy->hp.cur > 0) 
			return (enemy->id);
		// get length of the line
		dist_x = doom->mdl->player.x - doom->mdl->player.bullet_pos_x;
		dist_y = doom->mdl->player.y - doom->mdl->player.bullet_pos_y;
		len = sqrt((dist_x * dist_x) + (dist_y * dist_y));
		// get dot product of the line and circle
		dot = ( ((enemy->x - doom->mdl->player.x) * (doom->mdl->player.bullet_pos_x - doom->mdl->player.x)) +
			((enemy->y - doom->mdl->player.y) * (doom->mdl->player.bullet_pos_y - doom->mdl->player.y)) ) / pow(len,2);
		// find the closest point on the line
		closest_x = doom->mdl->player.x + (dot * (doom->mdl->player.bullet_pos_x - doom->mdl->player.x));
		closest_y = doom->mdl->player.y + (dot * (doom->mdl->player.bullet_pos_y - doom->mdl->player.y));
		// is this point actually on the line segment?
		// if so keep going, but if not, return false
		on_segment = line_point(doom->mdl->player.x, doom->mdl->player.y, doom->mdl->player.bullet_pos_x,
								doom->mdl->player.bullet_pos_y, closest_x, closest_y);
		if (!on_segment)
			return (-1);
		// get distance to closest point
		dist_x = closest_x - enemy->x;
		dist_y = closest_y - enemy->y;
		distance = sqrt((dist_x * dist_x) + (dist_y * dist_y));
		if (distance <= 10 && enemy->hp.cur > 0)
    		return (enemy->id);
		enemy = enemy->next;
	}
	return (-1);
}