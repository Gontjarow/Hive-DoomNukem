#include "doom-nukem.h"

void			render_pickups(t_doom *doom)
{
	t_world		*world = doom->game->world;
	signed		pickup_count = doom->mdl->pickup_count;
	t_pickup	*pickup = doom->mdl->pickup_first;

	t_wdata		saved;

	while (~--pickup_count)
	{
		t_xy_line sprite = viewer_facing_wall(veci2(pickup->loc), world);

		// draw
		if (sprite.start.y > NEAR_PLANE)
		{
			pickup = pickup->next;
			continue;
		}

		saved.texture = pickup->active_sprite;

		//! Only this value is considered for scaling (simple distance).
		saved.scale.start.y = GAME_WIN_HEIGHT / -sprite.start.y;

		saved.x1 = GAME_MIDWIDTH + (sprite.start.x * saved.scale.start.y);
		saved.x2 = GAME_MIDWIDTH + (sprite.stop.x  * saved.scale.start.y);

		//! Ignore impossible.
		if (saved.x1 >= saved.x2 || saved.x2 < 0 || saved.x1 >= GAME_WIN_WIDTH)
		{
			pickup = pickup->next;
			continue;
		}

		int pickup_id = room_id_from_polymap(doom->mdl->poly_map, pickup->loc.x, pickup->loc.y);
		t_room *room = room_by_id(pickup_id);

		saved.floor.start.y = room->floor_height / WORLD_SCALE - world->player.position.z;
		saved.floor.start.y = GAME_MIDHEIGHT - (saved.floor.start.y + sprite.start.y * world->player.yaw) * saved.scale.start.y;

		saved.ceil.start.y = room->floor_height / WORLD_SCALE + EYE_HEIGHT - world->player.position.z;
		saved.ceil.start.y = GAME_MIDHEIGHT - (saved.ceil.start.y + sprite.start.y * world->player.yaw) * saved.scale.start.y;

		t_stripe screen;
		screen.x = saved.x1;
		screen.y = saved.ceil.start.y;
		screen.y1 = saved.ceil.start.y;
		screen.y2 = saved.floor.start.y;
		screen.depth = sprite.start.y + 0.1;
		screen.x_delta = saved.texture->w / (double)(saved.x2 - saved.x1);
		screen.y_delta = saved.texture->h / (double)(screen.y2 - screen.y1);
		if (room->lit)
			draw_sprite(saved, screen, set_pixel);
		else
			draw_sprite(saved, screen, set_pixel_dark);

		t_xy_line debug;
		debug = line_add_offset(sprite, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
		if (doom->game->show_info)
			drawline(debug, doom->game->buff);

		pickup = pickup->next;
	}
}