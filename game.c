/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/22 03:38:12 by ngontjar         ###   ########.fr       */
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

void	show_mat(t_matrix m, const char *name)
{
	printf("\n"
		"'%4.4s' { %8.3f | %8.3f | %8.3f | %8.3f }\n"
		"       { %8.3f | %8.3f | %8.3f | %8.3f }\n"
		"       { %8.3f | %8.3f | %8.3f | %8.3f }\n"
		"       { %8.3f | %8.3f | %8.3f | %8.3f }\n",
		name,
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]
	);
}

void	show_vec(t_xyz v, const char *name)
{
	printf("\n'%4.4s' { %8.3f | %8.3f | %8.3f }\n",
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


	t_camera render;
	t_matrix cam_rot = identity(); // Camera Rotation
	{
		cam_rot = rz;
	}
	render.rot = cam_rot;

	t_matrix world = identity(); // Object -> World transformation
	{
		world = multiply_m(rotate_x(0), rotate_y(0));
		world = multiply_m(world, t);
	}
	render.world_m = world;

	t_matrix view = identity(); // World -> View transformation
	{
		t_xyz c_up = vec3(0, 0, -1);
		t_xyz target = vec3(1, 0, 0);
		target = vec3_transform(target, render.rot);
		render.dir = target;
		target = vec3_add(pos, target);
		t_matrix camera_m = point_at(pos, target, c_up);
		view = inverse_m(camera_m);
	}
	render.view_m = view;

	t_matrix screen = identity(); // View -> Screen transformation
	{
		screen = multiply_m(m, s);
	}
	render.screen_m = screen;

	render.full_m = multiply_m(multiply_m(
		render.world_m, render.view_m), render.screen_m);

	// Create 2 quads
	// Note: The center or ORIGIN of the object is {0,0,0}
	t_mesh mesh;
	mesh = init_mesh(2,	// faces
		init_face(4,	// verts
		vec3( 0.5, -0.5, -0.5),
		vec3(-0.5, -0.5, -0.5),
		vec3(-0.5,  0.5, -0.5),
		vec3( 0.5,  0.5, -0.5)
	), init_face(4,		// verts
		vec3( 0.5, -0.5,  0.5),
		vec3(-0.5, -0.5,  0.5),
		vec3(-0.5,  0.5,  0.5),
		vec3( 0.5,  0.5,  0.5)
	));
	t_mesh transformed = mesh_transform(&mesh, &render.full_m);
	show_mat(render.world_m, "wrld");
	show_mat(render.view_m, "view");
	show_mat(render.screen_m, "scrn");
	show_mat(render.full_m, "full");
	show_vec(transformed.face[0].vert[0], "smpl");
	mesh_draw(surface->pixels, &transformed);
	free_faces(&mesh);
	free_faces(&transformed);
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
