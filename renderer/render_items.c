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

t_room		*find_effect_room(t_effect *ptr)
{
	if (ptr->type_id == EFFECT_EXIT)
	{
		return room_by_id(
			room_id_from_polymap(
				doom_ptr()->mdl->poly_map,
				ptr->loc.x,
				ptr->loc.y));
	}
	else
	{
		return room_by_wall_id(ptr->target_id, doom_ptr()->mdl);
	}
}

void		render_sprite(t_point loc, SDL_Surface *tex, t_room *room)
{
	t_world		*world;
	t_wdata		saved;

	world = get_world();
	t_xy location = veci2(loc);
	t_xy_line sprite = viewer_facing_wall(location, world);

	if (sprite.start.y > NEAR_PLANE)
		return;

	saved.texture = tex;

	//! Only this value is considered for scaling (simple distance).
	saved.scale.start.y = GAME_WIN_HEIGHT / -sprite.start.y;

	saved.x1 = GAME_MIDWIDTH + (sprite.start.x * saved.scale.start.y);
	saved.x2 = GAME_MIDWIDTH + (sprite.stop.x  * saved.scale.start.y);

	//! Ignore impossible.
	if (saved.x1 >= saved.x2 || saved.x2 < 0 || saved.x1 >= GAME_WIN_WIDTH)
		return;

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

	if (doom_ptr()->game->show_info)
	{
		t_xy_line debug;
		debug = line_add_offset(sprite, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
		drawline(debug, doom_ptr()->game->buff);
	}
}

void		render_sprites(t_doom *doom)
{
	signed		i;
	t_effect	*effect;
	t_pickup	*pickup;

	i = doom->mdl->effect_count;
	effect = doom->mdl->effect_first;
	while (~--i)
	{
		render_sprite(effect->loc, effect->active_sprite, find_effect_room(effect));
		effect = effect->next;
	}
	i = doom->mdl->pickup_count;
	pickup = doom->mdl->pickup_first;
	while (~--i)
	{
		render_sprite(pickup->loc, pickup->active_sprite,
			room_by_id(room_id_from_polymap(doom->mdl->poly_map, pickup->loc.x, pickup->loc.y)));
		pickup = pickup->next;
	}
}
