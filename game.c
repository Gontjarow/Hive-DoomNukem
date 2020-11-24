/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <niko.gontjarow@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/10/05 19:00:37 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"
#include <stdint.h>

void 		init_game(t_doom *doom, int argc, char **argv)
{
	doom->game = (t_game*)malloc(sizeof(t_game));
	if (!doom->game)
		ft_die("Fatal error: Mallocing game struct failed at init_game.");
	doom->game->parent = doom;
	doom->game->win = SDL_CreateWindow("DoomNukem Play Level", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, GAME_WIN_WIDTH, GAME_WIN_HEIGHT, 0);
	if (!doom->game->win)
		ft_die("Fatal error: SDL_CreateWindow failed at init_game.");
	doom->game->buff = SDL_GetWindowSurface(doom->game->win);
	flood_buffer(doom->game->buff, 0xff000000);
	if (!doom->game->win)
		ft_die("Fatal error: SDL_GetWindowSurface failed at init_game.");
	doom->game->map_supplied = 0;
	doom->game->map_path = NULL;
	if (argc == 2)
	{
		doom->game->map_path = argv[1];
		doom->game->map_supplied = 1;
	}
	else
		ft_die("Fatal error: No map specified as argument to load with Play Level.");
}

void 		destroy_game(t_doom *doom)
{
	SDL_FreeSurface(doom->game->buff);
	SDL_DestroyWindow(doom->game->win);
	doom->game->win = NULL;
	doom->game->buff = NULL;
	doom->game->mdl = NULL;
	doom->game->parent = NULL;
	doom->game->map_path = NULL;
	free(doom->game);
	doom->game = NULL;
}

void 		game_mouse_motion(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

void 		game_mouse_down(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

void		game_key_down(t_doom *doom)
{
	return ;
}

void 		game_loop(t_doom *doom)
{
	if (doom->game_quit == 0)
		doom->game_quit = 0;
}

static double deg_to_rad(int deg)
{
	return (deg * M_PI / 180);
}

int		player_collision_with_enemies(t_doom *doom)
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
		dx = (int)doom->mdl->player.x - enemy->x;
		dy = (int)doom->mdl->player.y - enemy->y;
		distance = sqrt(dx * dx + dy * dy);
		if (distance < 10 + 10)
			return (-1);
		enemy = enemy->next;
	}
	return (0);
}

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

static double sq(double x) 
{
    return (x * x);
}

// int			check_hit(t_doom *doom, double bullet_pos_x, double bullet_pos_y, double r)
// {
// 	t_enemy	*enemy;
// 	int	ec;

// 	ec = doom->mdl->enemy_count;
// 	if (ec == 0)
// 		return (-1);
// 	enemy = doom->mdl->enemy_first;
// 	while (ec--)
// 	{
// 		double x0 = enemy->x, y0 = enemy->y;
// 		double x1 = doom->mdl->player.x, y1 = doom->mdl->player.y;
//     	double x2 = bullet_pos_x, y2 = bullet_pos_y;
//     	double A = y2 - y1;
//     	double B = x1 - x2;
//    		double C = x2 * y1 - x1 * y2;
//     	double a = sq(A) + sq(B);
//     	double b, c, d;
 
//     	if (fabs(B) >= EPS)
// 		{
//         	// if B isn't zero or close to it
//         	b = 2 * (A * C + A * B * y0 - sq(B) * x0);
//         	c = sq(C) + 2 * B * C * y0 - sq(B) * (sq(r) - sq(x0) - sq(y0));
//     	} 
// 		else
// 		{
//        		b = 2 * (B * C + A * B * x0 - sq(A) * y0);
//         	c = sq(C) + 2 * A * C * x0 - sq(A) * (sq(r) - sq(x0) - sq(y0));
//     	}
//     	d = sq(b) - 4 * a * c; // discriminant
// 		if (d == 0)	// line is tangent to circle, so just one intersect at most
// 			return (ec);
// 		else if (d > 0)		// two intersects at most
// 			return (ec);
// 		enemy = enemy->next;
// 	}
// 	return (-1);			// no intersection
// }

void		deal_damage(int enemy_id)
{
	printf("ENEMY HIT!!! YAY DEALING DAMAGE!!! TO ENEMY: %d\n", enemy_id);
}

static int	point_circle(double px, double py, double cx, double cy)
{
	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	double	dist_x;
	double	dist_y;
	double	dist;

	dist_x = px - cx;
	dist_y = py - cy;
	dist = sqrt((dist_x * dist_x) + (dist_y * dist_y));
	// if the distance is less than the circle's
	// radius the point is inside!
	if (dist <= 15) 
    	return (1);
  return (0);
}

static double	dist(double px, double py, double x1, double y1)
{
	double	distance;

	distance = sqrt(((px - x1) * (px - x1)) + ((py - y1) * (py - y1)));
	return (distance);
}

static int	line_point(double x1, double y1, double x2, double y2, double px, double py)
{
	double	d1;
	double	d2;
	double	len;
	double	buffer;
	// get distance from the point to the two ends of the line
	d1 = dist(px, py, x1, y1);
	d2 = dist(px, py, x2, y2);

	// get the length of the line
	len = dist(x1, y1, x2, y2);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	buffer = 0.001;    // higher # = less accurate

	// if the two distances are equal to the line's
	// length, the point is on the line!
	// note we use the buffer here to give a range,
	// rather than one #
	if (d1 + d2 >= len - buffer && d1 + d2 <= len + buffer)
		return (1);
	return (0);
}

int		check_hit(t_doom *doom)
{
	int		ec;
	int		inside1;
	int		inside2;
	int		on_segment;
	double	dist_x;
	double	dist_y;
	double	len;
	double	dot;
	double	closest_x;
	double	closest_y;
	double	distance;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return (0);
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		inside1 = point_circle(doom->mdl->player.x, doom->mdl->player.y, enemy->x, enemy->y);
		inside2 = point_circle(doom->mdl->player.bullet_pos_x, doom->mdl->player.bullet_pos_y, enemy->x, enemy->y);
		if (inside1 || inside2) 
			return (ec);
		// get length of the line
		dist_x = doom->mdl->player.x - doom->mdl->player.bullet_pos_x;
		dist_y = doom->mdl->player.y - doom->mdl->player.bullet_pos_y;
		len = sqrt((dist_x * dist_x) + (dist_y * dist_y));
		// get dot product of the line and circle
		dot = ( ((enemy->x - doom->mdl->player.x) * (doom->mdl->player.bullet_pos_x - doom->mdl->player.x)) +
			((enemy->y - doom->mdl->player.y) * (doom->mdl->player.bullet_pos_y - doom->mdl->player.y)) ) / pow(len,2);
		// find the closest point on the line
		closest_x = doom->mdl->player.x + (dot * (doom->mdl->player.bullet_pos_x - doom->mdl->player.x));
		closest_y = doom->mdl->player.y + (dot * (doom->mdl->player.bullet_pos_y - doom->mdl->player.y));
		// is this point actually on the line segment?
		// if so keep going, but if not, return false
		on_segment = line_point(doom->mdl->player.x, doom->mdl->player.y, doom->mdl->player.bullet_pos_x,
								doom->mdl->player.bullet_pos_y, closest_x, closest_y);
		if (!on_segment)
			return (-1);
		// get distance to closest point
		dist_x = closest_x - enemy->x;
		dist_y = closest_y - enemy->y;
		distance = sqrt((dist_x * dist_x) + (dist_y * dist_y));
		if (distance <= 15)
    		return (ec);
		enemy = enemy->next;
	}
	return (-1);
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
		// enemy_who_was_hit = check_hit(doom, doom->mdl->player.bullet_pos_x, doom->mdl->player.bullet_pos_y, 12);
		//ft_putnbr(discriminant);
		//ft_putendl(" ");
		if (enemy_who_was_hit >= 0)
		{
			doom->minimap->debug_ray_color = 0xff00ff00;
			deal_damage(enemy_who_was_hit);
		}
	}
	return (0);
}

void		game_render(t_doom *doom)
{
	// These will be the doom->game key handling, right now it only supports the minimap
	// but once the game can be tested with 3D rendering, these will work for both
	int		x;
	int		y;
	int		location_id;
	double	cur_x;
	double	cur_y;
	double	old_x;
	double	old_y;
	double	rad;

	cur_x = doom->mdl->player.x;
	cur_y = doom->mdl->player.y;
	old_x = cur_x;
	old_y = cur_y;
	if (doom->keystates[SDL_SCANCODE_ESCAPE])
	{
		// Open menu, quit game...
		printf("ESC key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_W])
	{
		// Walk forward
		rad = deg_to_rad(doom->mdl->player.rot);
		doom->mdl->player.x += ((double)doom->mdl->player.mov_speed) * -cos(rad);
 		doom->mdl->player.y += ((double)doom->mdl->player.mov_speed) * -sin(rad);
		x = doom->mdl->player.x + doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y + doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		printf("W key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_S])
	{
		// Walk backward
		rad = deg_to_rad(doom->mdl->player.rot);
		doom->mdl->player.x -= ((double)doom->mdl->player.mov_speed) * -cos(rad);
 		doom->mdl->player.y -= ((double)doom->mdl->player.mov_speed) * -sin(rad);
		x = doom->mdl->player.x - doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y - doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		printf("S key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_A])
	{
		// Rotate left
		doom->mdl->player.rot -= doom->mdl->player.rot_speed;
		if (doom->mdl->player.rot < 0)
			doom->mdl->player.rot = 359;
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x + doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y + doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		printf("A key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_D])
	{
		// Rotate right
		doom->mdl->player.rot += doom->mdl->player.rot_speed;
		if (doom->mdl->player.rot >= 360)
			doom->mdl->player.rot = 0;
		rad = deg_to_rad(doom->mdl->player.rot);
		x = doom->mdl->player.x + doom->mdl->player.mov_speed * -cos(rad);
		y = doom->mdl->player.y + doom->mdl->player.mov_speed * -sin(rad);
		doom->mdl->player.tail.x = x;
		doom->mdl->player.tail.y = y;
		printf("D key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_E])
	{
		// Use button, open doors, talk to NPC's...
		player_shoots(doom);
		printf("E key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_SPACE])
	{
		// Jump
		if (doom->mdl->player.is_jumping == 0 && doom->mdl->player.is_crouching == 0)
		{
			while (doom->mdl->player.is_jumping == 0)
			{
				doom->mdl->player.height += 10;
				if (doom->mdl->player.height == 200)
					doom->mdl->player.is_jumping = 1;
			}
			while (doom->mdl->player.is_jumping == 1)
			{
				doom->mdl->player.height -= 10;
				if (doom->mdl->player.height == 100)
					doom->mdl->player.is_jumping = 0;
			}
		}
		printf("Spacebar key pressed!\n");
	}

	/*
	**	Player Run Feature Handling
	*/

	if (doom->keystates[SDL_SCANCODE_LSHIFT] && !doom->mdl->player.run_lock)
	{
		doom->mdl->player.run_lock = 1;
		doom->mdl->player.is_running = 1;
		printf("LSHIFT key pressed!\n");
	}
	if (!doom->keystates[SDL_SCANCODE_LSHIFT] && doom->mdl->player.run_lock)
	{
		doom->mdl->player.is_running = 0;
		doom->mdl->player.run_lock = 0;
		printf("LSHIFT key released!\n");
	}
	if (doom->mdl->player.is_running && doom->mdl->player.mov_speed != doom->mdl->player.max_speed)
		doom->mdl->player.mov_speed++;
	else if (!doom->mdl->player.is_running && doom->mdl->player.mov_speed != 10 && !doom->mdl->player.crouch_lock)
		doom->mdl->player.mov_speed--;

	if (doom->keystates[SDL_SCANCODE_LCTRL] && !doom->mdl->player.crouch_lock)
	{
		doom->mdl->player.crouch_lock = 1;
		doom->mdl->player.is_crouching = 1;
		doom->mdl->player.mov_speed = 5;
		printf("LCTRL key pressed!\n");
	}
	if (!doom->keystates[SDL_SCANCODE_LCTRL] && doom->mdl->player.crouch_lock)
	{
		doom->mdl->player.crouch_lock = 0;
		doom->mdl->player.is_crouching = 0;
		doom->mdl->player.mov_speed = 10;
		printf("LCTRL key released!\n");
	}
	if (doom->mdl->player.is_crouching && doom->mdl->player.height != 50)
	{
		doom->mdl->player.height -= 10;
	}
	else if (!doom->mdl->player.is_crouching && doom->mdl->player.height != 100)
	{
		doom->mdl->player.height += 10;
	}
	/*
	**	Player Collision Detection, checking if it's position is valid according to where he is currently located
	*/
	location_id = check_location(doom, doom->mdl->player.x, doom->mdl->player.y);
	if (location_id == -1 || location_id == UINT_ERROR_CONSTANT || player_collision_with_enemies(doom) == -1)
	{
		doom->mdl->player.x = old_x;
		doom->mdl->player.y = old_y;
	}
	// ft_putnbr(check_location(doom));
	// ft_putstr("= [");
	// ft_putnbr(doom->mdl->player.x);
	// ft_putstr(",");
	// ft_putnbr(doom->mdl->player.y);
	// ft_putendl("] <- poly_map value.");
	update_minimap(doom);
	render_frame(doom);
	SDL_UpdateWindowSurface(doom->game->win);
	
}
