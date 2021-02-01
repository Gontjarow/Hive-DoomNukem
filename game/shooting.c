/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shooting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/27 19:46:06 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

unsigned int		check_location(t_doom *doom, int x, int y)
{
	int				location;
	unsigned int	*pixels;

	pixels = (unsigned int*)doom->mdl->poly_map->pixels;
	location = (int)x + (int)y * GAME_POLYMAP_WIDTH;
	if (location < 0 || location >
			((GAME_POLYMAP_WIDTH * GAME_POLYMAP_HEIGHT) - 1))
		return (UINT_ERROR_CONSTANT);
	return ((int)(pixels[location]));
}

void				deal_damage(t_doom *doom, int enemy_id)
{
	t_enemy	*enemy;
	int		dx;
	int		dy;
	int		ec;
	int		distance;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (enemy->id == enemy_id && enemy->hp.cur > 0)
		{
			if (enemy->stun_cd == 0)
				enemy->stun_time = 10;
			enemy->anim.done = HURT;
			enemy->ai.aggro = 1;
			enemy->hp.cur -=
			doom->mdl->player.weap_arr[doom->mdl->player.weap_id].dmg;
			if (enemy->hp.cur <= 0)
			{
				enemy->hp.cur = 0;
				enemy->anim.done = DEATH;
				Mix_PlayChannel(1, doom->sounds->mcEnemyDeath, 0);
			}
		}
		enemy = enemy->next;
	}
}

int					player_shoots(t_doom *doom)
{
	int		enemy_who_was_hit;
	double	rad;

	rad = deg_to_rad(doom->mdl->player.rot);
	rad = rad - M_PI;
	enemy_who_was_hit = -1;
	doom->mdl->player.bullet_pos.x = doom->mdl->player.x;
	doom->mdl->player.bullet_pos.y = doom->mdl->player.y;
	if (DEBUG == 1)
	{
		doom->minimap->player_ray_color = 0xffff0000;
		doom->minimap->player_ray_timeout = 15;
	}
	doom->mdl->player.shoot_cd = doom->mdl->player.weap_arr
								[doom->mdl->player.weap_id].cooldown;
	doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur--;
	while (check_location(doom, doom->mdl->player.bullet_pos.x,
			doom->mdl->player.bullet_pos.y) != -1 && enemy_who_was_hit < 0)
	{
		doom->mdl->player.bullet_pos.x += 1 * -cos(rad);
		doom->mdl->player.bullet_pos.y += 1 * -sin(rad);
		enemy_who_was_hit = check_hit(doom);
		if (enemy_who_was_hit >= 0)
		{
			if (DEBUG == 1)
				doom->minimap->player_ray_color = 0xff00ff00;
			deal_damage(doom, enemy_who_was_hit);
		}
	}
	return (0);
}
