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

void			*memset_i(void *b, int c, size_t len)
{
	int *p;

	p = (int*)b;
	while (len > 0)
	{
		*(p++) = c;
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
	while (draw-- > 0)
	{
		pixel[GAME_WIN_WIDTH * (start + draw) + column] = color;
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

        // Allocate fixed size blocks
		// Note: sector->vertex has one extra index
		// to enable us to later iterate with [i+0], [i+1] pairs.
		// Note: This extra index is NOT included in vertex_count.
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
		// Note: The last index will have the same data as the first index.
        t_wall *wall = room->first_wall;
        int i = 0;
        int walls = sector->vertex_count; //room->wall_count;
        while (i < walls)
        {
            sector->vertex[i++] = vec2_div(vec2(wall->start.x, wall->start.y), WORLD_SCALE); // why are these (X/Y) flipped?
            wall = wall->next;
        }
        sector->vertex[i] = sector->vertex[0];

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

double yaw_height(double ceil, double wall_y, double player_yaw)
{
	// printf("yaw_height(%f, %f, %f) = %f\n",
	// 	ceil, wall_y, player_yaw, (ceil + wall_y * player_yaw));
	return (ceil + (wall_y * player_yaw));
}

void			render_frame(t_doom *doom)
{
	printf("\nFRAME START\n");
	t_world	*world;
	double	*zbuffer;

	world = doom->game->world;
	zbuffer = get_zbuffer();

	ft_memset(&world->player, 0, sizeof(world->player));
	// Todo: Update these with regular inputs:
	world->player.position = vec3_div(vec3(
		doom->mdl->player.x,
		doom->mdl->player.y,
		doom->mdl->player.height), WORLD_SCALE);
	world->player.angle = deg_to_rad(doom->mdl->player.rot);
	world->player.sin = sin(world->player.angle);
	world->player.cos = cos(world->player.angle);

	flood_buffer(doom->game->buff, 0x112233);
	memset_f(zbuffer, INFINITY, GAME_WIN_WIDTH * GAME_WIN_HEIGHT);

	// Pixel coordinates for vertical drawing
	int y_top[GAME_WIN_WIDTH];
	int y_bot[GAME_WIN_WIDTH];
	memset_i(y_top,                   0, GAME_WIN_WIDTH);
	memset_i(y_bot, GAME_WIN_HEIGHT - 1, GAME_WIN_WIDTH);

	// Progressive rendering of neighboring sectors
	// int rendered_sectors[world->sector_count];
	// ft_memset(rendered_sectors, 0, sizeof(rendered_sectors));
	t_section	queue[MAX_SECTOR_QUEUE];
	t_section	*head = queue;
	t_section	*tail = queue;

	// First in queue will be the camera/player's sector.
	queue[0].id = world->player.sector_id;
	queue[0].left = 0;
	queue[0].right = GAME_WIN_WIDTH - 1;

	// Easy access to current render data (sector to draw, section to draw into)
	t_section	*section = &queue[0];
	t_sector2	*sector = &world->sectors[section->id];



	// WALL RENDER LOOP :D
	unsigned vertex = 0;
	while (vertex < sector->vertex_count)
	{
		drawline(line_xy(vec2(GAME_WIN_WIDTH-1, GAME_MIDHEIGHT-1), vec2(0,             GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);
		drawline(line_xy(vec2(GAME_MIDWIDTH,                   0), vec2(GAME_MIDWIDTH, GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);

		// Calculate relative vertex positions for one wall.
		// The world's zero-point is now considered to be the player.
		t_xy v1 = vec2_sub(sector->vertex[vertex+0], vec32(world->player.position));
		t_xy v2 = vec2_sub(sector->vertex[vertex+1], vec32(world->player.position));
		t_xy_line wall = line_xy(v1, v2, 0xffffff);
		t_xy_line debug;
		// linep("wall original: ", wall);

		// Rotate the world around the player. (counter to actual rotation)
		// The player can be thought of as always facing "up."
		wall = line_rot(wall, world->player.angle + M_PI);
		// linep("wall rotated:  ", wall);
		t_xy_line wall_preclip = wall;


		t_xy a = vec2(-GAME_MIDWIDTH, -GAME_WIN_HEIGHT);
		t_xy b = vec2( GAME_MIDWIDTH, -GAME_WIN_HEIGHT);
		t_xy c = vec2( GAME_MIDWIDTH,           -1.01);
		t_xy d = vec2(-GAME_MIDWIDTH,           -1.01);
		t_xy_line *bounds = set_clip_bounds(a, b, c, d);
		//                -GAME_MIDHEIGHT
		//                 |-----------|
		// -GAME_MIDWIDTH  |           |  GAME_MIDWIDTH
		//                 |           |
		//                 |-----0-----|
		clip_to_bounds(wall, &wall, bounds);
		// linep("wall clipped:  ", wall);


		// debug = line_add_offset(wall, vec2(GAME_MIDWIDTH, GAME_MIDHEIGHT));
		//   0----------|
		//   |          |  GAME_WIN_WIDTH
		//   |          |
		//   |----------|
		//  GAME_MIDHEIGHT
		if (line_is_zero(wall))
		{
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xFF005A;
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}
		// linep("dbg clipped:   ", debug);
		// drawline(debug, doom->game->buff);


		t_xy_line scale;

		// Move the X coordinates horizontally further away
		// from zero (center of the screen)
		// based on their Y distance.

		// (window height / Y position) = 90 degree FOV
		scale.start.x = -(GAME_WIN_HEIGHT / wall.start.y);
		scale.stop.x = -(GAME_WIN_HEIGHT / wall.stop.y);

		// Note these (and many things above) needed to be flipped. (x1 = stop, x2 = start)
		// Todo: Why is everything flipped? Is it the screen coordinates? (up: -Y)
		int x1 = GAME_MIDWIDTH + (int)(wall.start.x * scale.start.x);
		int x2 = GAME_MIDWIDTH + (int)(wall.stop.x * scale.stop.x);
		// printf("x1:%4i, x2:%4i\n", x1, x2);

		// Make sure the original_length line crosses the current render section.
		// If the points are backwards, identical, or impossible, ignore them.
		if(x1 >= x2 || x2 < section->left || x1 > section->right)
		{
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xFFA753;
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		x1 = clamp(x1, section->left, section->right);
		x2 = clamp(x2, section->left, section->right);
		printf("\nnew render section: x1:%i, x2:%i (clamped)\n", x1, x2);

		// Todo: Vertical lines!

		double ceil;
		double floor;
		ceil = sector->ceil - world->player.position.z;
		floor = sector->floor - world->player.position.z;


		t_xy planeleft = ( vec2_rot(vec2(0, -100), 135*DEG_TO_RAD) );
		t_xy planeright = ( vec2_rot(vec2(0, -100), 45*DEG_TO_RAD) );
		t_xy_line line = line_xy(vec2(0,0), planeleft, 0x00ffff);
		t_xy_line wall_segment;
		vec2_clip_line(wall, &wall_segment, line);
		line = line_xy(planeright, vec2(0,0), 0x00ffff);
		vec2_clip_line(wall_segment, &wall_segment, line);

		scale.start.y = (GAME_WIN_HEIGHT / wall_segment.start.y);
		scale.stop.y = (GAME_WIN_HEIGHT / wall_segment.stop.y);

		// Todo: account for camera yaw later.
		int yawed_start_ceil  = GAME_MIDHEIGHT - ((ceil ) * scale.start.y);
		int yawed_start_floor = GAME_MIDHEIGHT - ((floor) * scale.start.y);

		int yawed_stop_ceil   = GAME_MIDHEIGHT - ((ceil ) * scale.stop.y);
		int yawed_stop_floor  = GAME_MIDHEIGHT - ((floor) * scale.stop.y);

		// printf("\tceil:%-4.0f floor:%-4.0f || yawed_start_ceil:%-8i , yawed_stop_ceil:%-8i , yawed_start_floor:%-8i ,  yawed_stop_floor:%-8i \n",
		// 	ceil, floor, yawed_start_ceil, yawed_stop_ceil, yawed_start_floor, yawed_stop_floor);

		// Begin the confusion...
		{
			// begin/end at the shortest possible range.
			int beginx = ft_maxi(x1, section->left);
			int endx   = ft_mini(x2, section->right);
			// printf("\tvline begin%i end%i\n", beginx, endx);

			// Debug info for specific walls
			int colors[] = {0xff6666, 0x66ff66, 0x6666ff, 0x666666, 0xffffff, 0x0};
			const char *name[] = {"red", "green", "blue", "grey", "white", "black"};
			{
				printf("\tdrawing %s\n", name[vertex]);
				vertical_line(beginx, 0,              GAME_MIDHEIGHT,    colors[vertex]);
				vertical_line(endx,   GAME_MIDHEIGHT, GAME_WIN_HEIGHT-1, colors[vertex]);
			}

			int x = beginx;
			int i = 0;
			while (x < endx)
			{
				int ceil_diff = yawed_stop_ceil - yawed_start_ceil;
				int floor_diff = yawed_stop_floor - yawed_start_floor;

				double clip_length = line_mag(wall);
				double orig_length = line_mag(wall_preclip);
				double beginf = orig_length - clip_length;
				double increment = clip_length / (endx - beginx);
				int distance = x - x1;

				double kalle = (yawed_stop_ceil - yawed_start_ceil) / (double)(endx - beginx);


				double angle_calc = (i * kalle);
				int y_start = yawed_start_floor - angle_calc;
				int y_stop  = yawed_start_ceil + angle_calc;

				y_start = clamp(y_start, y_top[x], y_bot[x]);
				y_stop  = clamp(y_stop, y_top[x], y_bot[x]);
				vertical_line(x, y_start, y_stop, colors[vertex]);


				// more debugs wew
				if (colors[vertex] == 0xff6666)
				{
					// printf("SLOPE %f\n", wall_slope);
					drawline(line_xy(vec2(x1, GAME_MIDHEIGHT+5), vec2(x2, GAME_MIDHEIGHT+5), 0xffff00), doom->game->buff);
				}
				++x;
				++i;
			}
		}

		// Debug view.
		{
			debug = line_add_offset(wall, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xffffff;
			drawline(debug, doom->game->buff);
			// Proves 90 degree FOV
			t_xy center = vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100);
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90+45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90-45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(vec2_add(center, vec2(-50,0)), vec2_add(center, vec2(50,0)), 0xff0000), doom->game->buff);


			// t_xy planeleft = ( vec2_rot(vec2(0, -100), 135*DEG_TO_RAD) );
			// t_xy planeright = ( vec2_rot(vec2(0, -100), 45*DEG_TO_RAD) );

			// t_xy_line line = line_xy(vec2(0,0), planeleft, 0x00ffff);
			// drawline(line_add_offset(line, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100)), doom->game->buff);
			// wall.color = 0x00ffff;
			// drawline(line_add_offset(wall, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100)), doom->game->buff);

			// t_xy_line out;
			// out.color = 0x00ffff;
			// vec2_clip_line(wall, &out, line);
			// line = line_xy(planeright, vec2(0,0), 0x00ffff);
			// vec2_clip_line(out, &out, line);

			// draw_box(vec2_add(out.start, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100)), 3, out.color, doom->game->buff);
			// draw_box(vec2_add(out.stop, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100)), 3, out.color, doom->game->buff);
		}

		++vertex;
	}
}
