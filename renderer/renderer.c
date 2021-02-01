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

void draw_vertical_wall(t_vertical_wall wall)
{
	int x;
	int y_start;
	int y_stop;

	x = wall.section.left;

	// TODO: Smuggle some pointers all the way here.
	// TODO: Or return the wall back to the upper loop.
	while (x < wall.section.right)
	{
		y_start = wall.yawed_ceil.start.y + ((x - wall.section.left) * wall.ceil_angle);
		y_stop = wall.yawed_floor.start.y + ((x - wall.section.left) * wall.floor_angle);

		// y_start = clamp(y_start, y_top[x], y_bot[x]);
		// y_stop  = clamp(y_stop, y_top[x], y_bot[x]);
		// vertical_line(x, y_start, y_stop, colors[vertex]);
		y_start = clamp(y_start, 0, GAME_WIN_HEIGHT-1);
		y_stop  = clamp(y_stop, 0, GAME_WIN_HEIGHT-1);
		vertical_line(x, y_start, y_stop, 0xffffff);

		++x;
	}
}

void	calculate_yawed_wall(t_vertical_wall in, t_vertical_wall *out, t_sector *sector, t_camera player)
{
	double ceil;
	double floor;
	double start_yaw;
	double stop_yaw;
	double section_length;

	start_yaw = in.segment.start.y * player.yaw;
	stop_yaw = in.segment.stop.y * player.yaw;
	section_length = in.section.right - in.section.left;

	ceil = sector->ceil - player.position.z;
	out->yawed_ceil.start.y = GAME_MIDHEIGHT - (ceil + start_yaw) * in.scale.start.y;
	out->yawed_ceil.stop.y = GAME_MIDHEIGHT - (ceil + stop_yaw) * in.scale.stop.y;

	floor = sector->floor - player.position.z;
	out->yawed_floor.start.y = GAME_MIDHEIGHT - (floor + start_yaw) * in.scale.start.y;
	out->yawed_floor.stop.y = GAME_MIDHEIGHT - (floor + stop_yaw) * in.scale.stop.y;

	out->ceil_angle = (out->yawed_ceil.stop.y - out->yawed_ceil.start.y) / section_length;
	out->floor_angle = (out->yawed_floor.stop.y - out->yawed_floor.start.y) / section_length;
}

//! Render the walls of a sector to a section of the screen.
void			render_sector(t_sector *sector, t_section *section, t_doom *doom)
{
	t_xy_line		debug;

	unsigned		vertex;
	t_vertical_wall	wall;

	vertex = 0;
	while (vertex < sector->vertex_count)
	{
		//! Calculate relative (rotated) vertex positions for one wall.
		// The world's zero-point is considered to be the player.
		// The player is considered to be facing up towards -Y.
		wall.segment = line_rot(line_relative(
				sector->vertex[vertex + 0],
				sector->vertex[vertex + 1],
				vec32(doom->game->world->player.position)),
			doom->game->world->player.angle + M_PI);

		t_xy_line wall_preclip = wall.segment;

		clip_to_bounds(wall.segment, &wall.segment, set_clip_bounds(
			vec2(-GAME_MIDWIDTH, -GAME_WIN_HEIGHT),
			vec2(GAME_MIDWIDTH, -GAME_WIN_HEIGHT),
			vec2(GAME_MIDWIDTH, NEAR_PLANE),
			vec2(-GAME_MIDWIDTH, NEAR_PLANE)));

		//! Ignore zero-length walls.
		if (line_is_zero(wall.segment))
		{
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xFF005A;
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		calculate_horizontal_scale(wall.segment, &wall.scale);

		wall.section.left = GAME_MIDWIDTH + (wall.segment.start.x * wall.scale.start.x);
		wall.section.right = GAME_MIDWIDTH + (wall.segment.stop.x * wall.scale.stop.x);

		//! Ignore impossible walls.
		if(wall.section.left >= wall.section.right
		|| wall.section.right < section->left
		|| wall.section.left > section->right)
		{
			debug = line_add_offset(wall_preclip, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			debug.color = 0xFFA753;
			drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		//! Begin/end at the shortest possible range within the render section.
		wall.section.left = ft_maxi(wall.section.left, section->left);
		wall.section.right = ft_mini(wall.section.right, section->right);

		// Debug info for specific walls
		int colors[256] = {0xff6666, 0x66ff66, 0x6666ff, 0x666666, 0xffffff, 0x0};
		{
			vertical_line(wall.section.left,               0, GAME_MIDHEIGHT,    colors[vertex]);
			vertical_line(wall.section.right, GAME_MIDHEIGHT, GAME_WIN_HEIGHT-1, colors[vertex]);
		}

		//! One more clip into the player's view-cone.
		t_xy_line wall_segment;
		clip_to_cone(wall.segment, &wall_segment);

		//! Calculate ceil/floor height and draw vertical lines left-to-right.
		calculate_vertical_scale(wall_segment, &wall.scale);

		calculate_yawed_wall(wall, &wall, sector, doom->game->world->player);

		draw_vertical_wall(wall); // TODO: Find a place for this poor sod.

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
