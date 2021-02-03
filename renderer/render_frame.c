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

static void		print_data(t_world *world)
{
	unsigned sectors = world->sector_count;
	unsigned si = 0;
	while (si < sectors)
	{
		t_sector sector = world->sectors[si];
		unsigned verts = sector.vertex_count;
		//printf("world->sectors[%3d]->vertex_count: %u\n", si, verts);
		unsigned vi = 0;
		while (vi < verts)
		{
			//printf("\tvertex[%3d] x,y | %4.0f, %4.0f\n",
				//vi, sector.vertex[vi].x, sector.vertex[vi].y);
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
		world->sectors = malloc(mdl->room_count * sizeof(t_sector));
		ft_bzero(world->sectors, mdl->room_count * sizeof(t_sector));
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
        t_sector *sector = &world->sectors[room_index];

        // Sector init
        sector->vertex_count = room->wall_count; // +1?
        sector->floor = 10;
        sector->ceil = 15; //"60"

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
	ft_memset(&world->player, 0, sizeof(world->player));
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
	// printf("\nFRAME START\n");
	t_world	*world;
	double	*zbuffer;

	world = doom->game->world;
	zbuffer = get_zbuffer();

	//! Convert model values to useful camera values.
	world->player.position = vec3_div(vec3(
		doom->mdl->player.x,
		doom->mdl->player.y,
		doom->mdl->player.z), WORLD_SCALE);
	world->player.angle = doom->mdl->player.rot * DEG_TO_RAD;
	world->player.sin = sin(world->player.angle);
	world->player.cos = cos(world->player.angle);
	world->player.yaw = clamp(doom->mdl->player.yaw, -M_PI/2, M_PI/2);

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
	t_sector	*sector = &world->sectors[section->id];

	//! Render the walls of a room.
	unsigned vertex = 0;
	while (vertex < sector->vertex_count)
	{
		//! Calculate relative vertex positions for one wall.
		// The world's zero-point is considered to be the player.
		t_xy v1 = vec2_sub(sector->vertex[vertex+0], vec32(world->player.position));
		t_xy v2 = vec2_sub(sector->vertex[vertex+1], vec32(world->player.position));
		t_xy_line wall = line_xy(v1, v2, 0xffffff);
		t_xy_line debug;

		//! Rotate the world around the player. (counter to actual rotation)
		// The player is considered as always facing "up."
		wall = line_rot(wall, world->player.angle + M_PI);
		t_xy_line wall_preclip = wall;

		t_xy a = vec2(-GAME_MIDWIDTH, -GAME_WIN_HEIGHT);
		t_xy b = vec2( GAME_MIDWIDTH, -GAME_WIN_HEIGHT);
		t_xy c = vec2( GAME_MIDWIDTH,           -1.01);
		t_xy d = vec2(-GAME_MIDWIDTH,           -1.01);
		t_xy_line *bounds = set_clip_bounds(a, b, c, d);

		clip_to_bounds(wall, &wall, bounds);

		//! Ignore zero-length walls.
		if (line_is_zero(wall))
		{
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xFF005A;
			if (doom->game->show_info)
				drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		t_xy_line scale;

		//! Calculate points scaled by horizontal FOV.
		// (window height / Y position) = 90 degree FOV
		scale.start.x = GAME_WIN_HEIGHT / -wall.start.y;
		scale.stop.x = GAME_WIN_HEIGHT / -wall.stop.y;

		int x1 = GAME_MIDWIDTH + (int)(wall.start.x * scale.start.x);
		int x2 = GAME_MIDWIDTH + (int)(wall.stop.x * scale.stop.x);

		//! Ignore impossible walls, or walls that are backwards.
		if(x1 >= x2 || x2 < section->left || x1 > section->right)
		{
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xFFA753;
			if (doom->game->show_info)
				drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		//! Don't begin/end drawing from outside of the current render section.
		x1 = clamp(x1, section->left, section->right);
		x2 = clamp(x2, section->left, section->right);
		// printf("\nnew render section: x1:%i, x2:%i (clamped)\n", x1, x2);

		double ceil = sector->ceil - world->player.position.z;
		double floor = sector->floor - world->player.position.z;

		//! One more clip into the player's view-cone.
		t_xy_line wall_segment;

		t_xy planeleft = vec2_rot(vec2(0, -100), 135*DEG_TO_RAD);
		t_xy_line line = line_xy(vec2(0,0), planeleft, 0x00ffff);
		vec2_clip_line(wall, &wall_segment, line);

		t_xy planeright = vec2_rot(vec2(0, -100), 45*DEG_TO_RAD);
		line = line_xy(planeright, vec2(0,0), 0x00ffff);
		vec2_clip_line(wall_segment, &wall_segment, line);

		//! Calculate ceil/floor height and draw vertical lines left-to-right.
		scale.start.y = GAME_WIN_HEIGHT / -wall_segment.start.y;
		scale.stop.y = GAME_WIN_HEIGHT / -wall_segment.stop.y;

		int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + wall_segment.start.y * world->player.yaw) * scale.start.y;
		int yawed_start_floor = GAME_MIDHEIGHT - (floor + wall_segment.start.y * world->player.yaw) * scale.start.y;

		int yawed_stop_ceil   = GAME_MIDHEIGHT - (ceil  + wall_segment.stop.y * world->player.yaw) * scale.stop.y;
		int yawed_stop_floor  = GAME_MIDHEIGHT - (floor + wall_segment.stop.y * world->player.yaw) * scale.stop.y;

		//! Begin/end at the shortest possible range within the render section.
		x1 = ft_maxi(x1, section->left);
		x2 = ft_mini(x2, section->right);

		// Debug info for specific walls
		int colors[256] = {0xff6666, 0x66ff66, 0x6666ff, 0x666666, 0xffffff, 0x330033, 0x003300, 0x333300, 0x000033};
		if (doom->game->show_info)
		{
			vertical_line(x1,              0, GAME_MIDHEIGHT,    colors[vertex]);
			vertical_line(x2, GAME_MIDHEIGHT, GAME_WIN_HEIGHT-1, colors[vertex]);
		}

		double ceil_angle = (yawed_stop_ceil - yawed_start_ceil) / (double)(x2 - x1);
		double floor_angle = (yawed_stop_floor - yawed_start_floor) / (double)(x2 - x1);

		int x = x1;
		while (x < x2)
		{
			int y_start = yawed_start_ceil + ((x - x1) * ceil_angle);
			int y_stop = yawed_start_floor + ((x - x1) * floor_angle);

			y_start = clamp(y_start, y_top[x], y_bot[x]);
			y_stop  = clamp(y_stop, y_top[x], y_bot[x]);
			vertical_line(x, y_start, y_stop, colors[vertex]);

			++x;
		}

		//! Debug view.
		if (doom->game->show_info)
		{
			drawline(line_xy(vec2(GAME_WIN_WIDTH-1, GAME_MIDHEIGHT-1), vec2(0,             GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);
			drawline(line_xy(vec2(GAME_MIDWIDTH,                   0), vec2(GAME_MIDWIDTH, GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);

			debug = line_add_offset(wall_segment, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xffffff;
			drawline(debug, doom->game->buff);
			draw_box(vec2_add(wall_segment.start, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100)), 3, debug.color, doom->game->buff);
			draw_box(vec2_add(wall_segment.stop, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100)), 3, debug.color, doom->game->buff);

			// Proves 90 degree FOV
			t_xy center = vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100);
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90+45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90-45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(vec2_add(center, vec2(-50,0)), vec2_add(center, vec2(50,0)), 0xff0000), doom->game->buff);
		}

		++vertex;
	}

	//! Render enemies.
	int		sprite_size = 1;
	int		skip_on_start = 1;
	signed	enemy_count = doom->mdl->enemy_count;
	t_enemy	*enemy = doom->mdl->enemy_first;
	while (~--enemy_count)
	{
		if (!skip_on_start)
			enemy = enemy->next;
		skip_on_start = 0;
		if (enemy->hp.cur < 1)
			continue ;
		double turn_90 = world->player.angle + PI_BY_TWO;
		t_xy epos  = vec2_div(vec2(enemy->x, enemy->y), WORLD_SCALE);
		t_xy left  = vec2_add(epos, vec2(-cos(turn_90) * sprite_size, -sin(turn_90) * sprite_size));
		t_xy right = vec2_add(epos, vec2(+cos(turn_90) * sprite_size, +sin(turn_90) * sprite_size));
		left = vec2_sub(left, vec32(world->player.position));
		right = vec2_sub(right, vec32(world->player.position));

		t_xy_line eline = line_xy(left, right, 0xff00ff);
		eline = line_rot(eline, world->player.angle + M_PI);

		// TODO: REPLACE WITH ACTUAL SPRITE RENDERING
		// draw fake wall where the enemy is
		{
			t_xy_line scale;
			//! Calculate points scaled by horizontal FOV.
			// (window height / Y position) = 90 degree FOV
			scale.start.x = GAME_WIN_HEIGHT / -eline.start.y;
			scale.stop.x = GAME_WIN_HEIGHT / -eline.stop.y;

			int x1 = GAME_MIDWIDTH + (int)(eline.start.x * scale.start.x);
			int x2 = GAME_MIDWIDTH + (int)(eline.stop.x * scale.stop.x);

			//! Ignore impossible walls, or walls that are backwards.
			if(x1 >= x2 || x2 < section->left || x1 > section->right)
			{
				continue;
			}

			//! Don't begin/end drawing from outside of the current render section.
			x1 = clamp(x1, section->left, section->right);
			x2 = clamp(x2, section->left, section->right);

			//! Calculate ceil/floor height and draw vertical lines left-to-right.
			scale.start.y = GAME_WIN_HEIGHT / -eline.start.y;
			scale.stop.y = GAME_WIN_HEIGHT / -eline.stop.y;

			double ceil = sector->floor + 3  - world->player.position.z; //(enemy->height = "30")
			double floor = sector->floor - world->player.position.z;

			world->player.sector_id = room_id_from_polymap(get_model()->poly_map, enemy->x, enemy->y);

			int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + eline.start.y * world->player.yaw) * scale.start.y;
			int yawed_start_floor = GAME_MIDHEIGHT - (floor + eline.start.y * world->player.yaw) * scale.start.y;

			int yawed_stop_ceil   = GAME_MIDHEIGHT - (ceil  + eline.stop.y * world->player.yaw) * scale.stop.y;
			int yawed_stop_floor  = GAME_MIDHEIGHT - (floor + eline.stop.y * world->player.yaw) * scale.stop.y;

			//! Begin/end at the shortest possible range within the render section.
			x1 = ft_maxi(x1, section->left);
			x2 = ft_mini(x2, section->right);

			double ceil_angle = (yawed_stop_ceil - yawed_start_ceil) / (double)(x2 - x1);
			double floor_angle = (yawed_stop_floor - yawed_start_floor) / (double)(x2 - x1);

			int x = x1;
			while (x < x2)
			{
				int y_start = yawed_start_ceil + ((x - x1) * ceil_angle);
				int y_stop = yawed_start_floor + ((x - x1) * floor_angle);

				y_start = clamp(y_start, y_top[x], y_bot[x]);
				y_stop  = clamp(y_stop, y_top[x], y_bot[x]);
				vertical_line(x, y_start, y_stop, 0xff00ff);
				++x;
			}
		}

		t_xy_line debug = line_add_offset(eline, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
		if (doom->game->show_info)
			drawline(debug, doom->game->buff);

		//linep("eline", debug);
	}
}
