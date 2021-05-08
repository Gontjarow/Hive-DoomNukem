/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <niko.gontjarow@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2021/05/08 20:55:50 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"
#include "minimap.h"

void		print_player_ray(t_doom *doom)
{
	t_line	line;

	line.x1 = doom->mdl->player.x * doom->minimap->scale;
	line.y1 = doom->mdl->player.y * doom->minimap->scale;
	line.x2 = doom->mdl->player.bullet_pos.x * doom->minimap->scale;
	line.y2 = doom->mdl->player.bullet_pos.y * doom->minimap->scale;
	line.color = doom->minimap->player_ray_color;
	line.buff = doom->minimap->buff;
	render_line(&line);
	doom->minimap->player_ray_timeout--;
}

void		print_enemy_ray(t_doom *doom)
{
	t_line	line;
	t_enemy *enemy;
	int		ec;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (enemy->did_shoot == 1 && enemy->who_shot == enemy->id
			&& enemy->hp.cur > 0)
		{
			line.x1 = enemy->x * doom->minimap->scale;
			line.y1 = enemy->y * doom->minimap->scale;
			line.x2 = enemy->bullet_pos.x * doom->minimap->scale;
			line.y2 = enemy->bullet_pos.y * doom->minimap->scale;
			line.color = enemy->ray_color;
			line.buff = doom->minimap->buff;
			render_line(&line);
			enemy->did_shoot = 0;
		}
		doom->minimap->enemy_ray_timeout--;
		enemy = enemy->next;
	}
}

void		destroy_minimap(t_doom *doom)
{
	SDL_FreeSurface(doom->minimap->buff);
	SDL_DestroyWindow(doom->minimap->win);
	doom->minimap->win = NULL;
	doom->minimap->buff = NULL;
	free(doom->minimap);
	doom->minimap = NULL;
}

static void	print_uv_grid(t_doom *doom, int w, int h, int c)
{
	int x = 0;
	while (x < doom->minimap->buff->w)
	{
		t_xy_line l = line(x, 0, x, MWIN_HEIGHT);
		l.color = c;
		drawline(l, doom->minimap->buff);
		x += w;
	}
	int y = 0;
	while (y < doom->minimap->buff->w)
	{
		t_xy_line l = line(0, y, MWIN_WIDTH, y);
		l.color = c;
		drawline(l, doom->minimap->buff);
		y += h;
	}
}

void		update_minimap(t_doom *doom)
{
	flood_buffer(doom->minimap->buff, 0xff000000);
	print_uv_grid(doom, 64, 64, 0x303030);
	print_uv_grid(doom, 512, 512, 0x606060);
	if (doom->minimap->player_ray_timeout > 0
		&& doom->mdl->player.shoot_cd != 0)
		print_player_ray(doom);
	if (doom->minimap->enemy_ray_timeout > 0)
		print_enemy_ray(doom);
	print_minimap_walls(doom);
	print_minimap_player(doom);
	print_minimap_enemies(doom);
	print_minimap_pickups(doom);
	SDL_UpdateWindowSurface(doom->minimap->win);
}

void		init_minimap(t_doom *doom)
{
	int			win_x;
	int			win_y;

	if (!(doom->minimap = (t_minimap *)malloc(sizeof(t_minimap))))
		return ;
	doom->minimap->win = SDL_CreateWindow("Debug Minimap",
		SDL_WINDOWPOS_CENTERED, 0,
			MWIN_WIDTH, MWIN_HEIGHT, 0);
	if (doom->minimap->win == NULL)
		ft_die("Fatal error: Failed init of SDL_Window from minimap");
	doom->minimap->buff = SDL_GetWindowSurface(doom->minimap->win);
	if (doom->minimap->buff == NULL)
		ft_die("Fatal error: Failed init of SDL_Surface from minimap");
	doom->minimap->scale = 1.0 / 10;
	doom->minimap->sprite_num = 0;
	SDL_GetWindowPosition(doom->win, &win_x, &win_y);
	// SDL_SetWindowPosition(doom->minimap->win, (1920 / 2) - GAME_MIDWIDTH, win_y - 400);
	SDL_SetWindowPosition(doom->game->win, (1920 / 2) - GAME_MIDWIDTH, win_y + 128);
	// SDL_RaiseWindow(doom->game->win);
	update_minimap(doom);
}
