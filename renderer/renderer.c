#include "renderer.h"

//! Render the walls of a sector to a section of the screen.
void			render_sector(t_sector *sector, t_section *section, t_doom *doom, int *y_top, int *y_bot)
{
	t_xy_line debug;

	unsigned vertex;
	t_xy_line wall;

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

		//! Rotate the world around the player. (counter to actual rotation)
		// The player is considered as always facing "up."

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
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		//! Don't begin/end drawing from outside of the current render section.
		x1 = clamp(x1, section->left, section->right);
		x2 = clamp(x2, section->left, section->right);
		// printf("\nnew render section: x1:%i, x2:%i (clamped)\n", x1, x2);

		double ceil = sector->ceil - doom->game->world->player.position.z;
		double floor = sector->floor - doom->game->world->player.position.z;

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

		int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + wall_segment.start.y * doom->game->world->player.yaw) * scale.start.y;
		int yawed_start_floor = GAME_MIDHEIGHT - (floor + wall_segment.start.y * doom->game->world->player.yaw) * scale.start.y;

		int yawed_stop_ceil   = GAME_MIDHEIGHT - (ceil  + wall_segment.stop.y * doom->game->world->player.yaw) * scale.stop.y;
		int yawed_stop_floor  = GAME_MIDHEIGHT - (floor + wall_segment.stop.y * doom->game->world->player.yaw) * scale.stop.y;

		//! Begin/end at the shortest possible range within the render section.
		x1 = ft_maxi(x1, section->left);
		x2 = ft_mini(x2, section->right);

		// Debug info for specific walls
		int colors[256] = {0xff6666, 0x66ff66, 0x6666ff, 0x666666, 0xffffff, 0x0};
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