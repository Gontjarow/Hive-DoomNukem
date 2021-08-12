#include "doom-nukem.h"

t_xy_line		viewer_facing_wall(t_effect *effect, t_world *world)
{
	double cos90;
	double sin90;
	t_xy pos;
	t_xy left;
	t_xy right;

	cos90 = cos(world->player.angle + PI_BY_TWO);
	sin90 = sin(world->player.angle + PI_BY_TWO);

	pos = vec2_div(vec2(effect->loc.x, effect->loc.y), WORLD_SCALE);

	left = vec2_mul(vec2(-cos90, -sin90), SPRITE_SCALE);
	left = vec2_add(pos, left);
	left = vec2_sub(left, vec32(world->player.position));

	right = vec2_mul(vec2(+cos90, +sin90), SPRITE_SCALE);
	right = vec2_add(pos, right);
	right = vec2_sub(right, vec32(world->player.position));

	t_xy_line sprite = line_xy(left, right, 0xff00ff);
	return (line_rot(sprite, world->player.angle + M_PI));
}

// TODO: active_sprite needs a value!
void			render_effectors(t_doom *doom)
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

		if (sprite.start.y <= NEAR_PLANE)
		{
			//! Calculate points scaled by horizontal FOV.
			calculate_horizontal_scale(sprite, &(saved.scale));

			saved.x1 = GAME_MIDWIDTH + (sprite.start.x * saved.scale.start.x);
			saved.x2 = GAME_MIDWIDTH + (sprite.stop.x  * saved.scale.stop.x);

			//! Ignore impossible.
			if (saved.x1 >= saved.x2 || saved.x2 < 0 || saved.x1 >= GAME_WIN_WIDTH)
			{
				effect = effect->next;
				continue;
			}

			t_room *room = room_by_wall_id(effect->target_id, doom->mdl);

			//! Set sprite size (for all sprite types)
			double floor = room->floor_height / WORLD_SCALE - world->player.position.z;
			double ceil = room->floor_height / WORLD_SCALE + EYE_HEIGHT - world->player.position.z;

			//! Calculate ceil/floor height and draw vertical lines left-to-right.
			saved.scale.start.y = GAME_WIN_HEIGHT / -sprite.start.y;
			saved.scale.stop.y = GAME_WIN_HEIGHT / -sprite.stop.y;

			int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + sprite.start.y * world->player.yaw) * saved.scale.start.y;
			int yawed_start_floor = GAME_MIDHEIGHT - (floor + sprite.start.y * world->player.yaw) * saved.scale.start.y;

			int x = saved.x1;
			if (x < 0)
				x -= x;

				// Debug seg fault
				if (effect->active_sprite == NULL)
				{
					// ft_putendl("Active sprite was NULL in render_items!");
					// printf("effect id: %d\n", effect->id);
					effect = effect->next;
					continue;
				}
			while (x < saved.x2 && x < GAME_WIN_WIDTH)
			{
				signed	horizontal = (x - saved.x1);
				t_xy	len = vec2_sub(sprite.stop, sprite.start);
				double	depth = sprite.start.y + vec2_mul(len, (double)horizontal / (saved.x2 - saved.x1)).y;
				int tex_x = (x - saved.x1) * (double)effect->active_sprite->w / (saved.x2 - saved.x1);
				vertical_sprite(effect->active_sprite, x, tex_x, vec2(yawed_start_ceil, yawed_start_floor), depth);
				++x;
			}
		}

		t_xy_line debug;
		debug = line_add_offset(sprite, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
		if (doom->game->show_info)
			drawline(debug, doom->game->buff);

		effect = effect->next;
	}
}
