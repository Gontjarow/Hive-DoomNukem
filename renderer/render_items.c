#include "doom-nukem.h"

t_xy_line	viewer_facing_wall(t_effect *effect, t_world *world)
{
	double	cos90;
	double	sin90;
	t_xy	pos;
	t_xy	left;
	t_xy	right;

	cos90 = cos(world->player.angle + PI_BY_TWO);
	sin90 = sin(world->player.angle + PI_BY_TWO);

	pos = vec2_div(vec2(effect->loc.x, effect->loc.y), WORLD_SCALE);

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
		if (effect->type_id == EFFECT_POSTER)
			continue;

		t_xy_line sprite = viewer_facing_wall(effect, world);

		if (sprite.start.y > NEAR_PLANE)
		{
			effect = effect->next;
			continue;
		}

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

		double floor = room->floor_height / WORLD_SCALE - world->player.position.z;
		floor = GAME_MIDHEIGHT - (floor + sprite.start.y * world->player.yaw) * saved.scale.start.y;

		double ceil  = room->floor_height / WORLD_SCALE + EYE_HEIGHT - world->player.position.z;
		ceil  = GAME_MIDHEIGHT - (ceil  + sprite.start.y * world->player.yaw) * saved.scale.start.y;

		int x = saved.x1;
		if (x < 0)
			x -= x;

		int	length = (saved.x2 - saved.x1);
		while (x < saved.x2 && x < GAME_WIN_WIDTH)
		{
			int tx = (x - saved.x1) * effect->active_sprite->w / length;
			vertical_sprite(effect->active_sprite, x, tx, vec2(ceil, floor), sprite.start.y);
			++x;
		}

		if (doom->game->show_info)
		{
			t_xy_line debug;
			debug = line_add_offset(sprite, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
			drawline(debug, doom->game->buff);
		}

		effect = effect->next;
	}
}
