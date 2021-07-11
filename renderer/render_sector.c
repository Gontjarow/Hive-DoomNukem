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
	t_world	*world = get_world();

	saved->neighbor_id = sector_id;
	if (saved->neighbor_id != NO_NEIGHBOR)
		queue_add(saved->neighbor_id, saved->x1, saved->x2);
	saved->connecting = &(world->sectors[sector_id]);
}

void			calculate_yawed_wall_height(t_sector *sector, t_xy_line scale, t_wdata *saved)
{
	t_world	*world = get_world();
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

	// TODO: clipped_ratio may be the cause of some distortions...
	saved->clipped_ratio = line_mag(saved->start_clip) / line_mag(saved->preclip);
	saved->visible_ratio = line_mag(saved->render) / line_mag(saved->preclip);
	saved->h_step = saved->visible_ratio / (saved->x2 - saved->x1);
}
//? }

//! Render the walls of a sector to a section of the screen.
void			render_sector(t_sector *sector, t_section *section, t_doom *doom)
{
	t_world		*world = get_world();
	t_wdata		saved;
	t_xy_line	debug;

	unsigned vertex;
	t_xy_line wall;
	t_xy_line scale;

	// KRUSTHOL DISABLED: SDL_Surface *texture = get_bricks_tex(doom);
	SDL_Surface *texture;
	SDL_Surface *border = get_border_tex(doom);

	const t_xy debug_view_offset = vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100);

	vertex = 0;
	while (vertex < sector->vertex_count)
	{
		texture = sector->textures[vertex];

		//! Calculate relative vertex positions for one wall.
		// The world's zero-point is considered to be the player.
		wall = player_relative(&sector->vertex[vertex], world->player);
		saved.preclip = wall;
		clip_to_bounds(wall, &wall, world->player.bounds);

		//! Ignore zero-length walls.
		if (doom->game->show_info && line_is_zero(wall))
		{
			draw_debug(saved.preclip);
			++vertex;
			continue;
		}

		calculate_horizontal_scale(wall, &scale);

		saved.x1 = GAME_MIDWIDTH + (wall.start.x * scale.start.x);
		saved.x2 = GAME_MIDWIDTH + (wall.stop.x  * scale.stop.x);

		//! Ignore impossible walls, or walls that are backwards.
		if(saved.x1 >= saved.x2 || saved.x2 < section->left || section->right < saved.x1)
		{
			draw_debug(saved.preclip);
			++vertex;
			continue;
		}

		//! Begin/end at the shortest possible range within the render section.
		horizontal_clamp(&saved.x1, &saved.x2, section);

		//! Add neighboring sector to queue if it exists.
		record_neighboring_sector(&saved, sector->neighbors[vertex]);

		//! One more clip into the player's view-cone.
		clip_to_cone(wall, &saved.render);

		//! Calculate ceil/floor height.
		calculate_vertical_scale(saved.render, &scale);
		calculate_yawed_wall_height(sector, scale, &saved);

		//! Calculate wall angle/length.
		calculate_wall_angles(&saved);

		//! Calculate clipped/visible ratios relative to preclip.
		calculate_ratios(&saved);

		//! Draw vertical lines left-to-right.
		int screen_x = saved.x1;
		while (screen_x < saved.x2)
		{
			signed	horizontal = (screen_x - saved.x1);
			double	tex_x = saved.clipped_ratio + (horizontal * saved.h_step);

			signed	y_start = saved.ceil.start.y + (horizontal * saved.ceil_angle);
			signed	y_stop = saved.floor.start.y + (horizontal * saved.floor_angle);

			t_xy len = vec2_sub(saved.render.stop, saved.render.start);
			double depth = saved.render.start.y + vec2_mul(len, (double)horizontal / (saved.x2 - saved.x1)).y;

			int top = world->screen_y_top[screen_x];
			int bot = world->screen_y_bot[screen_x];

			t_room *r = room_by_id(sector->room_id);
			//! Draw wall
			if (saved.neighbor_id == NO_NEIGHBOR)
			{
				vertical_wall(screen_x, tex_x, vec2(y_start, y_stop), texture, depth, (r->lit ? set_pixel : set_pixel_dark));

				//! wall ceil floor
				if (sector->has_ceiling)
					vertical_wall(screen_x, tex_x, vec2(top, y_start), border, -INFINITY, (r->lit ? set_pixel : set_pixel_dark));
				vertical_wall(screen_x, tex_x, vec2(y_stop, bot), border, -INFINITY, (r->lit ? set_pixel : set_pixel_dark));

				//! Draw posters
				t_wall *w = wall_by_id(r->first_wall_id + vertex);
				int i = 0;
				// printf("wall %i effects %i\n", vertex, w->effect_count);
				while (i < w->effect_count)
				{
					t_effect *e = w->effects[i];
					SDL_Surface *tex = e->active_sprite;
					// printf("effect found %i\n", e->type_id);
					double h = e->target.x / 100.0;
					// within draw range
					if ((h - POSTER_HALF) <= tex_x && tex_x <= (h + POSTER_HALF))
					{
						// value ÷ range = (0, 1)
						double poster_x = (tex_x - (h - POSTER_HALF)) / ((h + POSTER_HALF) - (h - POSTER_HALF));
						vertical_wall(screen_x, poster_x, vec2(y_start, y_stop), tex, depth, (r->lit ? set_pixel : set_pixel_dark));
					}
					++i;
				}
			}
			else
			{
				//! portal ceil floor
				if (sector->has_ceiling)
					vertical_wall(screen_x, tex_x, vec2(top, y_start), border, -INFINITY, (r->lit ? set_pixel : set_pixel_dark));
				vertical_wall(screen_x, tex_x, vec2(y_stop, bot), border, -INFINITY, (r->lit ? set_pixel : set_pixel_dark));

				double		connecting_ceil        = saved.connecting->ceil - world->player.position.z;
				t_xy_line	connecting_yawed_ceil  = calculate_yawed(connecting_ceil, saved.render, scale, world->player.yaw);

				double		connecting_floor       = saved.connecting->floor - world->player.position.z;
				t_xy_line	connecting_yawed_floor = calculate_yawed(connecting_floor, saved.render, scale, world->player.yaw);

				double		connecting_ceil_angle  = (connecting_yawed_ceil.stop.y - connecting_yawed_ceil.start.y) / (double)(saved.x2 - saved.x1);
				double		connecting_floor_angle = (connecting_yawed_floor.stop.y - connecting_yawed_floor.start.y) / (double)(saved.x2 - saved.x1);

				signed		connecting_y_start     = connecting_yawed_ceil.start.y + (horizontal * connecting_ceil_angle);
				signed		connecting_y_stop      = connecting_yawed_floor.start.y + (horizontal * connecting_floor_angle);

				vertical_wall(screen_x, tex_x, vec2(y_start, connecting_y_start), texture, depth, (r->lit ? set_pixel : set_pixel_dark));
				vertical_wall(screen_x, tex_x, vec2(connecting_y_stop, y_stop), texture, depth, (r->lit ? set_pixel : set_pixel_dark));

				if (connecting_y_start > top) world->screen_y_top[screen_x] = connecting_y_start;
				if (connecting_y_stop  < bot) world->screen_y_bot[screen_x] = connecting_y_stop;
			}
			++screen_x;
		}


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

			// if (vertex == 0) drawline(line_add_offset(saved.start_clip, debug_view_offset), doom->game->buff);

		}

		++vertex;
	}
}
