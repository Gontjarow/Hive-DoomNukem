/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/21 22:20:58 by ngontjar         ###   ########.fr       */
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

void show_vec(t_xyz v, const char *name)
{
	printf("'%4.4s' { %8.3f | %8.3f | %8.3f }\n",
		name, v.x, v.y, v.z
	);
}

void 		game_mouse_motion(t_doom *doom)
{
	SDL_MouseMotionEvent	e = doom->event.motion;
	t_euler					*rot = &(doom->player->angle);
	t_xyz					pos = doom->player->pos;
	SDL_Surface				*surface = doom->game->buff;

	{
		printf("mouse abs {%4d %4d} rel {%4d %4d}\n", e.x, e.y, e.xrel, e.yrel);
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
		printf("player rot %f, yaw %f\n", rot->y, rot->z);
		printf("player pos {%.3f %.3f %.3f}\n", pos.x, pos.y, pos.z);

	}

	// Create all relevant matrices first.
	t_matrix rz = rotate_z(rot->z * DEG_TO_RAD);
	t_matrix ry = rotate_y(rot->y * DEG_TO_RAD);
	t_matrix rx = rotate_x(0);
	t_matrix t = translation(0, 0, -1.5);
	t_matrix m = perspective(90, 0.1, 64);
	t_matrix s = scale(GAME_MID_X, GAME_MID_Y, 1);


	t_matrix cam_rot = identity(); // Camera Rotation
	cam_rot = rz;


	t_matrix world = identity(); // Object -> World transformation
	world = multiply_m(rotate_x(0), rotate_y(0));
	world = multiply_m(world, t);

	t_matrix view = identity(); // World -> View transformation
	t_xyz c_up = vec3(0, 0, -1);
	t_xyz target = vec3(1, 0, 0);
	target = vec3_transform(target, cam_rot);
	target = vec3_add(pos, target);
	t_matrix camera_m = point_at(pos, target, c_up);
	view = inverse_m(camera_m);


	t_matrix screen = identity(); // View -> Screen transformation
	screen = multiply_m(m, s);

	{
		// Create 2 quads
		// Note: The center or ORIGIN of the object is {0,0,0}
		t_xyz A, B, C, D;
		t_xyz E, F, G, H;
		t_xyz zero, fwdX, fwdY, fwdZ;
		{
			A = vec3( 0.5, -0.5, -0.5);
			B = vec3(-0.5, -0.5, -0.5);
			C = vec3(-0.5,  0.5, -0.5);
			D = vec3( 0.5,  0.5, -0.5);

			E = vec3( 0.5, -0.5,  0.5);
			F = vec3(-0.5, -0.5,  0.5);
			G = vec3(-0.5,  0.5,  0.5);
			H = vec3( 0.5,  0.5,  0.5);

			fwdX = vec3(1, 0, 0);
			fwdY = vec3(0, 1, 0);
			fwdZ = vec3(0, 0, 1);
			zero = vec3(0, 0, 0);
		}

		// base axes in screen coordinates only
		t_xyz SX, SY, SZ, SO;
		{
			SX = vec3_transform(fwdX, screen);
			SY = vec3_transform(fwdY, screen);
			SZ = vec3_transform(fwdZ, screen);
			SO = vec3_transform(zero, screen);
		}

		// base axes in world coordinates
		t_xyz WX, WY, WZ, WO;
		{
			WX = vec3_transform(fwdX, world);
			WY = vec3_transform(fwdY, world);
			WZ = vec3_transform(fwdZ, world);
			WO = vec3_transform(zero, world);
			WX = vec3_transform(WX, view);
			WY = vec3_transform(WY, view);
			WZ = vec3_transform(WZ, view);
			WO = vec3_transform(WO, view);
			WX = vec3_transform(WX, screen);
			WY = vec3_transform(WY, screen);
			WZ = vec3_transform(WZ, screen);
			WO = vec3_transform(WO, screen);

			// WX = vec3_transform(fwdX, view);
			// WY = vec3_transform(fwdY, view);
			// WZ = vec3_transform(fwdZ, view);
			// WO = vec3_transform(zero, view);
			// WX = vec3_transform(WX, screen);
			// WY = vec3_transform(WY, screen);
			// WZ = vec3_transform(WZ, screen);
			// WO = vec3_transform(WO, screen);
		}

		// Debug cube in world coordinates
		t_xyz TA, TB, TC, TD;
		t_xyz TE, TF, TG, TH;
		{
			TA = vec3_transform(A, view);
			TB = vec3_transform(B, view);
			TC = vec3_transform(C, view);
			TD = vec3_transform(D, view);
			TE = vec3_transform(E, view);
			TF = vec3_transform(F, view);
			TG = vec3_transform(G, view);
			TH = vec3_transform(H, view);

			// Transformed debug cube in screen coordinates
			TA = vec3_transform(TA, screen);
			TB = vec3_transform(TB, screen);
			TC = vec3_transform(TC, screen);
			TD = vec3_transform(TD, screen);
			TE = vec3_transform(TE, screen);
			TF = vec3_transform(TF, screen);
			TG = vec3_transform(TG, screen);
			TH = vec3_transform(TH, screen);
		}

		// Just connecting the dots...
		{
			show_vec(WO, "WO");
			show_vec(WX, "WX");

			// Screen axes
			ft_draw(surface->pixels, SO, SX, 0xC97064);
			ft_draw(surface->pixels, SO, SY, 0xA6B07E);
			ft_draw(surface->pixels, SO, SZ, 0x759EB8);

			// Object axes
			ft_draw(surface->pixels, WO, WX, 0xff0000);//0xDC0073);
			ft_draw(surface->pixels, WO, WY, 0xff00);//0x540d6e);
			ft_draw(surface->pixels, WO, WZ, 0xff);//0x00A1E4);

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
	}
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

	if(doom->keystates[SDL_SCANCODE_W])
		doom->player->pos.x += 0.1;
	else if(doom->keystates[SDL_SCANCODE_S])
		doom->player->pos.x -= 0.1;
	if(doom->keystates[SDL_SCANCODE_D])
		doom->player->pos.y += 0.1;
	else if(doom->keystates[SDL_SCANCODE_A])
		doom->player->pos.y -= 0.1;
	if(doom->keystates[SDL_SCANCODE_SPACE])
		doom->player->pos.z += 0.1;
	if(doom->keystates[SDL_SCANCODE_LCTRL])
		doom->player->pos.z -= 0.1;
}

void		game_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->game->win);
}
