/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 16:41:46 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/03 20:21:27 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void     rotate_left(t_doom *doom, t_enemy *enemy)
{
    int     x;
    int     y;
    double  rad;

    enemy->rot -= 5;
    if (enemy->rot < 0)
	    enemy->rot = 359;
    rad = deg_to_rad(enemy->rot);
    x = enemy->x + 10 * -cos(rad);
    y = enemy->y + 10 * -sin(rad);
    enemy->tail.x = x;
    enemy->tail.y = y;
}

static void     rotate_right(t_doom *doom, t_enemy *enemy)
{
    int     x;
    int     y;
    double  rad;

    enemy->rot += 5;
    if (enemy->rot >= 360)
		enemy->rot = 0;
    rad = deg_to_rad(enemy->rot);
    x = enemy->x + 10 * -cos(rad);
    y = enemy->y + 10 * -sin(rad);
    enemy->tail.x = x;
    enemy->tail.y = y;
}

static int      orientation(t_doom *doom, t_enemy *enemy, int x, int y) 
{
    int val; 
    
    val = (y - enemy->y) * (doom->mdl->player.x - x) - (x - enemy->x) * (doom->mdl->player.y - y); 
    if (val == 0)
        return (0);                 // colinear 
    return ((val > 0) ? 1 : 2);     // clock or counterclock wise 
}

void            rotate_enemy_towards_player(t_doom *doom)
{
	t_enemy	*enemy;
	int     ec;
    int     x;
    int     y;
    int     orient;
    double  rad;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
        if (enemy->hp.cur > 0)
        {
            rad = deg_to_rad(enemy->rot);
            x = enemy->x + 10 * -cos(rad);
            y = enemy->y + 10 * -sin(rad);
            orient = orientation(doom, enemy, x, y);
            if (orient == 1)                      // the shortest angle to face the player is turning left
            {
                // printf("Rotating Left!\n");
                rotate_left(doom, enemy);
            }
            else if (orient == 2)                 // the shortest angle to face the player is turning right
            {
                // printf("Rotating Right!\n");
                rotate_right(doom, enemy);
            }
        }
		enemy = enemy->next;
    }
}

void            move_enemy_towards_player(t_doom *doom)
{
    t_enemy *enemy;
    int     ec;
    int     x;
    int     y;
    int     old_x;
    int     old_y;
    double  distance;
    double  dist_x;
    double  dist_y;
    double  rad;

    ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
        old_x = enemy->x;
        old_y = enemy->y;
        if (check_location(doom, enemy->x, enemy->y) != -1 && enemy->hp.cur > 0)
        {
            dist_x = enemy->x - doom->mdl->player.x;
            dist_y = enemy->y - doom->mdl->player.y;
            distance = sqrt(dist_x * dist_x + dist_y * dist_y);
            // printf("DISTANCE BETWEEN ENEMY AND PLAYER: %f\n", distance);
            if (distance >= 70 && distance <= 200)
            {
                // printf("FOUND ENEMY, MOVING TO ENGAGE!\n");
                rad = deg_to_rad(enemy->rot);
                enemy->x += 10 * -cos(rad);
                enemy->y += 10 * -sin(rad);
                x = enemy->x + 10 * -cos(rad);
                y = enemy->y + 10 * -sin(rad);
                enemy->tail.x = x;
                enemy->tail.y = y;
                if (check_location(doom, enemy->x, enemy->y) == -1 || check_location(doom, enemy->x, enemy->y) == UINT_ERROR_CONSTANT)
                {
                    enemy->x = old_x;
                    enemy->y = old_y;
                }
            }
            if (distance < 70)
            {
                enemy_shoot_the_player(doom, enemy, distance);
            }
        }
        enemy = enemy->next;
    }
}

static int	check_hit_on_player(t_doom *doom, t_enemy *enemy)
{
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

	// printf("enemy hp: %d\nenemy id: %d\n", enemy->hp.cur, enemy->id);
	inside1 = point_circle(enemy->x, enemy->y, doom->mdl->player.x, doom->mdl->player.y);
	inside2 = point_circle(enemy->bullet_pos_x, enemy->bullet_pos_y, doom->mdl->player.x, doom->mdl->player.y);
	if ((inside1 || inside2)) 
		return (1);
	// get length of the line
	dist_x = enemy->x - enemy->bullet_pos_x;
	dist_y = enemy->y - enemy->bullet_pos_y;
	len = sqrt((dist_x * dist_x) + (dist_y * dist_y));
	// get dot product of the line and circle
	dot = ( ((doom->mdl->player.x - enemy->x) * (enemy->bullet_pos_x - enemy->x)) +
		((doom->mdl->player.y - enemy->y) * (enemy->bullet_pos_y - enemy->y)) ) / pow(len, 2);
	// find the closest point on the line
	closest_x = enemy->x + (dot * (enemy->bullet_pos_x - enemy->x));
	closest_y = enemy->y + (dot * (enemy->bullet_pos_y - enemy->y));
	// is this point actually on the line enemy->idsegment?
	// if so keep going, but if not, return false
	on_segment = line_point(enemy->x, enemy->y, enemy->bullet_pos_x,
							enemy->bullet_pos_y, closest_x, closest_y);
	if (!on_segment)
		return (0);
	// get distance to closest point
	dist_x = closest_x - doom->mdl->player.x;
	dist_y = closest_y - doom->mdl->player.y;
	distance = sqrt((dist_x * dist_x) + (dist_y * dist_y));
	if (distance <= 15)
    	return (1);
	return (0);
}

static void deal_damage_on_player(t_doom *doom)
{
    ft_putstr("Player HP: ");
    ft_putnbr(doom->mdl->player.hp.cur);
    ft_putendl(" ");
    if (doom->mdl->player.hp.cur > 0)
    {
        doom->mdl->player.hp.cur -= 5;
        if (doom->mdl->player.hp.cur <= 0)
        {
            printf("GAME OVER :(\n");
            doom->mdl->player.hp.cur = 0;
        }
    }
}

void        enemy_shoot_the_player(t_doom *doom, t_enemy *enemy, double distance)
{
    int         bullet_speed;
    int         did_hit;
    int         shoot_cooldown;
    double      rad;

    bullet_speed = 1;
    did_hit = 0;
    doom->minimap->enemy_ray_color = 0xffff0000;
    doom->minimap->enemy_ray_timeout = 15;
    enemy->bullet_pos_x = enemy->x;
    enemy->bullet_pos_y = enemy->y;
    while (check_location(doom, enemy->bullet_pos_x, enemy->bullet_pos_y) != -1 && did_hit == 0)
	{
        rad = deg_to_rad(enemy->rot);
		enemy->bullet_pos_x += bullet_speed * -cos(rad);
		enemy->bullet_pos_y += bullet_speed * -sin(rad);
		did_hit = check_hit_on_player(doom, enemy);
		if (did_hit == 1)
		{
			doom->minimap->enemy_ray_color = 0xff00ff00;
			deal_damage_on_player(doom);
		}
	}
}
