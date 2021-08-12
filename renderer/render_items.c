#include "doom-nukem.h"

void			render_pickups(t_doom *doom)
{
	t_world		*world = doom->game->world;
	signed		pickup_count = doom->mdl->pickup_count;
	t_pickup	*pickup = doom->mdl->pickup_first;
	while (~--pickup_count)
	{
		double turn_90 = world->player.angle + PI_BY_TWO;
		t_xy epos  = vec2_div(vec2(pickup->loc.x, pickup->loc.y), WORLD_SCALE);
		t_xy left  = vec2_add(epos, vec2(-cos(turn_90) * SPRITE_SCALE, -sin(turn_90) * SPRITE_SCALE));
		t_xy right = vec2_add(epos, vec2(+cos(turn_90) * SPRITE_SCALE, +sin(turn_90) * SPRITE_SCALE));
		left = vec2_sub(left, vec32(world->player.position));
		right = vec2_sub(right, vec32(world->player.position));

		t_xy_line sprite = line_xy(left, right, 0xff00ff);
		sprite = line_rot(sprite, world->player.angle + M_PI);

		// draw
		if (sprite.start.y <= NEAR_PLANE)
		{
			t_xy_line scale;

			//! Calculate points scaled by horizontal FOV.
			scale.start.x = GAME_WIN_HEIGHT / -sprite.start.y;
			scale.stop.x = GAME_WIN_HEIGHT / -sprite.stop.y;

			int left = GAME_MIDWIDTH + (int)(sprite.start.x * scale.start.x);
			int right = GAME_MIDWIDTH + (int)(sprite.stop.x * scale.stop.x);

			//! Ignore impossible.
			if(left >= right || right < 0 || left >= GAME_WIN_WIDTH)
			{
				pickup = pickup->next;
				continue;
			}

			int pickup_id = room_id_from_polymap(doom->mdl->poly_map, pickup->loc.x, pickup->loc.y);

			// Lower height for sprites based on type
			double floor = world->sectors[pickup_id].floor - 0.5 - world->player.position.z;
			double ceil;
			if (pickup->flavor == PICKUP_HEALTH || pickup->flavor == PICKUP_AMMO || pickup->flavor == PICKUP_KEY)
				ceil = world->sectors[pickup_id].floor + 2 - world->player.position.z;
			else if (pickup->flavor == PICKUP_WEAPON && pickup->weapon_type_id == JETPACK)
				ceil = world->sectors[pickup_id].floor + 2 - world->player.position.z;
			else if (pickup->flavor == PICKUP_WEAPON)
				ceil = world->sectors[pickup_id].floor + 1 - world->player.position.z;
			else
				ceil = world->sectors[pickup_id].floor + EYE_HEIGHT - world->player.position.z;

			//! Calculate ceil/floor height and draw vertical lines left-to-right.
			scale.start.y = GAME_WIN_HEIGHT / -sprite.start.y;
			scale.stop.y = GAME_WIN_HEIGHT / -sprite.stop.y;

			int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + sprite.start.y * world->player.yaw) * scale.start.y;
			int yawed_start_floor = GAME_MIDHEIGHT - (floor + sprite.start.y * world->player.yaw) * scale.start.y;

			int x = left;
			if (x < 0)
				x -= x;
			while (x < right && x < GAME_WIN_WIDTH)
			{
				signed	horizontal = (x - left);
				t_xy	len = vec2_sub(sprite.stop, sprite.start);
				double	depth = sprite.start.y + vec2_mul(len, (double)horizontal / (right - left)).y;
				int tex_x = (x - left) * (double)pickup->active_sprite->w / (right - left);
				vertical_sprite(pickup->active_sprite, x, tex_x, vec2(yawed_start_ceil, yawed_start_floor), depth);
				++x;
			}
		}

		t_xy_line debug;
		debug = line_add_offset(sprite, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
		if (doom->game->show_info)
			drawline(debug, doom->game->buff);

		pickup = pickup->next;
	}
}

#include "doom-nukem.h"

// TODO: active_sprite needs a value!
void			render_effectors(t_doom *doom)
{
	t_world		*world = doom->game->world;
	signed		effect_count = doom->mdl->effect_count;
	t_effect	*effect = doom->mdl->effect_first;
	while (~--effect_count)
	{
		double turn_90 = world->player.angle + PI_BY_TWO;
		t_xy epos  = vec2_div(vec2(effect->loc.x, effect->loc.y), WORLD_SCALE);
		t_xy left  = vec2_add(epos, vec2(-cos(turn_90) * SPRITE_SCALE, -sin(turn_90) * SPRITE_SCALE));
		t_xy right = vec2_add(epos, vec2(+cos(turn_90) * SPRITE_SCALE, +sin(turn_90) * SPRITE_SCALE));
		left = vec2_sub(left, vec32(world->player.position));
		right = vec2_sub(right, vec32(world->player.position));

		t_xy_line sprite = line_xy(left, right, 0xff00ff);
		sprite = line_rot(sprite, world->player.angle + M_PI);

		// draw
		if (sprite.start.y <= NEAR_PLANE)
		{
			t_xy_line scale;

			//! Calculate points scaled by horizontal FOV.
			scale.start.x = GAME_WIN_HEIGHT / -sprite.start.y;
			scale.stop.x = GAME_WIN_HEIGHT / -sprite.stop.y;

			int left = GAME_MIDWIDTH + (int)(sprite.start.x * scale.start.x);
			int right = GAME_MIDWIDTH + (int)(sprite.stop.x * scale.stop.x);

			//! Ignore impossible.
			if(left >= right || right < 0 || left >= GAME_WIN_WIDTH)
			{
				effect = effect->next;
				continue;
			}

			int effect_id = room_id_from_polymap(doom->mdl->poly_map, effect->loc.x, effect->loc.y);

			double floor = world->sectors[effect_id].floor - 0.5 - world->player.position.z;
			double ceil = world->sectors[effect_id].floor + EYE_HEIGHT - world->player.position.z;

			//! Calculate ceil/floor height and draw vertical lines left-to-right.
			scale.start.y = GAME_WIN_HEIGHT / -sprite.start.y;
			scale.stop.y = GAME_WIN_HEIGHT / -sprite.stop.y;

			int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + sprite.start.y * world->player.yaw) * scale.start.y;
			int yawed_start_floor = GAME_MIDHEIGHT - (floor + sprite.start.y * world->player.yaw) * scale.start.y;

			int x = left;
			if (x < 0)
				x -= x;

				// Debug seg fault
				if (effect->active_sprite == NULL)
				{
					ft_putendl("Active sprite was NULL in render_items!");
					printf("effect id: %d\n", effect->id);
					effect = effect->next;
					continue;
				}
			while (x < right && x < GAME_WIN_WIDTH)
			{
				signed	horizontal = (x - left);
				t_xy	len = vec2_sub(sprite.stop, sprite.start);
				double	depth = sprite.start.y + vec2_mul(len, (double)horizontal / (right - left)).y;
				int tex_x = (x - left) * (double)effect->active_sprite->w / (right - left);
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
