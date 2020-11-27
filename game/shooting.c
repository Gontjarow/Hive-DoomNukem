/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shooting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2020/11/27 16:29:03 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

unsigned int		check_location(t_doom *doom, int x, int y)
{
	int				location;
	unsigned int	*pixels;

	pixels = (unsigned int*)doom->mdl->poly_map->pixels;
	location = (int)x + (int)y * GAME_POLYMAP_WIDTH;
	if (location < 0 || location > ((GAME_POLYMAP_WIDTH * GAME_POLYMAP_HEIGHT) -1))
		return (UINT_ERROR_CONSTANT);
	return ((int)(pixels[location]));
	// printf("pro debug: %d\n", doom->mdl->poly_map[location]);
}

void		deal_damage(t_doom *doom, int enemy_id)
{
	t_enemy	*enemy;
	int	dx;
	int	dy;
	int	ec;
	int	distance;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		// printf("enemy %d hp: %d\n", enemy->id, enemy->hp.cur);
		if (enemy->id == enemy_id && enemy->hp.cur > 0)
		{
			enemy->hp.cur -= 10;
			// printf("ENEMY HIT!!! YAY DEALING DAMAGE!!! TO ENEMY: %d\n", enemy_id);
			if (enemy->hp.cur <= 0)
			{
				// printf("HE IS DEAD ;-;\n");
				enemy->hp.cur = 0;
			}
		}
		enemy = enemy->next;
	}
}

int			player_shoots(t_doom *doom)
{
	int		bullet_speed;
	int		enemy_who_was_hit;
	double	rad;

	rad = deg_to_rad(doom->mdl->player.rot);
	bullet_speed = 1;
	enemy_who_was_hit = -1;
	doom->mdl->player.bullet_pos_x = doom->mdl->player.x;
	doom->mdl->player.bullet_pos_y = doom->mdl->player.y;
	doom->minimap->debug_ray_color = 0xffff0000;
	doom->minimap->debug_ray_timeout = 15;
	while (check_location(doom, doom->mdl->player.bullet_pos_x, doom->mdl->player.bullet_pos_y) != -1 && enemy_who_was_hit < 0)
	{
		doom->mdl->player.bullet_pos_x += bullet_speed * -cos(rad);
		doom->mdl->player.bullet_pos_y += bullet_speed * -sin(rad);
		enemy_who_was_hit = check_hit(doom);
		//ft_putnbr(discriminant);
		//ft_putendl(" ");
		if (enemy_who_was_hit >= 0)
		{
			doom->minimap->debug_ray_color = 0xff00ff00;
			deal_damage(doom, enemy_who_was_hit);
		}
	}
	return (0);
}
