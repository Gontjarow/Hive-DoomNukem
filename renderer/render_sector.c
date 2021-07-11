#include "renderer.h"

t_xy_line		calculate_horizontal_scale(t_xy_line segment, t_xy_line *out)
{
	//! Calculate points scaled by screen.delta FOV.
	// (window height / Y position) = 90 degree FOV
	out->start.x = GAME_WIN_HEIGHT / -segment.start.y;
	out->stop.x = GAME_WIN_HEIGHT / -segment.stop.y;
}

t_xy_line		calculate_vertical_scale(t_xy_line segment, t_xy_line *out)
{
	out->start.y = GAME_WIN_HEIGHT / -segment.start.y;
	out->stop.y = GAME_WIN_HEIGHT / -segment.stop.y;
}


t_xy_line		calculate_yawed(double height, t_xy_line wall, t_xy_line scale, double yaw)
{
	t_xy_line out;

	out.start.y = GAME_MIDHEIGHT - (height + wall.start.y * yaw) * scale.start.y;
	out.stop.y = GAME_MIDHEIGHT - (height + wall.stop.y * yaw) * scale.stop.y;
	return (out);
}

//? Attempt at shortening render_sector {
t_xy_line		player_relative(t_xy *vertex, t_camera player)
{
	t_xy_line line;
	t_xy_line view;

	line = line_relative(vertex[0], vertex[1], vec32(player.position));
	view = line_rot(line, player.angle + M_PI);
	return view;
}

void			draw_debug(t_xy_line line)
{
	const t_xy	debug_view_offset = vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100);
	t_xy_line	debug;

	debug = line_add_offset(line, debug_view_offset);
	debug.color = 0xFF005A;
	drawline(debug, doom_ptr()->game->buff);
}

void			horizontal_clamp(int *x1, int *x2, t_section *section)
{
	*x1 = max(*x1, section->left);
	*x2 = min(*x2, section->right);
}

void			record_neighboring_sector(t_sector *sector, t_wdata *saved)
{
	t_world	*world;

	world = get_world();
	saved->neighbor_id = sector->neighbors[saved->vertex];
	if (saved->neighbor_id != NO_NEIGHBOR)
		queue_add(saved->neighbor_id, saved->x1, saved->x2);
	saved->connecting = &(world->sectors[saved->neighbor_id]);
}

void			calculate_yawed_wall_height(t_sector *sector, t_xy_line scale, t_wdata *saved)
{
	t_world	*world;

	world = get_world();
	double	height;

	height = sector->ceil - world->player.position.z;
	saved->ceil = calculate_yawed(height, saved->render, scale,
		world->player.yaw);

	height = sector->floor - world->player.position.z;
	saved->floor = calculate_yawed(height, saved->render, scale,
		world->player.yaw);
}

void	calculate_wall_angles(t_wdata *saved)
{
	double height;
	double width;

	width = saved->x2 - saved->x1;
	height = saved->ceil.stop.y - saved->ceil.start.y;
	saved->ceil_angle = height / width;

	width = saved->x2 - saved->x1;
	height = saved->floor.stop.y - saved->floor.start.y;
	saved->floor_angle = height / width;
}

void	calculate_ratios(t_wdata *saved)
{
	saved->start_clip = line_xy(saved->preclip.start, saved->render.start, 0x00ffff);

	// TODO: clipped_start may be the cause of some distortions?
	saved->clipped_start = line_mag(saved->start_clip) / line_mag(saved->preclip);
	saved->visible_ratio = line_mag(saved->render) / line_mag(saved->preclip);
	saved->x_step = saved->visible_ratio / (saved->x2 - saved->x1);
}

void	calculate_z_depth(t_wdata saved, t_stripe *screen)
{
	double ratio; //? What ratio?

	ratio = (double)screen->delta / (saved.x2 - saved.x1);
	screen->depth = saved.render.start.y + vec2_mul(screen->render_width, ratio).y;
}

// NOTE: Single vertical stripe.
void	draw_wall_posters(t_wdata saved, t_stripe screen)
{
	t_wall *wall = wall_by_id(saved.room->first_wall_id + saved.vertex);
	int i = 0;
	while (i < wall->effect_count)
	{
		saved.texture = wall->effects[i]->active_sprite;
		double h = wall->effects[i]->target.x / 100.0;

		// within draw range
		if ((h - POSTER_HALF) <= screen.tx && screen.tx <= (h + POSTER_HALF))
		{
			// value ÷ range = (0, 1)
			double poster_x = (screen.tx - (h - POSTER_HALF)) / ((h + POSTER_HALF) - (h - POSTER_HALF));
			vertical_wall(screen.x, poster_x, vec2(screen.y1, screen.y2), saved.texture, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
		}
		++i;
	}
}

void	draw_solid_stripe(t_sector *sector, t_wdata saved, t_stripe screen)
{
	vertical_wall(screen.x, screen.tx, vec2(screen.y1, screen.y2), saved.texture, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	draw_wall_posters(saved, screen);
}

void	draw_portal_stripes(t_sector *sector, t_wdata saved, t_stripe screen)
{
	t_world	*world;

	world = get_world();

	if (sector->has_ceiling)
		vertical_wall(screen.x, screen.tx, vec2(screen.top, screen.y1), get_border_tex(doom_ptr()), -INFINITY, (saved.room->lit ? set_pixel : set_pixel_dark));
	vertical_wall(screen.x, screen.tx, vec2(screen.y2, screen.bot), get_border_tex(doom_ptr()), -INFINITY, (saved.room->lit ? set_pixel : set_pixel_dark));

	double		connecting_ceil        = saved.connecting->ceil - world->player.position.z;
	t_xy_line	connecting_yawed_ceil  = calculate_yawed(connecting_ceil, saved.render, saved.scale, world->player.yaw);

	double		connecting_floor       = saved.connecting->floor - world->player.position.z;
	t_xy_line	connecting_yawed_floor = calculate_yawed(connecting_floor, saved.render, saved.scale, world->player.yaw);

	double		connecting_ceil_angle  = (connecting_yawed_ceil.stop.y - connecting_yawed_ceil.start.y) / (double)(saved.x2 - saved.x1);
	double		connecting_floor_angle = (connecting_yawed_floor.stop.y - connecting_yawed_floor.start.y) / (double)(saved.x2 - saved.x1);

	signed		connecting_y_start     = connecting_yawed_ceil.start.y + (screen.delta * connecting_ceil_angle);
	signed		connecting_y_stop      = connecting_yawed_floor.start.y + (screen.delta * connecting_floor_angle);

	vertical_wall(screen.x, screen.tx, vec2(screen.y1, connecting_y_start), saved.texture, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
	vertical_wall(screen.x, screen.tx, vec2(connecting_y_stop, screen.y2), saved.texture, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	if (connecting_y_start > screen.top) world->screen_y_top[screen.x] = connecting_y_start;
	if (connecting_y_stop  < screen.bot) world->screen_y_bot[screen.x] = connecting_y_stop;
}

void	calculate_vertical_endpoints(t_wdata saved, t_stripe *screen)
{
	screen->y1 = saved.ceil.start.y + (screen->delta * saved.ceil_angle);
	screen->y2 = saved.floor.start.y + (screen->delta * saved.floor_angle);
}

void	draw_single_wall(t_sector *sector, t_wdata saved)
{
	t_stripe screen;
	t_world	*world;

	world = get_world();

	// TODO: Is this same as (sector.right - sector.left) ?
	screen.render_width = vec2_sub(saved.render.stop, saved.render.start);
	screen.x = saved.x1;
	while (screen.x < saved.x2)
	{
		screen.delta = (screen.x - saved.x1);
		screen.tx = saved.clipped_start + (screen.delta * saved.x_step);

		calculate_vertical_endpoints(saved, &screen);
		calculate_z_depth(saved, &screen);

		screen.top = world->screen_y_top[screen.x];
		screen.bot = world->screen_y_bot[screen.x];

		if (saved.neighbor_id == NO_NEIGHBOR)
		{
			draw_solid_stripe(sector, saved, screen);
		}
		else
		{
			draw_portal_stripes(sector, saved, screen);
		}
		++screen.x;
	}
}
//? }

//! Render the walls of a sector to a section of the screen.
void			render_sector(t_sector *sector, t_section *section, t_doom *doom)
{
	t_world		*world = get_world();
	t_wdata		saved;
	t_xy_line	debug;

	t_xy_line wall;

	const t_xy debug_view_offset = vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100);

	saved.room = room_by_id(sector->room_id);
	saved.vertex = 0;
	while (saved.vertex < sector->vertex_count)
	{
		saved.texture = sector->textures[saved.vertex];

		//! Calculate relative vertex positions for one wall.
		// The world's zero-point is considered to be the player.
		wall = player_relative(&sector->vertex[saved.vertex], world->player);
		saved.preclip = wall;
		clip_to_bounds(wall, &wall, world->player.bounds);

		//! Ignore zero-length walls.
		if (doom->game->show_info && line_is_zero(wall))
		{
			draw_debug(saved.preclip);
			++saved.vertex;
			continue;
		}

		calculate_horizontal_scale(wall, &(saved.scale));

		saved.x1 = GAME_MIDWIDTH + (wall.start.x * saved.scale.start.x);
		saved.x2 = GAME_MIDWIDTH + (wall.stop.x  * saved.scale.stop.x);

		//! Ignore impossible walls, or walls that are backwards.
		if(doom->game->show_info && (saved.x1 >= saved.x2 || saved.x2 < section->left || section->right < saved.x1))
		{
			draw_debug(saved.preclip);
			++saved.vertex;
			continue;
		}

		//! Begin/end at the shortest possible range within the render section.
		horizontal_clamp(&saved.x1, &saved.x2, section);

		//! Add neighboring sector to queue if it exists.
		record_neighboring_sector(sector, &saved);

		//! One more clip into the player's view-cone.
		clip_to_cone(wall, &saved.render);

		//! Calculate ceil/floor height.
		calculate_vertical_scale(saved.render, &(saved.scale));
		calculate_yawed_wall_height(sector, saved.scale, &saved);

		//! Calculate wall angle/length.
		calculate_wall_angles(&saved);

		//! Calculate clipped/visible ratios relative to preclip.
		calculate_ratios(&saved);

		//! Draw vertical lines left-to-right.
		draw_single_wall(sector, saved);

		//! Debug view.
		if (doom->game->show_info)
		{
			drawline(line_xy(vec2(GAME_WIN_WIDTH-1, GAME_MIDHEIGHT-1), vec2(0,             GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);
			drawline(line_xy(vec2(GAME_MIDWIDTH,                   0), vec2(GAME_MIDWIDTH, GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);

			debug = line_add_offset(saved.render, debug_view_offset);
			debug.color = 0xffffff;
			drawline(debug, doom->game->buff);
			draw_box(vec2_add(saved.render.start, debug_view_offset), 3, debug.color, doom->game->buff);
			draw_box(vec2_add(saved.render.stop, debug_view_offset), 3, debug.color, doom->game->buff);

			// Proves 90 degree FOV
			t_xy center = debug_view_offset;
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90+45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90-45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(vec2_add(center, vec2(-50,0)), vec2_add(center, vec2(50,0)), 0xff0000), doom->game->buff);

			// if (saved.vertex == 0) drawline(line_add_offset(saved.start_clip, debug_view_offset), doom->game->buff);

		}

		++saved.vertex;
	}
}
