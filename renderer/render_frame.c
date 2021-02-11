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
	static t_world	*world = NULL;
	t_model			*mdl;

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
	}
	return (world);
}

// Portal related code

static int		room_id_by_wall(t_wall *wall, t_model *mdl)
{
	t_room	*room;
	int		rc;

	room = mdl->room_first;
	rc = mdl->room_count;
	while (rc--)
	{
		if ((room->first_wall_id <= wall->id)
		&& (room->first_wall_id + room->wall_count >= wall->id))
			return (room->id);
		room = room->next;
	}
	ft_die("Fatal error: Could not determine room_id_by_wall.");
	return (-1);
}

// Find a wall whose start/end match the given portal.
static t_wall	*match_connecting_wall(t_model *mdl, t_wall *portal)
{
	t_wall	*wall;
	int 	wc;

	wall = mdl->wall_first;
	wc = mdl->wall_count;
	while (wc--)
	{
		if ((wall->start.x == portal->start.x && wall->start.y == portal->start.y)
		&& (wall->end.x == portal->end.x && wall->end.y == portal->end.y))
			return (wall);
		wall = wall->next;
	}
}

// Find a wall whose start/end match the given portal,
// but check opposite direction as well because the other room may be in a different order.
static t_wall	*match_original_wall(t_model *mdl, t_wall *portal, t_wall *connecting)
{
	t_wall	*wall;
	int 	wc;

	wall = mdl->wall_first;
	wc = mdl->wall_count;
	while (wc--)
	{
		if ((wall->start.x == portal->start.x && wall->start.y == portal->start.y)
		&& (wall->end.x == portal->end.x && wall->end.y == portal->end.y)
		&& (connecting != wall))
			return (wall);
		if ((wall->end.x == portal->start.x && wall->end.y == portal->start.y)
		&& (wall->start.x == portal->end.x && wall->start.y == portal->end.y)
		&& (connecting != wall))
			return (wall);
		wall = wall->next;
	}
}

static void		spawn_neighbors(t_world *world, t_model *mdl, t_wall *portal)
{
	t_sector	*sect;
	t_wall		*original;
	t_wall		*connecting;
	int			room_a;
	int			room_b;

	connecting = match_connecting_wall(mdl, portal);
	original = match_original_wall(mdl, portal, connecting);
	room_a = room_id_by_wall(connecting, mdl);
	room_b = room_id_by_wall(original, mdl);

	int sect_wall; // Connect two neighbours, from a to b and b to a

	sect_wall = connecting->id - room_by_id(room_a)->first_wall_id;
	sect = &world->sectors[room_a];
	sect->neighbors[sect_wall] = room_b;

	sect_wall = original->id - room_by_id(room_b)->first_wall_id;
	sect = &world->sectors[room_b];
	sect->neighbors[sect_wall] = room_a;
}

static void		portals_to_neighbors(t_world *world, t_model *mdl)
{
	t_wall	*portal;
	int		pc;

	portal = mdl->portal_first;
	pc = mdl->portal_count;
	while (pc--)
	{
		spawn_neighbors(world, mdl, portal);
		portal = portal->next;
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
        sector->vertex_count = room->wall_count;
        sector->floor = room->floor_height;
        sector->ceil = room->roof_height;

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
            sector->neighbors[vertex++] = NO_NEIGHBOR;
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
	portals_to_neighbors(world, mdl);
    print_data(world);
    return (world);
}

void			render_frame(t_doom *doom)
{
	printf("\nFRAME START\n");
	t_world	*world;
	double	*zbuffer;

	world = doom->game->world;
	zbuffer = get_zbuffer();

	//! Convert model values to useful camera values.
	world->player.position = vec3_div(vec3(
		doom->mdl->player.x,
		doom->mdl->player.y,
		0), WORLD_SCALE);
	world->player.sector_id = room_id_from_polymap(doom->mdl->poly_map, doom->mdl->player.x, doom->mdl->player.y);
	world->player.position.z = world->sectors[world->player.sector_id].floor;

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

	// First in queue will be the camera/player's sector.
	// queue[0].id = world->player.sector_id;
	queue[0].id = 0;
	queue[0].left = 0;
	queue[0].right = GAME_WIN_WIDTH - 1;

	queue[1].id = 1;
	queue[1].left = 0;
	queue[1].right = GAME_WIN_WIDTH - 1;

	// Note: Always start from queue+0
	// Note: Whenever a new sector needs to be rendered,
	//       assign to *end, then ++end
	// Note: If first == queue+MAX_SECTOR_QUEUE
	//       or end == queue+MAX_SECTOR_QUEUE
	//       set to 0 (circle back to beginning)
	t_section	*first = queue+0;
	t_section	*end   = queue+2;

	// todo: queue loop
	while (first != end)
	{
		// Easy access to current render data (sector to draw, section to draw into)
		t_section	*section = first;
		t_sector	*sector = &world->sectors[section->id];

		render_sector(sector, section, doom, y_top, y_bot);
		++first;
	}

	// enemies don't have a "current sector" so they must be drawn together.
	render_enemies(doom);
}
