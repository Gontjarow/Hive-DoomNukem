/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 16:41:46 by msuarez-          #+#    #+#             */
/*   Updated: 2020/11/27 19:07:37 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void     rotate_left(t_doom *doom, t_enemy *enemy)
{
    int     x;
    int     y;
    double  rad;

    enemy->rot -= 10;
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

    enemy->rot += 10;
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
        if (check_location(doom, doom->mdl->player.x, doom->mdl->player.y) == check_location(doom, enemy->x, enemy->y) && enemy->hp.cur > 0)
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