/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shooting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2021/07/04 15:09:31 by msuarez-         ###   ########.fr       */
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

void				update_enemy_status(t_doom *doom, t_enemy *enemy, int id)
{
	if (enemy->id == id && enemy->hp.cur > 0)
	{
		if (enemy->stun_cd <= 0)
		{
			enemy->stun_time = 0.3;
			enemy->stun_cd = 1.5 + enemy->stun_time;
			enemy->anim.done = HURT;
		}
		enemy->ai.aggro = ACTIVE;
		enemy->hp.cur -=
		doom->mdl->player.weap_arr[doom->mdl->player.weap_id].dmg;
		if (enemy->hp.cur <= 0)
		{
			enemy->hp.cur = 0;
			enemy->anim.done = DEATH;
			Mix_PlayChannel(-1, enemy->ai.mcEnemyDeath, 0);
		}
	}
}

static void       aggro_enemies(t_doom *doom, t_enemy *enemy_hit)
{
   int		ec;
   t_enemy	*enemy;

	enemy = doom->mdl->enemy_first;
	ec = doom->mdl->enemy_count;
	while (ec--)
	{
    	if (enemy->ai.aggro == 0 && check_location(doom, enemy->x,
			enemy->y) == check_location(doom, enemy_hit->x, enemy_hit->y))
    		enemy->ai.aggro = 1;
    	enemy = enemy->next;
	}
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
		update_enemy_status(doom, enemy, enemy_id);
		if (enemy->id == enemy_id)
			aggro_enemies(doom, enemy);
		enemy = enemy->next;
	}
}

int					player_shoots_shotgun(t_doom *doom)
{
	doom->mdl->player.rot -= 10;
	player_shoots(doom);
	doom->mdl->player.rot += 10;
	player_shoots(doom);
	doom->mdl->player.rot += 10;
	player_shoots(doom);
	doom->mdl->player.rot -= 10;
	doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur += 2;
}

static void			check_poster_hit(t_doom *doom)
{
	int		ec;
	int		dx;
	int		dy;
	int		distance;
	t_effect *effector;

	ec = doom->mdl->effect_count;
	if (ec == 0)
		return ;
	effector = doom->mdl->effect_first;
	while (ec--)
	{
		if (effector->type_id == EFFECT_POSTER && effector->activated == 0)
		{
			dx = (int)doom->mdl->player.bullet_pos.x - effector->loc.x;
			dy = (int)doom->mdl->player.bullet_pos.y - effector->loc.y;
			distance = sqrt(dx * dx + dy * dy);
			if ((distance < 10 + PICKING_RADIUS))
			{
				effector->activated = 1;
				effector->active_sprite = doom->sprites->txt_poster_off;
				return ;
			}
		}
		effector = effector->next;
	}
}

int					player_shoots(t_doom *doom)
{
	int		enemy_id;
	double	rad;

	rad = deg_to_rad(doom->mdl->player.rot) - M_PI;
	enemy_id = -1;
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
			doom->mdl->player.bullet_pos.y) != -1 && enemy_id < 0)
	{
		doom->mdl->player.bullet_pos.x += 1 * -cos(rad);
		doom->mdl->player.bullet_pos.y += 1 * -sin(rad);
		check_poster_hit(doom);
		enemy_id = check_hit(doom);
		if (enemy_id >= 0)
		{
			if (DEBUG == 1)
				doom->minimap->player_ray_color = 0xff00ff00;
			deal_damage(doom, enemy_id);
		}
	}
	return (0);
}

static void			play_shoot_sounds(t_doom *doom)
{
	static int mix_i = 0;

	Mix_PlayChannel(mix_i, doom->mdl->player.weap_arr[doom->mdl->player.weap_id].fire_sound, 0);
	mix_i++;
	if (mix_i > 6)
		mix_i = 0;
}

void				handle_player_shooting(t_doom *doom)
{
	if (doom->mdl->player.shooting && !doom->mdl->player.is_running)
	{
		if (doom->mdl->player.shoot_cd <= 0 && doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur > 0 && doom->mdl->player.reload_time <= 0)
		{
			if (doom->mdl->player.weap_id == PISTOL && !doom->mdl->player.has_fired)
			{
				player_shoots(doom);
				play_shoot_sounds(doom);
				doom->mdl->player.has_fired = 1;
			}
			else if (doom->mdl->player.weap_id == SHOTGUN && !doom->mdl->player.has_fired)
			{
				player_shoots_shotgun(doom);
				play_shoot_sounds(doom);
				doom->mdl->player.has_fired = 1;
			}
			else if (doom->mdl->player.weap_id == ASSAULT_RIFLE)
			{
				player_shoots(doom);
				play_shoot_sounds(doom);
			}
		}
		else if (doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur == 0)
		{
			Mix_PlayChannel(-1, doom->sounds->mcOutAmmo, 0);
		}
	}
}
