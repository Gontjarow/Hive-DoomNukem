/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <niko.gontjarow@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:28:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/29 18:52:23 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

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
	flood_window(doom->game->buff, 0xff000000);
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

void		render(t_doom *doom);

void		game_render(t_doom *doom)
{
		// These will be the doom->game key handling, right now it only supports the minimap
	// but once the game can be tested with 3D rendering, these will work for both
	if (doom->keystates[SDL_SCANCODE_ESCAPE])
	{
		// Open menu, quit game...
		printf("ESC key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_W])
	{
		// Walk forward
		doom->mdl->player.y--;
		doom->mdl->player.tail.y--;
		//printf("W key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_S])
	{
		// Walk backward
		doom->mdl->player.y++;
		doom->mdl->player.tail.y++;
		//printf("S key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_A])
	{
		// Rotate left or walk left (if free camera implemented)
		doom->mdl->player.x--;
		doom->mdl->player.tail.x--;
		//printf("A key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_D])
	{
		// Rotate right or walk right (if free camera implemented)
		doom->mdl->player.x++;
		doom->mdl->player.tail.x++;
		//printf("D key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_E])
	{
		// Use button, open doors, talk to NPC's...
		printf("E key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_SPACE])
	{
		// Jump
		printf("Spacebar key pressed!\n");
	}
	if (doom->keystates[SDL_SCANCODE_LSHIFT])
	{
		// Increase player's speed, sprint
		printf("Left Shift key pressed!\n");
	}
	update_minimap(doom);
	render(doom);
	SDL_UpdateWindowSurface(doom->game->win);
}

// !!! NEW CODE, TO BE CLEANED UP

void draw(unsigned int *pixel, t_xy start, t_xy end, int color)
{
	t_xy	dir;
	t_xy	abs;
	float	step;
	int		i;
	int		index;

	dir = vec2_sub(end, start);
	abs = vec2(fabs(dir.x), fabs(dir.y));
	step = (abs.x > abs.y) ? abs.x : abs.y;
	dir = vec2_div(dir, step);
	i = 0;
	while (i <= step)
	{
		index = (int)start.x + (int)start.y * GAME_WIN_WIDTH;
		if ((int)start.x >= 0 && (int)start.x < GAME_WIN_WIDTH
		&& ((int)start.y >= 0 && (int)start.y < GAME_WIN_HEIGHT)
		&& (index >= 0 && index < GAME_WIN_WIDTH * GAME_WIN_HEIGHT))
		{
			pixel[index] = color;
		}
		else
		{
			printf("bad pixel %d\n", index);
		}

		start.x += dir.x;
		start.y += dir.y;
		++i;
	}
}

// Raster-space bounding box
t_xy		bb_min(t_face face)
{
	int i;
	t_vert lowest;
	t_vert current;

	lowest = face.vert[0];
	i = 1;
	while (i < face.verts)
	{
		current = face.vert[i];
		if (current.y < lowest.y) lowest.y = current.y;
		if (current.x < lowest.x) lowest.x = current.x;
		++i;
	}
	return vec2_clamp(vec2(lowest.x, lowest.y), 0, GAME_WIN_HEIGHT);
}

// Raster-space bounding box
t_xy		bb_max(t_face face)
{
	int i;
	t_vert highest;
	t_vert current;

	highest = face.vert[0];
	i = 1;
	while (i < face.verts)
	{
		current = face.vert[i];
		if (current.y > highest.y) highest.y = current.y;
		if (current.x > highest.x) highest.x = current.x;
		++i;
	}
	return vec2_clamp(vec2(highest.x, highest.y), 0, GAME_WIN_WIDTH);
}

// Note: left < 0,  edge == 0,  right > 0
double edge(t_xy p, t_xy a, t_xy b)
{
	return ((p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x));
}

// Note: Triangles are assumed to be in CCW order as per Wavefront.
// When this is the case, the INSIDE of the triangle
// is on the left side of each edge. (negative space)
int inside(t_xy p, t_face face)
{
	t_xy v0 = vec42(face.vert[0]);
	t_xy v1 = vec42(face.vert[1]);
	t_xy v2 = vec42(face.vert[2]);

	return (edge(p, v0, v1) <= 0
		&& (edge(p, v1, v2) <= 0)
		&& (edge(p, v2, v0) <= 0));
}

// Fixed, exactly 3-vert triangle.
// Note: wavefront.obj triangles have verts in counter-clockwise order.
void		draw_tri(unsigned int *pixel, t_face face, int color)
{
	t_xy min = bb_min(face);
	t_xy max = bb_max(face);

	for (int y = min.y; y < max.y; ++y)
	for (int x = min.x; x < max.x; ++x)
	{
		if (inside(vec2(x, y), face))
		{
			pixel[x + y * GAME_WIN_WIDTH] = color;
		}
	}
}

void render(t_doom *doom)
{
	// SDL_memset(doom->game->buff->pixels, 0, GAME_WIN_WIDTH * doom->game->buff->pitch);
	t_mesh test = load_mesh_obj("tiny-donut.obj");

	int i = 0;
	while (i < test.faces)
	{
		t_vert *v = test.face[i++].vert;

		// Face-normal (counter-clockwise vertex order)
		t_xyz normal = vec3_norm(vec4_cross(
			vec4_sub(v[1], v[0]),
			vec4_sub(v[2], v[0])));

		// How much the face aligns with the camera (backface culling)
		// Note: The face must have the opposite direction as the camera to be seen.
		// 📷-->   <-|
		double facing = -vec3_dot(vec3(0,0,-1), normal);
		if (facing > 0)
		{
			// How much the face aligns with the light
			// Note: Normal must face in the OPPOSITE direction as the light-source to be lit.
			// 💡-->   <-|
			double light = -vec3_dot(vec3(0,0,-1), normal);
			if (light > 0)
			{
				// Greyscale brightness; Same value used for R, G, and B
				int color = 255 * light;
				color = color | color << 8 | color << 16;

				// Transformed face (moved and scaled to window size)
				// return;
				double s = 1.2;
				t_face tf = init_face(3,
					vec4((v[0].x + s) * GAME_MIDWIDTH / s, (v[0].y + s) * GAME_MIDHEIGHT / s, 0, 1),
					vec4((v[1].x + s) * GAME_MIDWIDTH / s, (v[1].y + s) * GAME_MIDHEIGHT / s, 0, 1),
					vec4((v[2].x + s) * GAME_MIDWIDTH / s, (v[2].y + s) * GAME_MIDHEIGHT / s, 0, 1));

				draw_tri(doom->game->buff->pixels, tf, color);
				free_verts(&tf);
			}
		}
	}

	free_faces(&test);
}