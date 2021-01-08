#include "renderer.h"

void			draw(unsigned int *pixel, t_xy start, t_xy end, int color)
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
			pixel[index] = color;
		start.x += dir.x;
		start.y += dir.y;
		++i;
	}
}

t_doom			*get_doom(t_doom *doom)
{
	static t_doom *doom_static = NULL;

	if (doom_static == NULL)
		doom_static = doom;
	return (doom_static);
}

SDL_Surface		*get_pbuffer(SDL_Surface *p)
{
	static SDL_Surface *pbuffer = NULL;

	if (pbuffer == NULL)
		pbuffer = p;
	return (pbuffer);
}
double			*get_zbuffer()
{
	static double *zbuffer = NULL;

	if (zbuffer == NULL)
		zbuffer = malloc(GAME_WIN_WIDTH * GAME_WIN_HEIGHT * sizeof(*zbuffer));
	return (zbuffer);
}

int				zbuffer_ok(int index, double depth)
{
	double *zbuffer;

	zbuffer = get_zbuffer();
	if (zbuffer[index] > depth)
	{
		zbuffer[index] = depth;
		return (1);
	}
	// printf("%f  %f\n", zbuffer[index], depth);
	return (0);
}

double			face_depth(t_xyz weight, t_actual_face *face)
{
	return (weight.x * list2fvert(face->vert, 0)->data->pos.z
		  + weight.y * list2fvert(face->vert, 1)->data->pos.z
		  + weight.z * list2fvert(face->vert, 2)->data->pos.z);
}

static			render_box(t_xy c1, t_xy c2) // bounding box rainbow debug
{
	static unsigned int i = 0;

	t_line l;
	l.buff = get_pbuffer(NULL);
	l.color = ((i%3 == 0) ? 0xFF : (i%3 == 1) ? 0xFF00 : 0xFF0000); // modulo-shift wasn't working??
	l.x1 = c1.x; l.y1 = c1.y;
	l.x2 = c2.x; l.y2 = c1.y;
	render_line(&l);
	l.x1 = c1.x; l.y1 = c1.y;
	l.x2 = c1.x; l.y2 = c2.y;
	render_line(&l);
	l.x1 = c2.x; l.y1 = c2.y;
	l.x2 = c1.x; l.y2 = c2.y;
	render_line(&l);
	l.x1 = c2.x; l.y1 = c2.y;
	l.x2 = c2.x; l.y2 = c1.y;
	render_line(&l);
	++i;
}

// Fixed, exactly 3-vert triangle.
// Note: wavefront.obj triangles have verts in counter-clockwise order.
void			draw_tri(unsigned int *pixel, t_actual_face *face, int color)
{
	t_xy min = bb_min(face);
	t_xy max = bb_max(face);
	render_box(min, max);

	// compare (<=) to ensure zero-width triangles get drawn for clarity.
	for (int y = min.y; y <= max.y; ++y)
	for (int x = min.x; x <= max.x; ++x)
	{
		if (inside(vec2(x, y), face))
		{
			double depth = face_depth(bary(vec2(x, y), face), face);
			if (zbuffer_ok(x + y * GAME_WIN_WIDTH, depth))
				pixel[x + y * GAME_WIN_WIDTH] = color;
			else
			{
				pixel[x + y * GAME_WIN_WIDTH] = 0x008FFF;
			}

		}
		else
		{
			// pixel[x + y * GAME_WIN_WIDTH] = 0xFFF800;
		}
	}
}

void			draw_tri_color(unsigned int *pixel, t_actual_face *face)
{
	t_xy v0 = vec42(list2fvert(face->vert, 0)->data->pos);
	t_xy v1 = vec42(list2fvert(face->vert, 1)->data->pos);
	t_xy v2 = vec42(list2fvert(face->vert, 2)->data->pos);
	t_xy min = bb_min(face);
	t_xy max = bb_max(face);

	for (int y = min.y; y < max.y; ++y)
	for (int x = min.x; x < max.x; ++x)
	{
		double area2 = edge(v0, v1, v2);
		t_xyz weight = bary(vec2(x, y), face);
		if (weight.x <= 0 && weight.y <= 0 && weight.z <= 0)
		{
			weight = vec3_div(weight, area2);
			int r = 255 * weight.x;
			int g = 255 * weight.y;
			int b = 255 * weight.z;
			pixel[x + y * GAME_WIN_WIDTH] = (r << 16 | g << 8 | b);
		}
	}
}
