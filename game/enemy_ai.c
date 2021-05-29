/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 16:41:46 by msuarez-          #+#    #+#             */
/*   Updated: 2021/05/29 18:33:19 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	deal_damage_on_player(t_doom *doom, t_enemy *enemy)
{
	if (doom->mdl->player.hp.cur > 0)
	{
		doom->mdl->player.hp.cur -= enemy->ai.dmg;
		if (doom->mdl->player.hp.cur <= 0)
			doom->mdl->player.hp.cur = 0;
	}
}

void		enemy_update_cooldown(t_doom *doom, t_enemy *enemy)
{
	if (enemy->shoot_cd > 0)
		enemy->shoot_cd -= 1 * doom->delta_time;
	if (enemy->stun_cd > 0)
		enemy->stun_cd -= 1 * doom->delta_time;
	if (enemy->stun_time > 0)
		enemy->stun_time -= 1 * doom->delta_time;
}

void		enemy_shoot_the_player(t_doom *doom, t_enemy *enemy)
{
	int		did_hit;

	did_hit = 0;
	enemy->ray_color = 0xffff0000;
	if (DEBUG == 1)
		doom->minimap->enemy_ray_timeout = 15;
	enemy->shoot_cd = 1;
	enemy->bullet_pos.x = enemy->x;
	enemy->bullet_pos.y = enemy->y;
	while (check_location(doom, enemy->bullet_pos.x,
		enemy->bullet_pos.y) != -1 && did_hit == 0)
	{
		enemy->bullet_pos.x += 1 * -cos(deg_to_rad(enemy->rot));
		enemy->bullet_pos.y += 1 * -sin(deg_to_rad(enemy->rot));
		did_hit = check_hit_on_player(doom, enemy);
		if (did_hit == 1)
		{
			enemy->ray_color = 0xff00ff00;
			deal_damage_on_player(doom, enemy);
		}
	}
	enemy->anim.done = ATTACK;
	enemy->did_shoot = 1;
	enemy->who_shot = enemy->id;
	Mix_PlayChannel(-1, doom->sounds->mcPistolShot, 0);
}

void		handle_enemy_shooting(t_doom *doom, t_enemy *enemy)
{
	int		distance;

	if (enemy->hp.cur > 0 && enemy->stun_time <= 0)
	{
		distance = calc_distance(enemy, doom);
		if (distance < enemy->ai.min_dis && enemy->shoot_cd <= 0)
			enemy_shoot_the_player(doom, enemy);
	}
}
