/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 16:41:46 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/15 19:48:21 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	deal_damage_on_player(t_doom *doom)
{
	if (doom->mdl->player.hp.cur > 0)
	{
		doom->mdl->player.hp.cur -= 5;
		if (doom->mdl->player.hp.cur <= 0)
			doom->mdl->player.hp.cur = 0;
	}
}

void		enemy_update_cooldown(t_doom *doom)
{
	int		ec;
	t_enemy	*enemy;

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

void		enemy_shoot_the_player(t_doom *doom, t_enemy *enemy)
{
	int		bullet_speed;
	int		did_hit;

	bullet_speed = 1;
	did_hit = 0;
	enemy->ray_color = 0xffff0000;
	doom->minimap->enemy_ray_timeout = 15;
	enemy->shoot_cd = enemy->wep.cooldown;
	enemy->bullet_pos.x = enemy->x;
	enemy->bullet_pos.y = enemy->y;
	while (check_location(doom, enemy->bullet_pos.x,
		enemy->bullet_pos.y) != -1 && did_hit == 0)
	{
		enemy->bullet_pos.x += bullet_speed * -cos(deg_to_rad(enemy->rot));
		enemy->bullet_pos.y += bullet_speed * -sin(deg_to_rad(enemy->rot));
		did_hit = check_hit_on_player(doom, enemy);
		if (did_hit == 1)
		{
			enemy->ray_color = 0xff00ff00;
			deal_damage_on_player(doom);
		}
	}
	enemy->did_shoot = 1;
	enemy->who_shot = enemy->id;
}
