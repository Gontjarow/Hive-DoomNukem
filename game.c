/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/20 07:41:14 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_player	*init_player(t_xyz pos, t_xyz rot, int hp)
{
	t_player *p;

	if ((p = (t_player*)malloc(sizeof(t_player))))
	{
		p->hp.max = hp;
		p->hp.cur = p->hp.max;
		p->pos = pos;
		p->angle = rot;
		ft_memset(&(p->wep), 0, sizeof(t_weapon));
	}
	return (p); // Returns NULL if malloc fails, sets values if succeeds.
}
void 		init_game(t_doom *doom, int argc, char **argv)
{
	doom->game = (t_game*)malloc(sizeof(t_game));
	doom->player = init_player(vec3_zero(), vec3_zero(), 100);
	if (!doom->game || !doom->player)
		ft_die("Fatal error: Mallocing game/player struct failed at init_game.");
	doom->game->parent = doom;
	doom->game->win = SDL_CreateWindow("DoomNukem Play Level", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, GAME_WIN_WIDTH, GAME_WIN_HEIGHT, 0);
	if (!doom->game->win)
		ft_die("Fatal error: SDL_CreateWindow failed at init_game.");
	doom->game->buff = SDL_GetWindowSurface(doom->game->win);
	if (!doom->game->buff)
		ft_die("Fatal error: SDL_GetWindowSurface failed at init_game.");
	if (argc == 2)
		doom->game->map_path = argv[1];
	else
		ft_die("Fatal error: No map specified as argument to load with Play Level.");
	if (0 != SDL_SetRelativeMouseMode(SDL_TRUE))
		ft_die("Fatal error: SDL_SetRelativeMouseMode not supported!");
}

void 		destroy_game(t_doom *doom)
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_FreeSurface(doom->game->buff);
	SDL_DestroyWindow(doom->game->win);
	doom->game->win = NULL;
	doom->game->buff = NULL;
	free(doom->game);
	doom->game = NULL;
}

static void show_vec(t_xyz v, const char *name)
{
	printf("'%4.4s' { %8.3f | %8.3f | %8.3f }\n",
		name, v.x, v.y, v.z
	);
}

void 		game_mouse_motion(t_doom *doom)
{
	SDL_MouseMotionEvent	e = doom->event.motion;
	t_euler					*rot = &(doom->player->angle);
	SDL_Surface				*surface = doom->game->buff;
	printf("%d %d %d %d\n", e.x, e.y, e.xrel, e.yrel);

	SDL_memset(surface->pixels, 0, surface->h * surface->pitch);

	// X is the "forward" axis. Rotating around it would barrelroll our camera.

	// Z is the "up" axis. Horizontal mouse movement turns us left/right.
	//	To turn left/right, we must rotate around the up-axis.
	rot->z += e.xrel * 0.15;

	// Y is the "side" (left or right, TBD) axis. Vertical mouse movement turns us up/down.
	//	To turn up/down, we must rotate around the side-axis.
	rot->y += e.yrel * 0.15;

	// Note: Right now, Y is effectively left-handed (towards right)
	//	because of the natural Y direction of screen coordinates.
	//	(Mouse down == Positive screen Y == angle Y + value)
	//	(Mouse up   == Negative screen Y == angle Y - value)

	// Let's keep us in the -180:180 range for now...
	if (rot->z > 180)  rot->z -= 360;
	if (rot->z < -180) rot->z += 360;
	// Except vertical angle should be -90:90 degrees up/down.
	if (rot->y > 90)   rot->y = 90;
	if (rot->y < -90)  rot->y = -90;
	printf("player pitch %f, yaw %f\n", rot->y, rot->z);

	// Create all relevant matrices first.
	t_matrix rz = rotate_z(rot->z * DEG_TO_RAD);
	t_matrix ry = rotate_y(rot->y * DEG_TO_RAD);
	t_matrix t = translation(0, 2, -1);
	t_matrix m = perspective(90, 0.1, 64);
	t_matrix s = scale(GAME_MID_X, GAME_MID_Y, 1);

	// Object -> World transformation
	t_matrix world;
	world = multiply_m(rz, ry);
	world = multiply_m(world, t);

	// World -> View transformation
	t_matrix view;
	t_xyz c_pos = (t_xyz){0, 0, 0};

	t_xyz c_dir = (t_xyz){1, 0, 0};
	t_xyz c_up = (t_xyz){0, 0, -1};
	t_xyz target = vec3_add(c_pos, c_dir);
	view = inverse_m(point_at(c_pos, target, c_up));
	view = multiply_m(world, view);

	// View -> Screen transformation
	t_matrix screen;
	screen = multiply_m(view, m);
	screen = multiply_m(screen, s);

	// Create 2 quads
	// Note: The center or ORIGIN of the object is {0,0,0}
	t_xyz A = (t_xyz){ 0.5, -0.5, -0.5};
	t_xyz B = (t_xyz){-0.5, -0.5, -0.5};
	t_xyz C = (t_xyz){-0.5,  0.5, -0.5};
	t_xyz D = (t_xyz){ 0.5,  0.5, -0.5};

	t_xyz E = (t_xyz){ 0.5, -0.5,  0.5};
	t_xyz F = (t_xyz){-0.5, -0.5,  0.5};
	t_xyz G = (t_xyz){-0.5,  0.5,  0.5};
	t_xyz H = (t_xyz){ 0.5,  0.5,  0.5};

	t_xyz fwdX = (t_xyz){1, 0, 0};
	t_xyz fwdY = (t_xyz){0, 1, 0};
	t_xyz fwdZ = (t_xyz){0, 0, 1};
	t_xyz zero = (t_xyz){0, 0, 0};

	// base axes in screen coordinates only
	t_xyz TX = vec3_transform(fwdX, m);
	t_xyz TY = vec3_transform(fwdY, m);
	t_xyz TZ = vec3_transform(fwdZ, m);
	t_xyz TO = vec3_transform(zero, m);
	TX = vec3_transform(TX, s);
	TY = vec3_transform(TY, s);
	TZ = vec3_transform(TZ, s);
	TO = vec3_transform(TO, s);

	// base axes
	t_xyz WX = vec3_transform(fwdX, screen);
	t_xyz WY = vec3_transform(fwdY, screen);
	t_xyz WZ = vec3_transform(fwdZ, screen);
	t_xyz WO = vec3_transform(zero, screen);

	t_xyz TA = vec3_transform(A, screen);
	t_xyz TB = vec3_transform(B, screen);
	t_xyz TC = vec3_transform(C, screen);
	t_xyz TD = vec3_transform(D, screen);

	t_xyz TE = vec3_transform(E, screen);
	t_xyz TF = vec3_transform(F, screen);
	t_xyz TG = vec3_transform(G, screen);
	t_xyz TH = vec3_transform(H, screen);

	show_vec(TB, "4 TB");
	show_vec(TG, "4 TG");

	// Just connecting the dots...

	// Screen axes
	ft_draw(surface->pixels, TO, TX, 0xFF0000);
	ft_draw(surface->pixels, TO, TY, 0x00FF00);
	ft_draw(surface->pixels, TO, TZ, 0x0000FF);
	// Object axes
	ft_draw(surface->pixels, WO, WX, 0xFF0000);
	ft_draw(surface->pixels, WO, WY, 0x00FF00);
	ft_draw(surface->pixels, WO, WZ, 0x0000FF);

	// First quad, red
	ft_draw(surface->pixels, TA, TB, 0xFF0000);
	ft_draw(surface->pixels, TB, TC, 0xFF0000);
	ft_draw(surface->pixels, TC, TD, 0xFF0000);
	ft_draw(surface->pixels, TD, TA, 0xFF0000);

	// Second quad, green
	ft_draw(surface->pixels, TE, TF, 0x00FF00);
	ft_draw(surface->pixels, TF, TG, 0x00FF00);
	ft_draw(surface->pixels, TG, TH, 0x00FF00);
	ft_draw(surface->pixels, TH, TE, 0x00FF00);

	// connecting lines, white
	ft_draw(surface->pixels, TA, TE, 0xFFFFFF);
	ft_draw(surface->pixels, TB, TF, 0xFFFFFF);
	ft_draw(surface->pixels, TC, TG, 0xFFFFFF);
	ft_draw(surface->pixels, TD, TH, 0xFFFFFF);
}

void 		game_mouse_down(t_doom *doom)
{
	uint32_t  color;

	color = 0xff00ff00;
	set_pixel(doom->game->buff, doom->event.motion.x, doom->event.motion.y, color);
	set_pixel(doom->game->buff,doom->event.motion.x+1, doom->event.motion.y+1, color);
	set_pixel(doom->game->buff,doom->event.motion.x+1, doom->event.motion.y-1, color);
	set_pixel(doom->game->buff,doom->event.motion.x-1, doom->event.motion.y+1, color);
	set_pixel(doom->game->buff,doom->event.motion.x-1, doom->event.motion.y-1, color);
}

void 		game_loop(t_doom *doom)
{
	// Random placeholder demo
	static int cooldown = 0;
	if (!cooldown)
	{
		// ft_putendl("Game looping!");
		cooldown = 100;
	} else
		cooldown--;
	set_pixel(doom->game->buff,cooldown+1, cooldown+1, 0xffffffff);
	set_pixel(doom->game->buff,cooldown, cooldown, 0xff000000);
}

void		game_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->game->win);
}
