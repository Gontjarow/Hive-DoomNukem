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

unsigned		sprite_pixel(t_enemy *enemy, int x, int y)
{
	unsigned	*sprite;
	unsigned	pixel;
	unsigned	size;

	sprite = enemy->active_sprite->pixels;
	pixel = enemy->active_sprite->w * y + x;
	size = enemy->active_sprite->w * enemy->active_sprite->h;
	if ((pixel < size) && ((sprite[pixel] >> 6) != BYTE_TRANSPARENT))
		return(sprite[pixel]);
	else
		return (COLOR_TRANSPARENT);
}

void			vertical_sprite(t_enemy *enemy, int screen_x, int tex_x, t_xy range)
{
	unsigned	*pixels;
	unsigned	color;
	double		y_step;
	double		tex_y;

	tex_y = 0;
	y_step = (double)enemy->active_sprite->h / (range.y - range.x);
	if (range.x < 0)
	{
		tex_y += y_step * -range.x;
		range.x = 0;
	}
	pixels = doom_ptr()->game->buff->pixels;
	while (range.x <= range.y && range.x < GAME_WIN_HEIGHT)
	{
		color = sprite_pixel(enemy, tex_x, tex_y);
		if (color != COLOR_TRANSPARENT)
			pixels[GAME_WIN_WIDTH * (int)range.x + screen_x] = color;
		tex_y += y_step;
		range.x++;
	}
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
		doom->mdl->player.height), WORLD_SCALE);
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

	render_sector(sector, section, doom, y_top, y_bot);


	//! Render enemies.
	// TODO: Z Buffer
	signed	enemy_count = doom->mdl->enemy_count;
	t_enemy	*enemy = doom->mdl->enemy_first;
	while (~--enemy_count)
	{
		double turn_90 = world->player.angle + PI_BY_TWO;
		t_xy epos  = vec2_div(vec2(enemy->x, enemy->y), WORLD_SCALE);
		t_xy left  = vec2_add(epos, vec2(-cos(turn_90) * SPRITE_SCALE, -sin(turn_90) * SPRITE_SCALE));
		t_xy right = vec2_add(epos, vec2(+cos(turn_90) * SPRITE_SCALE, +sin(turn_90) * SPRITE_SCALE));
		left = vec2_sub(left, vec32(world->player.position));
		right = vec2_sub(right, vec32(world->player.position));

		t_xy_line eline = line_xy(left, right, 0xff00ff);
		eline = line_rot(eline, world->player.angle + M_PI);

		// draw
		if (eline.start.y <= NEAR_PLANE)
		{
			t_xy_line scale;

			//! Calculate points scaled by horizontal FOV.
			scale.start.x = GAME_WIN_HEIGHT / -eline.start.y;
			scale.stop.x = GAME_WIN_HEIGHT / -eline.stop.y;

			int left = GAME_MIDWIDTH + (int)(eline.start.x * scale.start.x);
			int right = GAME_MIDWIDTH + (int)(eline.stop.x * scale.stop.x);

			//! Ignore impossible walls.
			if(left >= right || right < 0 || left >= GAME_WIN_WIDTH)
			{
				enemy = enemy->next;
				continue;
			}

			int enemy_id = room_id_from_polymap(doom->mdl->poly_map, enemy->x, enemy->y);

			double floor = world->sectors[enemy_id].floor - world->player.position.z;
			double ceil = floor + 20; // TODO: Define enemy height in a sensible way.

			//! Calculate ceil/floor height and draw vertical lines left-to-right.
			scale.start.y = GAME_WIN_HEIGHT / -eline.start.y;
			scale.stop.y = GAME_WIN_HEIGHT / -eline.stop.y;

			int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + eline.start.y * world->player.yaw) * scale.start.y;
			int yawed_start_floor = GAME_MIDHEIGHT - (floor + eline.start.y * world->player.yaw) * scale.start.y;

			int x = left;
			if (x < 0)
				x -= x;
			while (x < right && x < GAME_WIN_WIDTH)
			{
				int tex_x = (x - left) * (double)enemy->active_sprite->w / (right - left);
				vertical_sprite(enemy, x, tex_x, vec2(yawed_start_ceil, yawed_start_floor));
				++x;
			}
		}

		t_xy_line debug;
		debug = line_add_offset(eline, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
		drawline(debug, doom->game->buff);

		enemy = enemy->next;
	}
}
