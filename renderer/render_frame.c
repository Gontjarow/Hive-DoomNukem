#include "doom-nukem.h"

void			*memset_f(void *b, double c, size_t len)
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

void			zbuffer_to_window(t_doom *doom)
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

// ---------------------

typedef struct	s_sector2
{
	double		floor;
	double		ceil;
	unsigned	vertex_count;
	t_xy		*vertex;
	signed		*neighbors;
}				t_sector2;

typedef struct	s_section
{
	int id;
	int left;
	int right;
}				t_section;

typedef struct	s_camera
{
	signed	sector_id;
	t_xyz	position;
	t_xyz	velocity;
	double	angle;
	double	sin;
	double	cos;
	double	yaw;
}				t_camera;

typedef struct	s_world
{
	unsigned	sector_count;
	t_sector2	*sectors;
	t_camera	player;
}				t_world;

// ---------------------

static void		print_data(t_world *world)
{
	unsigned sectors = world->sector_count;
	unsigned si = 0;
	while (si < sectors)
	{
		t_sector2 sector = world->sectors[si];
		unsigned verts = sector.vertex_count;
		printf("world->sectors[%3d]->vertex_count: %u\n", si, verts);
		unsigned vi = 0;
		while (vi < verts)
		{
			printf("\tvertex[%3d] x,y | %4.0f, %4.0f\n",
				vi, sector.vertex[vi].x, sector.vertex[vi].y);
			++vi;
		}
		++si;
	}
}

t_world			*get_world()
{
    static t_world  *world = NULL;
    t_model         *mdl;

    if (world == NULL)
    {
        world = malloc(sizeof(*world));
        mdl = get_model();
        world->sectors = malloc(mdl->room_count * sizeof(t_sector2));
		ft_bzero(world->sectors, mdl->room_count * sizeof(t_sector2));
        world->sector_count = 0;
        ft_memset(&world->player, 0, sizeof(world->player));
        world->player.position = vec3(mdl->player.x, mdl->player.y, 0);
        world->player.position = vec3_div(world->player.position, WORLD_SCALE);
        world->player.position.z = EYE_HEIGHT;
    }
    return (world);
}

static void		vertical_line(int column, int start, int end, int color)
{
	int *pixel;
	int draw;

	pixel = (int*)doom_ptr()->game->buff->pixels;
	start = clamp(start, 0, GAME_WIN_HEIGHT - 1);
	end = clamp(end, 0, GAME_WIN_HEIGHT - 1);

	// Always draw at least one pixel.
	draw = 1 + (end - start);
	while (draw--)
	{
		pixel[GAME_WIN_HEIGHT * (start + draw) + column] = color;
	}
}

t_world			*load_world(t_world *world)
{
    t_model *mdl = get_model();
	t_room *room = mdl->room_first;
    int room_index = 0;
    int rooms = mdl->room_count;
    while (~--rooms)
    {
        // Pointer to sector to modify
        t_sector2 *sector = &world->sectors[room_index];

        // Sector init
        sector->vertex_count = room->wall_count; // +1?
        sector->floor = 0;
        sector->ceil = 20;

        // Allocate
        int vertices = sector->vertex_count;
        sector->neighbors = malloc((vertices  ) * sizeof(*sector->neighbors));
        sector->vertex    = malloc((vertices+1) * sizeof(*sector->vertex)   );

        // Init allocated memory
        int vertex = 0;
        while (vertex < vertices)
        {
            sector->neighbors[vertex++] = -1;
        }

        // Copy vertex positions
        t_wall *wall = room->first_wall;
        sector->vertex[0] = vec2_div(vec2(wall->start.x, wall->start.y), WORLD_SCALE); // why are these (X/Y) flipped?
        int i = 1;
        int walls = sector->vertex_count; //room->wall_count;
        while (~--walls)
        {
            sector->vertex[i++] = vec2_div(vec2(wall->end.x, wall->end.y), WORLD_SCALE); // why are these (X/Y) flipped?
            wall = wall->next;
        }

        // Go next
        ++room_index;
        ++world->sector_count;
        room = room->next;
    }
	world->player.sector_id = room_id_from_polymap(mdl->poly_map, mdl->player.x, mdl->player.y);
    print_data(world);
    return (world);
}

int				ft_maxi(int a, int b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

int				ft_mini(int a, int b)
{
	if (a <= b)
		return (a);
	else
		return (b);
}

void			render_frame(t_doom *doom)
{
	t_world	*world;
	double	*zbuffer;

	world = doom->game->world;
	zbuffer = get_zbuffer();

	flood_buffer(doom->game->buff, 0x112233);
	memset_f(zbuffer, INFINITY, GAME_WIN_WIDTH * GAME_WIN_HEIGHT);

	// do the rendering
	int y_top[GAME_WIN_WIDTH];
	int y_bot[GAME_WIN_WIDTH];
	ft_memset(y_top,                   0, sizeof(y_top));
	ft_memset(y_bot, GAME_WIN_HEIGHT - 1, sizeof(y_bot));

	// int rendered_sectors[world->sector_count];
	// ft_memset(rendered_sectors, 0, sizeof(rendered_sectors));

	t_section	queue[MAX_SECTOR_QUEUE];
	t_section	*head = queue;
	t_section	*tail = queue;

	// First in queue will be the camera/player's sector.
	queue[0].id = world->player.sector_id;
	queue[0].left = 0;
	queue[0].right = GAME_WIN_WIDTH - 1;

	t_section	*now = &queue[0];
	const t_sector2 *const sector = &world->sectors[now->id];

	unsigned wall = 0;
	while (wall < sector->vertex_count)
	{
		// Calculate relative vertex positions for wall.
		t_xy v1 = vec2_sub(sector->vertex[wall+0], vec32(world->player.position));
		t_xy v2 = vec2_sub(sector->vertex[wall+1], vec32(world->player.position));

		// Rotate them around the player. (Negate player's rotation?)
		// Note: Y value is considered the Z depth at this point.
		world->player.angle = (float)deg_to_rad(get_model()->player.rot);
		world->player.sin = sinf(world->player.angle);
		world->player.cos = cosf(world->player.angle);
		t_xy rot_v1 = vec2_rot(v1, world->player.cos, world->player.sin);
		t_xy rot_v2 = vec2_rot(v2, world->player.cos, world->player.sin);

		if (rot_v1.y <= 0 && rot_v2.y <= 0)
		{
			printf("Both vertices are behind the player's view. Skip.\n");
			++wall;
			continue;
		}

		if (rot_v1.y <= 0 || rot_v2.y <= 0)
		{
			const double near_z = 0.0001;
			const double far_z = 5;
			const double near_side = 0.00001;
			const double far_side = 20;

			// Uhhh...
			t_xy intersect1 = vec2_intersect(rot_v1, rot_v2, vec2(-near_side, near_z), vec2(-far_side, far_z));
			t_xy intersect2 = vec2_intersect(rot_v1, rot_v2, vec2( near_side, near_z), vec2( far_side, far_z));

			// If the transformed point is behind the player's view,
			// replace it with the appropriate intersection. (whose depth is positive)
			if (rot_v1.y < near_z) rot_v1 = (intersect1.y > 0) ? intersect1 : intersect2;
			if (rot_v2.y < near_z) rot_v2 = (intersect1.y > 0) ? intersect1 : intersect2;
		}

		// Calculate perspective transformation.
		const double horizontal_fov = 0.73 * GAME_WIN_HEIGHT;
		const double vertical_fov = 0.2 * GAME_WIN_HEIGHT;
		t_xy scale1 = vec2(horizontal_fov / rot_v1.y, vertical_fov / rot_v1.y);
		t_xy scale2 = vec2(horizontal_fov / rot_v2.y, vertical_fov / rot_v2.y);
		int x1 = GAME_MIDWIDTH - (rot_v1.x * scale1.x);
		int x2 = GAME_MIDWIDTH - (rot_v2.x * scale2.x);

		if (x1 >= x2 || x2 < now->left || x1 > now->right)
		{
			printf("This section range is out of bounds. Skip.\n");
			++wall;
			continue;
		}

		// Calculate distance to floor and ceiling.
		double y_ceil  = sector->ceil - world->player.position.z;
		double y_floor = sector->floor - world->player.position.z;

		int neighbor = sector->neighbors[wall];

		// Calculate vertical line length.
		int y1_start = GAME_MIDHEIGHT - (int)(y_ceil  * scale1.y);
		int y1_end   = GAME_MIDHEIGHT - (int)(y_floor * scale1.y);
		int y2_start = GAME_MIDHEIGHT - (int)(y_ceil  * scale2.y);
		int y2_end   = GAME_MIDHEIGHT - (int)(y_floor * scale2.y);

		// ACTUALLY DO THE RENDERING
		int x_begin	= ft_maxi(x1, now->left);
		int x_end	= ft_maxi(x2, now->right);
		int x = x_begin;
		while (x <= x_end)
		{
			int y_begin = y2_start + (x - x1) * (y2_start - y1_start) / (x2 - x1);
			int y_end   = y2_end   + (x - x1) * (y2_end   - y1_end  ) / (x2 - x1);
			double y_begin_clamped = clamp(y_begin, y_top[x], y_bot[x]);
			double y_end_clamped   = clamp(y_end,   y_top[x], y_bot[x]);

			// ceiling
			vertical_line(x, y_top[x], y_begin_clamped-1, 0x9AC4F8);

			// wall
			if (neighbor >= 0)
				vertical_line(x, y_begin_clamped, y_end_clamped, 0x9A275A);
			else
				vertical_line(x, y_begin_clamped, y_end_clamped, 0xCB958E);

			// floor
			vertical_line(x, y_end_clamped, y_bot[x], 0x99EDCC);
			++x;
		}

		++wall;
	}
}
