#include "doom-nukem.h"

static void	calculate_vertical_endpoints(t_wdata saved, t_stripe *screen)
{
	screen->y1 = saved.ceil.start.y + (screen->x_delta * saved.ceil_angle);
	screen->y2 = saved.floor.start.y + (screen->x_delta * saved.floor_angle);
}

static void	calculate_z_depth(t_wdata saved, t_stripe *screen)
{
	double ratio; //? What ratio?

	ratio = (double)screen->x_delta / (saved.x2 - saved.x1);
	screen->depth = saved.render.start.y + vec2_mul(screen->render_width, ratio).y;
}

static void	draw_solid_stripe(t_sector *sector, t_wdata saved, t_stripe screen);
static void	draw_portal_stripes(t_sector *sector, t_wdata saved, t_stripe screen);

void	render_wall(t_sector *sector, t_wdata saved)
{
	t_stripe screen;
	t_world	*world;

	world = get_world();

	// TODO: Is this same as (sector.right - sector.left) ?
	screen.render_width = vec2_sub(saved.render.stop, saved.render.start);
	screen.x = saved.x1;
	while (screen.x < saved.x2)
	{
		screen.x_delta = (screen.x - saved.x1);
		screen.tx = saved.clipped_start + (screen.x_delta * saved.x_step);

		calculate_vertical_endpoints(saved, &screen);
		calculate_z_depth(saved, &screen);

		screen.top = world->screen_y_top[screen.x];
		screen.bot = world->screen_y_bot[screen.x];

		if (saved.neighbor_id == NO_NEIGHBOR)
			draw_solid_stripe(sector, saved, screen);
		else
			draw_portal_stripes(sector, saved, screen);
		++screen.x;
	}
}

void	draw_solid_stripe(t_sector *sector, t_wdata saved, t_stripe screen)
{
	//! wall
	draw_vertical(saved, screen, (saved.room->lit ? set_pixel : set_pixel_dark));

	//! ceil/floor
	// if (sector->has_ceiling)
		// vertical_wall(screen.x, screen.tx, vec2(screen.top, screen.y1), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
	// vertical_wall(screen.x, screen.tx, vec2(screen.y2, screen.bot), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	//! Draw posters
	render_posters(saved, screen);
}

render_posters(t_wdata saved, t_stripe screen)
{
	t_wall		*w;
	signed		ec;
	double		h;
	t_stripe	poster;

	w = wall_by_id(saved.room->first_wall_id + saved.vertex);
	ec = 0;
	while (ec < w->effect_count)
	{
		saved.texture = w->effects[ec]->active_sprite;
		h = w->effects[ec]->target.x / 100.0;
		// poster within draw range
		if (w->effects[ec]->type_id == EFFECT_POSTER
			&& (h - POSTER_HALF) <= screen.tx && screen.tx <= (h + POSTER_HALF))
		{
			poster = screen;
			poster.tx = (screen.tx - (h - POSTER_HALF))
				/ ((h + POSTER_HALF) - (h - POSTER_HALF));
			draw_vertical(saved, poster, (saved.room->lit ? set_pixel : set_pixel_dark));
		}
		++ec;
	}
}

void	draw_portal_stripes(t_sector *sector, t_wdata saved, t_stripe screen)
{
	t_world		*world;
	t_stripe	part;
	t_wdata		copy;

	world = get_world();

	//! ceil/floor
	// if (sector->has_ceiling)
		// vertical_wall(screen.x, screen.tx, vec2(screen.top, screen.y1), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
	// vertical_wall(screen.x, screen.tx, vec2(screen.y2, screen.bot), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	calculate_yawed_wall_height(sector, saved.scale, &saved);

	calculate_wall_angles(&saved);

	signed		connecting_y_start     = saved.ceil.start.y + (screen.x_delta * saved.ceil_angle);
	signed		connecting_y_stop      = saved.floor.start.y + (screen.x_delta * saved.floor_angle);

	//! top
	part = screen;
	part.y2 = connecting_y_start;
	draw_vertical(saved, part, (saved.room->lit ? set_pixel : set_pixel_dark));

	//door
	t_wall *w = wall_by_id(saved.room->first_wall_id + saved.vertex);
	w = portal_by_wall(w);
	if (w->portal_type == DOOR_PORTAL)
	{
		copy = saved;
		copy.texture = w->active_sprite;
		part = screen;
		part.y1 = connecting_y_start;
		part.y2 = connecting_y_stop;
		draw_vertical(copy, part, (saved.room->lit ? set_pixel : set_pixel_dark));
	}

	//! bottom
	part = screen;
	part.y1 = connecting_y_stop;
	draw_vertical(saved, part, (saved.room->lit ? set_pixel : set_pixel_dark));

	if (connecting_y_start > screen.top) world->screen_y_top[screen.x] = connecting_y_start;
	if (connecting_y_stop  < screen.bot) world->screen_y_bot[screen.x] = connecting_y_stop;
}
