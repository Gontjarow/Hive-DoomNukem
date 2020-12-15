/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/15 16:38:30 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"
#include "minimap.h"

static void	rescale_minimap(t_wall *wall, t_doom *doom)
{
	while (wall->start.x * doom->minimap->scale > MINIMAP_WIN_WIDTH)
		doom->minimap->scale -= 0.1;
	while (wall->start.y * doom->minimap->scale > MINIMAP_WIN_HEIGHT)
		doom->minimap->scale -= 0.1;
	while (wall->end.x * doom->minimap->scale > MINIMAP_WIN_WIDTH)
		doom->minimap->scale -= 0.1;
	while (wall->end.y * doom->minimap->scale > MINIMAP_WIN_HEIGHT)
		doom->minimap->scale -= 0.1;
}

void	print_minimap_walls(t_doom *doom)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = doom->mdl->wall_count;
	wall = doom->mdl->wall_first;
	//printf("Walls | wall_count %d\n------------------\n\n", wc);
	while (wc--)
	{
		// printf("Wall id: %d | start: %d, %d | end: %d, %d\n",
		// 		wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		rescale_minimap(wall, doom);
		line.x1 = wall->start.x * doom->minimap->scale;
		line.y1 = wall->start.y * doom->minimap->scale;
		line.x2 = wall->end.x * doom->minimap->scale;
		line.y2 = wall->end.y * doom->minimap->scale;
		line.color = 0xffffffff;
		line.buff = doom->minimap->buff;
		render_line(&line);
		wall = wall->next;
	}
}

static void minimap_circle_enemy(t_doom *doom, t_enemy *enemy, unsigned int color)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;
	t_point scaled;

	pixels = doom->minimap->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			scaled.x = enemy->x * doom->minimap->scale;
			scaled.y = enemy->y * doom->minimap->scale;
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[scaled.x + x + ((scaled.y + y) * MINIMAP_WIN_WIDTH)] = color;
			x++;
		}
		y++;
		x = -radius;
	}
}

void	print_minimap_enemies(t_doom *doom)
{
	t_enemy	*enemy;
	t_line	line;
	int 	ec;
	unsigned int color;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		line.x1 = enemy->x * doom->minimap->scale;
		line.y1 = enemy->y * doom->minimap->scale;
		line.x2 = enemy->tail.x * doom->minimap->scale;
		line.y2 = enemy->tail.y * doom->minimap->scale;
		if (enemy->hp.cur == 0)
			color = 0xffff0000;
		else
			color = 0xff00ff00;
		line.color = color;
		line.buff = doom->minimap->buff;
		render_line(&line);
		minimap_circle_enemy(doom, enemy, color);
		enemy = enemy->next;
	}
}

static void minimap_circle_player(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;
	t_point scaled;

	pixels = doom->minimap->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			scaled.x = (int)doom->mdl->player.x * doom->minimap->scale;
			scaled.y = (int)doom->mdl->player.y * doom->minimap->scale;
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[scaled.x + x + ((scaled.y + y) * MINIMAP_WIN_WIDTH)] = 0xffffff00;
			x++;
		}
		y++;
		x = -radius;
	}
}

void		print_minimap_player(t_doom *doom)
{
	t_line	line;

	line.x1 = doom->mdl->player.x * doom->minimap->scale;
	line.y1 = doom->mdl->player.y * doom->minimap->scale;
	line.x2 = doom->mdl->player.tail.x * doom->minimap->scale;
	line.y2 = doom->mdl->player.tail.y * doom->minimap->scale;
	line.color = 0xffffff00;
	line.buff = doom->minimap->buff;
	render_line(&line);
	minimap_circle_player(doom);
}

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
		if (enemy->did_shoot == 1 && enemy->who_shot == enemy->id && enemy->hp.cur > 0)
		{
			// printf("Who shot: %d\n", doom->mdl->player.who_shot);
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

void		update_minimap(t_doom *doom)
{
	flood_buffer(doom->minimap->buff, 0xff000000);
	if (doom->minimap->player_ray_timeout > 0 && doom->mdl->player.shoot_cd != 0)
		print_player_ray(doom);
	if (doom->minimap->enemy_ray_timeout > 0)
		print_enemy_ray(doom);
	print_minimap_walls(doom);
	print_minimap_player(doom);
	print_minimap_enemies(doom);
	SDL_UpdateWindowSurface(doom->minimap->win);
}

void		init_minimap(t_doom *doom)
{
	int			win_x;
	int			win_y;

	if (!(doom->minimap = (t_minimap *)malloc(sizeof(t_minimap))))
		return ;
	doom->minimap->win = SDL_CreateWindow("Debug Minimap", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, MINIMAP_WIN_WIDTH, MINIMAP_WIN_HEIGHT, 0);
	if (doom->minimap->win == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window with SDL_CreateWindow on init_minimap.");
	doom->minimap->buff = SDL_GetWindowSurface(doom->minimap->win);
	if (doom->minimap->buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Surface with SDL_GetWindowSurface on init_minimap.");
	doom->minimap->scale = 1.0;
	SDL_GetWindowPosition(doom->win, &win_x, &win_y);
	SDL_SetWindowPosition(doom->minimap->win, win_x + WIN_WIDTH, win_y);
	update_minimap(doom);
}
