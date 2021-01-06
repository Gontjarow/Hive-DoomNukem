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

static double deg_to_rad(int deg)
{
	return (deg * M_PI / 180);
}

void render_frame(t_doom *doom)
{
	flood_buffer(doom->game->buff, 0x112233);
	double *zbuffer = get_zbuffer();
	memset_f(zbuffer, INFINITY, GAME_WIN_WIDTH * GAME_WIN_HEIGHT);

	double		rad = deg_to_rad(doom->mdl->player.rot);

	int			px = doom->mdl->player.x;
	int			py = doom->mdl->player.y;
	t_xyz		pos = vec3_mul(vec3(px, 0, py), WORLD_SCALE_FACTOR);
				// pos.y = doom->mdl->room_first->floor_height;
	t_xyz		cam_dir = vec3_norm(vec3_sub(pos, vec3(0, 0, 0)));

	t_matrix	world = translate_m(0, 0, 0);
	t_matrix	dimensions = scale_m(GAME_MIDWIDTH/2, GAME_MIDHEIGHT/2, 1);
	t_matrix	view = lookat_m(pos, vec3_add(pos, vec3(cos(rad), 0, sin(rad))), vec3(0,1,0));
	t_matrix	projection = project_pure_m();
	t_matrix	perspective = project_m(90, GAME_WIN_ASPECT, 0.1, 1000);
	t_matrix	window = window_m(0.1, 1000);

																	// start with model coordinates
																	// apply model matrix (identity, no change)
	t_obj		world_obj = doom->game->world_obj;					// -> world coordinates

																	// apply view matrix
	t_obj		view_obj = obj_transform(view, world_obj);			// -> camera coordinates

																	// apply projection matrix
				view_obj = obj_transform(projection, view_obj);		// -> homogenous coordinates (w!=1)

	t_obj		clipped_obj = obj_clip(view_obj);
	// t_obj		clipped_obj = view_obj;
	t_obj		screen_obj;

	if (clipped_obj.f_count != 0)
	{
		screen_obj = screenspace(clipped_obj);
		t_vert tv1, tv2, tv3;
		tv1 = screen_obj.face->vert->data->pos;
		tv2 = screen_obj.face->vert->next->data->pos;
		tv3 = screen_obj.face->vert->next->next->data->pos;
		// printf("\n");
		// printf(
		// 	"frame (%i %i)->(%f %f %f) \n"
		// 	"      (X:%f Y:%f Z:%f (W:%f)) \n"
		// 	"      (X:%f Y:%f Z:%f (W:%f)) \n"
		// 	"      (X:%f Y:%f Z:%f (W:%f)) \n",
		// 	px, py,	pos.x, pos.y, pos.z,
		// 	tv1.x, tv1.y, tv1.z, tv1.w,
		// 	tv2.x, tv2.y, tv2.z, tv2.w,
		// 	tv3.x, tv3.y, tv3.z, tv3.w);
		printf("\n");
		for(int i = 0; i < clipped_obj.f_count; ++i)
		{
			for(int v = 0; v < 3; ++v)
			{
				t_actual_face *face = list2face(clipped_obj.face, i);
				t_face_vert *vrt = list2fvert(face->vert, v);
				t_vert vp = vrt->data->pos;
				printf("tri %i vert %i (%8.4f, %8.4f, %8.4f, %8.4f)\n", i, v, vp.x, vp.y, vp.z, vp.w);
			}
			printf("\n");
		}
	}
	else return;

	int i = 0;
	while (i < screen_obj.f_count)
	{
		// t_vert *vo = test.face[i].vert;
		// t_vert *vt = screen.face[i].vert;
		t_actual_face *clipped_face = list2face(clipped_obj.face, i);
		t_actual_face *screen_face = list2face(screen_obj.face, i);
		t_vert v0 = clipped_face->vert->data->pos;
		t_vert v1 = clipped_face->vert->next->data->pos;
		t_vert v2 = clipped_face->vert->next->next->data->pos;

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

				draw_tri(doom->game->buff->pixels, screen_face, color);
				printf("drew tri %i\n", i);
				// SDL_UpdateWindowSurface(doom->game->win);
				// SDL_Delay(40);
			}
		}
		++i;
	}

	// Todo: Free stuff
}
