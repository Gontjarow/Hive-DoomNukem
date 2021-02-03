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
			vec2(GAME_MIDWIDTH, -GAME_WIN_HEIGHT),
			vec2(GAME_MIDWIDTH, NEAR_PLANE),
			vec2(-GAME_MIDWIDTH, NEAR_PLANE)));

		//! Ignore zero-length walls.
		if (line_is_zero(wall))
		{
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
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
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xFFA753;
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		//! Don't begin/end drawing from outside of the current render section.
		x1 = clamp(x1, section->left, section->right);
		x2 = clamp(x2, section->left, section->right);

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

		//! Begin/end at the shortest possible range within the render section.
		x1 = max(x1, section->left);
		x2 = min(x2, section->right);

		// Debug info for specific walls
		int colors[256] = {0xff6666, 0x66ff66, 0x6666ff, 0x666666, 0xffffff, 0x0};
		{
			vertical_line(x1,              0, GAME_MIDHEIGHT,    colors[vertex]);
			vertical_line(x2, GAME_MIDHEIGHT, GAME_WIN_HEIGHT-1, colors[vertex]);
		}

		double ceil_angle = (yawed_ceil.stop.y - yawed_ceil.start.y) / (double)(x2 - x1);
		double floor_angle = (yawed_floor.stop.y - yawed_floor.start.y) / (double)(x2 - x1);

		int x = x1;
		while (x < x2)
		{
			int y_start = yawed_ceil.start.y + ((x - x1) * ceil_angle);
			int y_stop = yawed_floor.start.y + ((x - x1) * floor_angle);

			y_start = clamp(y_start, y_top[x], y_bot[x]);
			y_stop  = clamp(y_stop, y_top[x], y_bot[x]);
			vertical_line(x, y_start, y_stop, colors[vertex]);

			++x;
		}

		//! Debug view.
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
}
