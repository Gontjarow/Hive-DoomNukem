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
	vertical_wall(screen.x, screen.tx, vec2(screen.y1, screen.y2), saved.texture, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	//! ceil/floor
	if (sector->has_ceiling)
		vertical_wall(screen.x, screen.tx, vec2(screen.top, screen.y1), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
	vertical_wall(screen.x, screen.tx, vec2(screen.y2, screen.bot), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	//! Draw posters
	t_wall *w = wall_by_id(saved.room->first_wall_id + saved.vertex);
	int ec = 0;
	while (ec < w->effect_count)
	{
		t_effect *e = w->effects[ec];
		SDL_Surface *tex = e->active_sprite;
		double h = e->target.x / 100.0;
		// within draw range
		if ((h - POSTER_HALF) <= screen.tx && screen.tx <= (h + POSTER_HALF))
		{
			// value ÷ range = (0, 1)
			double poster_x = (screen.tx - (h - POSTER_HALF)) / ((h + POSTER_HALF) - (h - POSTER_HALF));
			vertical_wall(screen.x, poster_x, vec2(screen.y1, screen.y2), tex, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
		}
		++ec;
	}
}

void	draw_portal_stripes(t_sector *sector, t_wdata saved, t_stripe screen)
{
	t_world	*world;

	world = get_world();

	if (sector->has_ceiling)
		vertical_wall(screen.x, screen.tx, vec2(screen.top, screen.y1), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
	vertical_wall(screen.x, screen.tx, vec2(screen.y2, screen.bot), get_border_tex(doom_ptr()), screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	double		connecting_ceil        = saved.connecting->ceil - world->player.position.z;
	t_xy_line	connecting_yawed_ceil  = calculate_yawed(connecting_ceil, saved.render, saved.scale, world->player.yaw);

	double		connecting_floor       = saved.connecting->floor - world->player.position.z;
	t_xy_line	connecting_yawed_floor = calculate_yawed(connecting_floor, saved.render, saved.scale, world->player.yaw);

	double		connecting_ceil_angle  = (connecting_yawed_ceil.stop.y - connecting_yawed_ceil.start.y) / (double)(saved.x2 - saved.x1);
	double		connecting_floor_angle = (connecting_yawed_floor.stop.y - connecting_yawed_floor.start.y) / (double)(saved.x2 - saved.x1);

	signed		connecting_y_start     = connecting_yawed_ceil.start.y + (screen.x_delta * connecting_ceil_angle);
	signed		connecting_y_stop      = connecting_yawed_floor.start.y + (screen.x_delta * connecting_floor_angle);

	// top
	vertical_wall(screen.x, screen.tx, vec2(screen.y1, connecting_y_start), saved.texture, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
	//door
	t_wall *w = wall_by_id(saved.room->first_wall_id + saved.vertex);
	w = portal_by_wall(w);
	if (w->portal_type == DOOR_PORTAL)
	{
		vertical_wall(screen.x, screen.tx, vec2(connecting_y_start, connecting_y_stop), w->active_sprite, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));
	}
	//bottom
	vertical_wall(screen.x, screen.tx, vec2(connecting_y_stop, screen.y2), saved.texture, screen.depth, (saved.room->lit ? set_pixel : set_pixel_dark));

	if (connecting_y_start > screen.top) world->screen_y_top[screen.x] = connecting_y_start;
	if (connecting_y_stop  < screen.bot) world->screen_y_bot[screen.x] = connecting_y_stop;
}
