#include "renderer.h"

t_xy_line		calculate_horizontal_scale(t_xy_line segment, t_xy_line *out)
{
	//! Calculate points scaled by screen.x_delta FOV.
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

void			record_neighboring_sector(t_wdata *saved, signed sector_id)
{
	t_world	*world;

	world = get_world();
	saved->neighbor_id = sector_id;
	if (saved->neighbor_id != NO_NEIGHBOR)
		queue_add(saved->neighbor_id, saved->x1, saved->x2);
	saved->connecting = &(world->sectors[sector_id]);
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
		if (line_is_zero(wall))
		{
			if (doom->game->show_info)
				draw_debug(saved.preclip);
			++saved.vertex;
			continue;
		}

		calculate_horizontal_scale(wall, &(saved.scale));

		saved.x1 = GAME_MIDWIDTH + (wall.start.x * saved.scale.start.x);
		saved.x2 = GAME_MIDWIDTH + (wall.stop.x  * saved.scale.stop.x);

		//! Ignore impossible walls, or walls that are backwards.
		if(saved.x1 >= saved.x2 || saved.x2 < section->left || section->right < saved.x1)
		{
			if (doom->game->show_info)
				draw_debug(saved.preclip);
			++saved.vertex;
			continue;
		}

		//! Begin/end at the shortest possible range within the render section.
		horizontal_clamp(&saved.x1, &saved.x2, section);

		//! Add neighboring sector to queue if it exists.
		record_neighboring_sector(&saved, sector->neighbors[saved.vertex]);

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
		render_wall(sector, saved);

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
