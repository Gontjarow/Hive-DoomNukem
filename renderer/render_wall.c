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
static void	render_posters(t_wdata saved, t_stripe screen);

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
	draw_vertical_column(&saved, &screen);

	//! Draw posters
	render_posters(saved, screen);
}

void	draw_portal_stripes(t_sector *sector, t_wdata saved, t_stripe screen)
{
	t_world		*world;
	t_wdata		copy;
	t_wall		*wall;

	world = get_world();

	calculate_yawed_wall_height(sector, saved.scale, &saved);
	calculate_wall_angles(&saved);
	calculate_vertical_endpoints(saved, &screen);
	narrow_drawable_area(screen.y1, screen.y2, screen.x);

	//! Door
	wall = portal_by_wall(wall_by_id(saved.room->first_wall_id + saved.vertex));
	if (wall->portal_type == DOOR_PORTAL)
	{
		copy = saved;
		copy.texture = wall->active_sprite;
		draw_vertical_column(&copy, &screen);
	}

	render_portal_connectors(saved, screen);
}

void	render_posters(t_wdata saved, t_stripe screen)
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
			draw_vertical_column(&saved, &poster);
		}
		++ec;
	}
}

void	render_portal_connectors(t_wdata saved, t_stripe screen)
{
	t_wdata		copy;
	t_stripe	part;
	t_irange	y;

	copy = saved;
	calculate_yawed_wall_height(&get_world()->sectors[saved.neighbor_id], saved.scale, &copy);
	calculate_wall_angles(&copy);
	part = screen;

	//! top
	calculate_vertical_endpoints(copy, &part);
	part.y2 = part.y1;
	part.y1 = screen.y1;
	draw_vertical_column(&saved, &part);

	//! bottom
	calculate_vertical_endpoints(copy, &part);
	y.low = part.y1;
	y.high = part.y2;
	part.y1 = part.y2;
	part.y2 = screen.y2;
	draw_vertical_column(&saved, &part);
	narrow_drawable_area(y.low, y.high, screen.x);
}
