/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shooting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2021/07/04 19:37:54 by msuarez-         ###   ########.fr       */
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
			if ((distance < 22))
			{
				effector->activated = 1;
				effector->active_sprite = doom->sprites->txt_poster_off;
				return ;
			}
		}
		effector = effector->next;
	}
}

static int 			check_intersect(t_xy current_loc, t_point start, t_point end)
{
	double	slope;
	int		pt3_on;
	int		pt3_between;

	pt3_on = 0;
	pt3_between = 0;
	slope = (end.y - start.y) / (end.x - start.x);
	if ((current_loc.y - start.y) == (slope * (current_loc.x - start.x)))
		pt3_on = 1;
	if ((min(start.x, end.x) <= current_loc.x && current_loc.x <= max(start.x, end.x)) && (min(start.y, end.y) <= current_loc.y && current_loc.y <= max(start.y, end.y)))
		pt3_between = 1;
	if (pt3_on && pt3_between)
		return (1);
	return (0);
}

static int			check_bullet_hitting_door(t_doom *doom, t_xy prev_loc, t_xy current_loc)
{
	// algorithm to loop through the mdl->portal(s?) linked list and check for intersections.
	//this is not run on every frame, just the frames when bullet room_id has changed
	// return (1) if intersecting closed door

	//if its intersecting, check its actual status, if its status is 0, 1, 2
	// 0 = REGULAR_PORTAL
	// 1 = DOOR_PORTAL
	// 2 = WINDOW_PORTAL
	//if 0, let it gooooooo
	//if 1, check the door status, if closed, stop it, if open, let it goooooo
	//if 2, stop it
	int		pc;
	t_wall	*portals;

	pc = doom->mdl->portal_count;
	if (pc == 0)
		return (0);
	portals = doom->mdl->portal_first;
	while (pc--)
	{
		// printf("Found a portal!\n");
		if ()
		{
			// check whats the status of this portal!
			printf("ITS INTERSECTING!\n");
			return (1);
		}
		// printf("Next portal!\n");
		portals = portals->next;
	}
	return (0);
}

int					player_shoots(t_doom *doom)
{
	int		enemy_id;
	double	rad;
	int		last_room_id;
	int		current_room_id;
	t_xy	cached_bullet_loc;

	rad = deg_to_rad(doom->mdl->player.rot) - M_PI;
	enemy_id = -1;
	doom->mdl->player.bullet_pos.x = doom->mdl->player.x;
	doom->mdl->player.bullet_pos.y = doom->mdl->player.y;
	last_room_id = room_id_from_polymap(doom->mdl->poly_map, doom->mdl->player.x, doom->mdl->player.y);

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
		cached_bullet_loc.x = doom->mdl->player.bullet_pos.x;
		cached_bullet_loc.y = doom->mdl->player.bullet_pos.y;
		doom->mdl->player.bullet_pos.x += 1 * -cos(rad);
		doom->mdl->player.bullet_pos.y += 1 * -sin(rad);
		current_room_id = room_id_from_polymap(doom->mdl->poly_map, doom->mdl->player.bullet_pos.x, doom->mdl->player.bullet_pos.y);
		if (current_room_id != last_room_id)
		{
			// Different room_id for bullet, reroute to algorithm for checking hits against door
			if (check_bullet_hitting_door(doom, cached_bullet_loc, (t_xy){doom->mdl->player.bullet_pos.x, doom->mdl->player.bullet_pos.y}))
			{
				break ;
			}
		}
		last_room_id = current_room_id;
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
