#include "doom-nukem.h"

t_xy_line	viewer_facing_wall(t_xy location, t_world *world)
{
	double	cos90;
	double	sin90;
	t_xy	pos;
	t_xy	left;
	t_xy	right;

	cos90 = cos(world->player.angle + PI_BY_TWO);
	sin90 = sin(world->player.angle + PI_BY_TWO);

	pos = vec2_div(location, WORLD_SCALE);

	left = vec2_mul(vec2(-cos90, -sin90), SPRITE_SCALE);
	left = vec2_add(pos, left);
	left = vec2_sub(left, vec32(world->player.position));

	right = vec2_mul(vec2(+cos90, +sin90), SPRITE_SCALE);
	right = vec2_add(pos, right);
	right = vec2_sub(right, vec32(world->player.position));

	return (line_rot(line_xy(left, right, 0xff00ff),
		world->player.angle + M_PI));
}

// TODO: active_sprite needs a value!
void		render_effectors(t_doom *doom)
{
	t_world		*world = doom->game->world;
	signed		effect_count = doom->mdl->effect_count;
	t_effect	*effect = doom->mdl->effect_first;

	t_wdata		saved;

	while (~--effect_count)
	{
		// if (effect->type_id == EFFECT_POSTER)
		// 	continue;

		t_xy_line sprite = viewer_facing_wall(veci2(effect->loc), world);

		if (sprite.start.y > NEAR_PLANE)
		{
			effect = effect->next;
			continue;
		}

		saved.texture = effect->active_sprite;

		//! Only this value is considered for scaling (simple distance).
		saved.scale.start.y = GAME_WIN_HEIGHT / -sprite.start.y;

		saved.x1 = GAME_MIDWIDTH + (sprite.start.x * saved.scale.start.y);
		saved.x2 = GAME_MIDWIDTH + (sprite.stop.x  * saved.scale.start.y);

		//! Ignore impossible.
		if (saved.x1 >= saved.x2 || saved.x2 < 0 || saved.x1 >= GAME_WIN_WIDTH)
		{
			effect = effect->next;
			continue;
		}

		//? room_by_wall is required because some effectors (like lightswitches)
		//? may be on the edge of of a room/sector, even outside of its sector,
		//? meaning that retrieving it by location may return -1 as the index.
		t_room *room = room_by_wall_id(effect->target_id, doom->mdl);

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

		if (doom->game->show_info)
		{
			t_xy_line debug;
			debug = line_add_offset(sprite, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			drawline(debug, doom->game->buff);
		}

		effect = effect->next;
	}
}
