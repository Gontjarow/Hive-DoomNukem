/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 16:41:46 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/15 17:27:33 by msuarez-         ###   ########.fr       */
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
        return (0);
    return ((val > 0) ? 1 : 2);
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
            if (orient == 1)
                rotate_left(doom, enemy);
            else if (orient == 2)
                rotate_right(doom, enemy);
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
                enemy->x += 5 * -cos(rad);
                enemy->y += 5 * -sin(rad);
                x = enemy->x + 5 * -cos(rad);
                y = enemy->y + 5 * -sin(rad);
                enemy->tail.x = x;
                enemy->tail.y = y;
                if (check_location(doom, enemy->x, enemy->y) == -1 || check_location(doom, enemy->x, enemy->y) == UINT_ERROR_CONSTANT || enemy_collision_with_enemies(doom, enemy) == -1)
                {
                    enemy->x = old_x;
                    enemy->y = old_y;
                }
            }
            if (distance < 70 && enemy->shoot_cd == 0)
            {
                enemy_shoot_the_player(doom, enemy);
            }
        }
        enemy = enemy->next;
    }
}

static void deal_damage_on_player(t_doom *doom)
{
    if (doom->mdl->player.hp.cur > 0)
    {
        doom->mdl->player.hp.cur -= 5;
        if (doom->mdl->player.hp.cur <= 0)
            doom->mdl->player.hp.cur = 0;
    }
}

void        enemy_update_cooldown(t_doom *doom)
{
    int ec;
    t_enemy *enemy;

    ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
        if (enemy->shoot_cd > 0)
            enemy->shoot_cd--;
        enemy = enemy->next;
    }
}

void        enemy_shoot_the_player(t_doom *doom, t_enemy *enemy)
{
    int         bullet_speed;
    int         did_hit;
    double      rad;

    bullet_speed = 1;
    did_hit = 0;
    enemy->ray_color = 0xffff0000;
    doom->minimap->enemy_ray_timeout = 15;
    enemy->shoot_cd = enemy->wep.cooldown;
    enemy->bullet_pos.x = enemy->x;
    enemy->bullet_pos.y = enemy->y;
    while (check_location(doom, enemy->bullet_pos.x, enemy->bullet_pos.y) != -1 && did_hit == 0)
	{
        rad = deg_to_rad(enemy->rot);
		enemy->bullet_pos.x += bullet_speed * -cos(rad);
		enemy->bullet_pos.y += bullet_speed * -sin(rad);
		did_hit = check_hit_on_player(doom, enemy);
		if (did_hit == 1)
		{
            enemy->ray_color = 0xff00ff00;
			// deal_damage_on_player(doom);     // disabled until we actually need it (it's working already)
		}
	}
    enemy->did_shoot = 1;
    enemy->who_shot = enemy->id;
}
