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


t_xy_line		calculate_yawed(double height, t_xy_line wall, t_xy_line scale, double yaw)
{
	t_xy_line out;

	out.start.y = GAME_MIDHEIGHT - (height + wall.start.y * yaw) * scale.start.y;
	out.stop.y = GAME_MIDHEIGHT - (height + wall.stop.y * yaw) * scale.stop.y;
	return (out);
}


//! Render the walls of a sector to a section of the screen.
void			render_sector(t_sector *sector, t_section *section, t_doom *doom)
{
	t_xy_line debug;

	unsigned vertex;
	t_xy_line wall;
	t_xy_line scale;

	SDL_Surface *bricks = get_bricks_tex(doom);
	SDL_Surface *border = get_border_tex(doom);

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
			if (doom->game->show_info)
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
			if (doom->game->show_info)
				drawline(debug, doom->game->buff);
			++vertex;
			continue;
		}

		//! Begin/end at the shortest possible range within the render section.
		x1 = max(x1, section->left);
		x2 = min(x2, section->right);

		int neighbor = sector->neighbors[vertex];
		if (neighbor != NO_NEIGHBOR)
			queue_add(neighbor, x1, x2);
		t_world		*world = get_world();
		t_sector	*connecting = &(world->sectors[neighbor]);

		//! One more clip into the player's view-cone.
		t_xy_line wall_segment;
		clip_to_cone(wall, &wall_segment);

		//! Calculate ceil/floor height.
		calculate_vertical_scale(wall_segment, &scale);

		double ceil = sector->ceil - doom->game->world->player.position.z;
		t_xy_line yawed_ceil = calculate_yawed(ceil, wall_segment, scale, doom->game->world->player.yaw);

		double floor = sector->floor - doom->game->world->player.position.z;
		t_xy_line yawed_floor = calculate_yawed(floor, wall_segment, scale, doom->game->world->player.yaw);


		// Debug info for specific walls
		int colors[256] = {0xff6666, 0x66ff66, 0x6666ff, 0x666666, 0xffffff, 0x0};
		if (doom->game->show_info)
		{
			vertical_line(x1,              0, GAME_MIDHEIGHT,    colors[vertex]);
			vertical_line(x2, GAME_MIDHEIGHT, GAME_WIN_HEIGHT-1, colors[vertex]);
		}


		//! Calculate wall angle/length.
		double ceil_angle = (yawed_ceil.stop.y - yawed_ceil.start.y) / (double)(x2 - x1);
		double floor_angle = (yawed_floor.stop.y - yawed_floor.start.y) / (double)(x2 - x1);

		t_xy_line start_clip = line_xy(wall_preclip.start, wall_segment.start, 0x00ffff);

		double clipped_ratio = line_mag(start_clip) / line_mag(wall_preclip);
		double visible_ratio = line_mag(wall_segment) / line_mag(wall_preclip);

		double h_step = visible_ratio / (x2 - x1);

		// if (vertex == 0) printf("clipped %-8.10f visible %-8.10f h_step %-8.8f\n", clipped_ratio, visible_ratio, h_step);

		//! Draw vertical lines left-to-right.
		int screen_x = x1;
		while (screen_x < x2)
		{
			signed	horizontal = (screen_x - x1);
			double	tex_x = clipped_ratio + (horizontal * h_step);

			signed	y_start = yawed_ceil.start.y + (horizontal * ceil_angle);
			signed	y_stop = yawed_floor.start.y + (horizontal * floor_angle);

			t_xy len = vec2_sub(wall_segment.stop, wall_segment.start);
			double depth = wall_segment.start.y + vec2_mul(len, (double)horizontal / (x2 - x1)).y;

			// Todo: draw sky or ceiling

			//! Draw wall
			if (neighbor == NO_NEIGHBOR)
			{
				if (vertex != 1)
					vertical_wall(screen_x, tex_x, vec2(y_start, y_stop), bricks, depth);
				else
					vertical_wall(screen_x, tex_x, vec2(y_start, y_stop), border, depth);
				world->screen_y_top[screen_x] = GAME_MIDHEIGHT;
				world->screen_y_bot[screen_x] = GAME_MIDHEIGHT;
			}
			else
			{
				double		connecting_ceil        = connecting->ceil - doom->game->world->player.position.z;
				t_xy_line	connecting_yawed_ceil  = calculate_yawed(connecting_ceil, wall_segment, scale, doom->game->world->player.yaw);

				double		connecting_floor       = connecting->floor - doom->game->world->player.position.z;
				t_xy_line	connecting_yawed_floor = calculate_yawed(connecting_floor, wall_segment, scale, doom->game->world->player.yaw);

				double		connecting_ceil_angle  = (connecting_yawed_ceil.stop.y - connecting_yawed_ceil.start.y) / (double)(x2 - x1);
				double		connecting_floor_angle = (connecting_yawed_floor.stop.y - connecting_yawed_floor.start.y) / (double)(x2 - x1);

				signed		connecting_y_start     = connecting_yawed_ceil.start.y + (horizontal * connecting_ceil_angle);
				signed		connecting_y_stop      = connecting_yawed_floor.start.y + (horizontal * connecting_floor_angle);

				int top = world->screen_y_top[screen_x];
				int bot = world->screen_y_bot[screen_x];
				connecting_y_start = clamp(connecting_y_start, top, bot);
				connecting_y_stop =  clamp(connecting_y_stop,  top, bot);

				vertical_wall(screen_x, tex_x, vec2(y_start, connecting_y_start), bricks, depth);
				vertical_wall(screen_x, tex_x, vec2(connecting_y_stop, y_stop), bricks, depth);
				world->screen_y_top[screen_x] = connecting_y_start;
				world->screen_y_bot[screen_x] = connecting_y_stop;
			}


			//! Draw floor/ground
			// if (vertex == 1 && GAME_MIDWIDTH-1 <= screen_x && screen_x <= GAME_MIDWIDTH+1)
			// if (vertex == 1)
			{
				// vertical_floor(screen_x,
				// 	vec2_add(line_lerp(wall_segment, tex_x), vec2(0, y_stop)),
				// 	vec2(y_stop, GAME_WIN_HEIGHT),
				// 	border, doom);
				vertical_line(screen_x, 0, y_start, 0x00000f);
				vertical_line(screen_x, y_stop, GAME_WIN_HEIGHT, 0x00000f);
			}

			++screen_x;
		}

		//! Debug view.
		if (doom->game->show_info)
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
