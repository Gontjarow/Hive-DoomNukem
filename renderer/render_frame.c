#include "doom-nukem.h"

void	*memset_f(void *b, double c, size_t len)
{
	double *p;

	p = (double*)b;
	while (len > 0)
	{
		*(p++) = (double)c;
		len--;
	}
	return (b);
}

void zbuffer_to_window(t_doom *doom)
{
	static unsigned int *pixels = NULL;
	double *zbuffer;

	if (pixels == NULL)
		pixels = malloc(GAME_WIN_WIDTH * GAME_WIN_HEIGHT * sizeof(*pixels));
	zbuffer = get_zbuffer();

	int i = 0;
	int grey;
	while (i < GAME_WIN_WIDTH * GAME_WIN_HEIGHT)
	{
		grey = abs(zbuffer[i] / 10000) & 0xFF;
		pixels[i] = grey << 16 | grey << 8 | grey;
		++i;
	}

	ft_memcpy(doom->game->buff->pixels, pixels,
		GAME_WIN_WIDTH * GAME_WIN_HEIGHT * sizeof(*pixels));
}

void render_frame(t_doom *doom)
{
	flood_buffer(doom->game->buff, 0x112233);
	double *zbuffer = get_zbuffer();
	memset_f(zbuffer, INFINITY, GAME_WIN_WIDTH * GAME_WIN_HEIGHT);

	double		hrz = doom->mdl->player.rot_horizontal;
	int			px = doom->mdl->player.x;
	int			py = doom->mdl->player.y;
	t_xyz		pos = vec3_mul(vec3(px, 0, py), WORLD_SCALE_FACTOR);
				// pos.y = doom->mdl->room_first->floor_height;
	t_xyz		cam_dir = vec3_norm(vec3_sub(pos, vec3(0, 0, 0)));

	t_matrix	world = translate_m(0, 0, 0);
	t_matrix	dimensions = scale_m(GAME_MIDWIDTH/2, GAME_MIDHEIGHT/2, 1);
	t_matrix	modelview = lookat_m(pos, vec3(0, 0, 0), vec3(0,1,0));
	t_matrix	projection = project_pure_m();
	t_matrix	window = window_m(0.1, 1000);
	modelview = multiply_m(projection, modelview);

	t_obj		world_obj = doom->game->world_obj;
	t_obj		view_obj = obj_transform(modelview, world_obj);
	t_obj		clipped_obj = obj_clip(view_obj);
	t_obj		screen_obj;

	if (clipped_obj.f_count != 0)
	{
		screen_obj = screenspace(clipped_obj);
		t_vert tv1, tv2, tv3;
		tv1 = screen_obj.face->vert->data->pos;
		tv2 = screen_obj.face->vert->next->data->pos;
		tv3 = screen_obj.face->vert->next->next->data->pos;
		printf("\n");
		printf(
			"frame (%i %i)->(%f %f %f) \n"
			"      (X:%f Y:%f Z:%f (W:%f)) \n"
			"      (X:%f Y:%f Z:%f (W:%f)) \n"
			"      (X:%f Y:%f Z:%f (W:%f)) \n",
			px, py,	pos.x, pos.y, pos.z,
			tv1.x, tv1.y, tv1.z, tv1.w,
			tv2.x, tv2.y, tv2.z, tv2.w,
			tv3.x, tv3.y, tv3.z, tv3.w);
	}
	else return;

	int i = 0;
	while (i < screen_obj.f_count)
	{
		// t_vert *vo = test.face[i].vert;
		// t_vert *vt = screen.face[i].vert;
		t_actual_face *face = list2face(screen_obj.face, i);
		t_vert v0 = face->vert->data->pos;
		t_vert v1 = face->vert->next->data->pos;
		t_vert v2 = face->vert->next->next->data->pos;

		// Face-normal (counter-clockwise vertex order)
		t_xyz normal = vec3_norm(vec4_cross(
			vec4_sub(v1, v0),
			vec4_sub(v2, v0)));

		// How much the face aligns with the camera (backface culling)
		// Note: The face must have the opposite direction as the camera to be seen.
		// 📷-->   <-|
		// if (-vec3_dot(cam_dir, normal) > 0)
		{
			// How much the face aligns with the light
			// Note: Normal must face in the OPPOSITE direction as the light-source to be lit.
			// 💡-->   <-|
			double light = -vec3_dot(cam_dir, normal);
			// if (light > 0)
			{
				// Greyscale brightness; Same value used for R, G, and B
				int color = 255 * fabs(light);
				color = color | color << 8 | color << 16;

				draw_tri(doom->game->buff->pixels, face, color);

				// t_face tf = init_face(3, vt[0], vt[1], vt[2]);
				// draw_tri(doom->game->buff->pixels, tf, color);
				// draw_tri_color(doom->game->buff->pixels, tf);
				// free_verts(&tf);
				// SDL_UpdateWindowSurface(doom->game->win);
				// SDL_Delay(10);
			}
		}
		++i;
	}



	return; // ----> Todo: Continue <----

	/*
	t_mesh test;
	t_mesh		mv = mesh_transform(modelview, test);
	printf("debug start\n");
	for (int f = 0; f < mv.faces; ++f)
	{
		printf("%p = %f %f %f\n",
			mv.face[f].vert,
			mv.face[f].vert[0].w,
			mv.face[f].vert[1].w,
			mv.face[f].vert[2].w);
	}
	printf("debug end\n");
	t_mesh		culled = mesh_clip(mv);

	if (culled.faces == 0)
	{
		// printf("NOTHING TO DRAW\n");
		return;
	}

	t_mesh		ndc = mesh_normalize(culled);
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

	// printf("%f\n", test.face[0].vert[0].w);
	// printf("%f\n", mv.face[0].vert[0].w);
	// printf("%f\n", ndc.face[0].vert[0].w);
	// printf("%f\n", screen.face[0].vert[0].w);

	int i = 0;
	while (i < screen.faces)
	{
		//! test.face index won't always match with screen.face
		//! because of discarded faces or new faces created by clipping
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
		if (-vec3_dot(cam_dir, normal) > 0)
		{
			// How much the face aligns with the light
			// Note: Normal must face in the OPPOSITE direction as the light-source to be lit.
			// 💡-->   <-|
			double light = -vec3_dot(cam_dir, normal);
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

	// zbuffer_to_window(doom);
	// free_faces(&screen_mesh);
	free_faces(&test);
	*/
}
