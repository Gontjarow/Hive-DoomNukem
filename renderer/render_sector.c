#include "renderer.h"

t_xy_line		calculate_horizontal_scale(t_xy_line segment, t_xy_line *out)
{
	//! Calculate points scaled by horizontal FOV.
	// (window height / Y position) = 90 degree FOV
	out->start.x = GAME_WIN_HEIGHT / -segment.start.y;
	out->stop.x = GAME_WIN_HEIGHT / -segment.stop.y;
}

t_xy_line		calculate_vertical_scale(t_xy_line segment, t_xy_line *out)
{
	out->start.y = GAME_WIN_HEIGHT / -segment.start.y;
	out->stop.y = GAME_WIN_HEIGHT / -segment.stop.y;
}


//! Render the walls of a sector to a section of the screen.
void			render_sector(t_sector *sector, t_section *section, t_doom *doom, int *y_top, int *y_bot)
{
	t_xy_line debug;

	unsigned vertex;
	t_xy_line wall;
	t_xy_line scale;

	SDL_Surface *bricks = get_bricks(doom);

	const t_xy debug_view_offset = vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100);

	vertex = 0;
	while (vertex < sector->vertex_count)
	{
		//! Calculate relative vertex positions for one wall.
		// The world's zero-point is considered to be the player.

		wall = line_rot(line_relative(
				sector->vertex[vertex + 0],
				sector->vertex[vertex + 1],
				vec32(doom->game->world->player.position)),
			doom->game->world->player.angle + M_PI);

		t_xy_line wall_preclip = wall;

		clip_to_bounds(wall, &wall, set_clip_bounds(
			vec2(-GAME_MIDWIDTH, -GAME_WIN_HEIGHT),
			vec2(+GAME_MIDWIDTH, -GAME_WIN_HEIGHT),
			vec2(+GAME_MIDWIDTH, NEAR_PLANE),
			vec2(-GAME_MIDWIDTH, NEAR_PLANE)));

		//! Ignore zero-length walls.
		if (line_is_zero(wall))
		{
			debug = line_add_offset(wall_preclip, debug_view_offset);
			debug.color = 0xFF005A;
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		calculate_horizontal_scale(wall, &scale);

		int x1 = GAME_MIDWIDTH + (wall.start.x * scale.start.x);
		int x2 = GAME_MIDWIDTH + (wall.stop.x * scale.stop.x);

		//! Ignore impossible walls, or walls that are backwards.
		if(x1 >= x2 || x2 < section->left || x1 > section->right)
		{
			debug = line_add_offset(wall_preclip, debug_view_offset);
			debug.color = 0xFFA753;
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		//! Begin/end at the shortest possible range within the render section.
		x1 = max(x1, section->left);
		x2 = min(x2, section->right);

		//! One more clip into the player's view-cone.
		t_xy_line wall_segment;
		clip_to_cone(wall, &wall_segment);

		//! Calculate ceil/floor height and draw vertical lines left-to-right.
		calculate_vertical_scale(wall_segment, &scale);

		double ceil = sector->ceil - doom->game->world->player.position.z;
		t_xy_line yawed_ceil;
		yawed_ceil.start.y = GAME_MIDHEIGHT - (ceil + wall_segment.start.y * doom->game->world->player.yaw) * scale.start.y;
		yawed_ceil.stop.y = GAME_MIDHEIGHT - (ceil + wall_segment.stop.y * doom->game->world->player.yaw) * scale.stop.y;

		double floor = sector->floor - doom->game->world->player.position.z;
		t_xy_line yawed_floor;
		yawed_floor.start.y = GAME_MIDHEIGHT - (floor + wall_segment.start.y * doom->game->world->player.yaw) * scale.start.y;
		yawed_floor.stop.y = GAME_MIDHEIGHT - (floor + wall_segment.stop.y * doom->game->world->player.yaw) * scale.stop.y;


		// Debug info for specific walls
		int colors[256] = {0xff6666, 0x66ff66, 0x6666ff, 0x666666, 0xffffff, 0x0};
		{
			vertical_line(x1,              0, GAME_MIDHEIGHT,    colors[vertex]);
			vertical_line(x2, GAME_MIDHEIGHT, GAME_WIN_HEIGHT-1, colors[vertex]);
		}

		double ceil_angle = (yawed_ceil.stop.y - yawed_ceil.start.y) / (double)(x2 - x1);
		double floor_angle = (yawed_floor.stop.y - yawed_floor.start.y) / (double)(x2 - x1);

		int orig_x1 = GAME_MIDWIDTH + (wall.start.x * scale.start.x);

		// Apply perspective to the original line segment.
		t_xy_line scaled_preclip = line(
			wall_preclip.start.x * scale.start.y,
			wall_preclip.start.y * scale.start.y,
			wall_preclip.stop.x  * scale.stop.y,
			wall_preclip.stop.y  * scale.stop.y
		);

		// This wall's per-pixel ratio is based on the length
		// of the original wall segment, regardless of any clipping.
		double texture_step = bricks->w / line_mag(scaled_preclip);

		double wall_step = 1 / line_mag(scaled_preclip);

		t_xy_line start_clip = line_xy(wall_preclip.start, wall_segment.start, 0x00ffff);

		double clipped_ratio = line_mag(start_clip) / line_mag(wall_preclip);

		int x = x1;
		while (x < x2)
		{
			int horizontal = (x - x1);
			int range = (x2 - x1);
			int tex_x = ((range * clipped_ratio) * texture_step) + (horizontal * texture_step);

			int y_start = yawed_ceil.start.y + (horizontal * ceil_angle);
			int y_stop = yawed_floor.start.y + (horizontal * floor_angle);
			vertical_wall(x, tex_x, vec2(y_start, y_stop), bricks);

			++x;
		}

		//! Debug view.
		{
			drawline(line_xy(vec2(GAME_WIN_WIDTH-1, GAME_MIDHEIGHT-1), vec2(0,             GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);
			drawline(line_xy(vec2(GAME_MIDWIDTH,                   0), vec2(GAME_MIDWIDTH, GAME_MIDHEIGHT-1), 0x0000ff), doom->game->buff);

			debug = line_add_offset(wall_segment, debug_view_offset);
			debug.color = 0xffffff;
			drawline(debug, doom->game->buff);
			draw_box(vec2_add(wall_segment.start, debug_view_offset), 3, debug.color, doom->game->buff);
			draw_box(vec2_add(wall_segment.stop, debug_view_offset), 3, debug.color, doom->game->buff);

			// Proves 90 degree FOV
			t_xy center = debug_view_offset;
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90+45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(center, vec2_add(center, vec2_rot(vec2(0, -100), (90-45)*DEG_TO_RAD)), 0xff0000), doom->game->buff);
			drawline(line_xy(vec2_add(center, vec2(-50,0)), vec2_add(center, vec2(50,0)), 0xff0000), doom->game->buff);

			if (vertex == 0) drawline(line_add_offset(start_clip, debug_view_offset), doom->game->buff);

		}

		++vertex;
	}
}
