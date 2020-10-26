#include "doom_nukem.h"

void render_frame(t_doom *doom)
{
	flood_window(doom->game->buff, 0x112233);
	t_mesh test = load_mesh_obj("tiny-donut.obj");

	printf("load_mesh_obj succeeded\n");

	double		hrz = doom->mdl->player.rot_horizontal;
	int px = doom->mdl->player.x;
	int py = doom->mdl->player.y;
	t_xyz		pos = vec3(px, 0, py);

	t_matrix	world = translate_m(0, 0, 0);
	t_matrix	dimensions = scale_m(GAME_MIDWIDTH/2, GAME_MIDHEIGHT/2, 1);
	t_matrix	modelview = lookat_m(pos, vec3(0, 0, 0), vec3(0,1,0));
	t_matrix	projection = project_pure_m();
	t_matrix	window = window_m(0.1, 1000);
	modelview = multiply_m(world, modelview);
	modelview = multiply_m(projection, modelview);
	// modelview = multiply_m(dimensions, modelview);


	t_mesh		mv = mesh_transform(modelview, test);
	t_mesh		ndc = mesh_normalize(mv);
	// t_mesh		screen = mesh_transform(dimensions, ndc);
	t_mesh		screen = mesh_transform(window, ndc);
	// t_mesh	world_mesh   = mesh_transform(world, test);

	// t_mesh	eye_mesh     = mesh_transform(modelview, world_mesh);

	// t_mesh	clip_mesh    = mesh_transform(projection, eye_mesh);

	// t_mesh	ndc_mesh     = mesh_normalize(clip_mesh);

	// t_mesh	screen_mesh  = mesh_transform(dimensions, eye_mesh);

	// free_faces(&eye_mesh);
	// free_faces(&clip_mesh);
	// free_faces(&ndc_mesh);

	int i = 0;
	while (i < test.faces)
	{
		t_vert *vo = test.face[i].vert;
		t_vert *vt = screen.face[i].vert;
		++i;

		// Face-normal (counter-clockwise vertex order)
		t_xyz normal = vec3_norm(vec4_cross(
			vec4_sub(vo[1], vo[0]),
			vec4_sub(vo[2], vo[0])));

		// How much the face aligns with the camera (backface culling)
		// Note: The face must have the opposite direction as the camera to be seen.
		// 📷-->   <-|
		if (-vec3_dot(vec3(0,0,-1), normal) > 0)
		{
			// How much the face aligns with the light
			// Note: Normal must face in the OPPOSITE direction as the light-source to be lit.
			// 💡-->   <-|
			double light = -vec3_dot(vec3(0,0,-1), normal);
			// if (light > 0)
			{
				// Greyscale brightness; Same value used for R, G, and B
				int color = 255 * fabs(light);
				color = color | color << 8 | color << 16;

				t_face tf = init_face(3, vt[0], vt[1], vt[2]);

				draw_tri(doom->game->buff->pixels, tf, color);
				// draw_tri_color(doom->game->buff->pixels, tf);
				free_verts(&tf);
				// SDL_UpdateWindowSurface(doom->game->win);
				// SDL_Delay(10);
			}
		}
	}

	// free_faces(&screen_mesh);
	free_faces(&test);
}
