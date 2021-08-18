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
		return (room_by_id(
			room_id_from_polymap(
				doom_ptr()->mdl->poly_map,
				ptr->loc.x,
				ptr->loc.y)));
	}
	else
	{
		return (room_by_wall_id(ptr->target_id, doom_ptr()->mdl));
	}
}

t_room		*find_pickup_room(t_pickup *ptr)
{
	return (room_by_id(room_id_from_polymap(
		doom_ptr()->mdl->poly_map, ptr->loc.x, ptr->loc.y)));
}

t_room		*find_enemy_room(t_enemy *ptr)
{
	return (room_by_id(room_id_from_polymap(
			doom_ptr()->mdl->poly_map, ptr->x, ptr->y)));
}

void		calculate_sprite_floor_ceil(
	t_room *room, t_world *world, t_xy_line sprite, t_wdata *out)
{
	double base;
	double yawed;

	base = room->floor_height / WORLD_SCALE;
	yawed = sprite.start.y * world->player.yaw;

	out->floor.start.y = base - world->player.position.z;
	out->floor.start.y = (out->floor.start.y + yawed) * out->scale.start.y;
	out->floor.start.y = GAME_MIDHEIGHT - out->floor.start.y;

	out->ceil.start.y = (base + EYE_HEIGHT) - world->player.position.z;
	out->ceil.start.y = (out->ceil.start.y + yawed) * out->scale.start.y;
	out->ceil.start.y = GAME_MIDHEIGHT - out->ceil.start.y;
}

t_stripe	init_stripe(t_wdata saved, t_xy start)
{
	t_stripe screen;

	screen.x = saved.x1;
	screen.y = saved.ceil.start.y;
	screen.y1 = saved.ceil.start.y;
	screen.y2 = saved.floor.start.y;
	screen.x_delta = saved.texture->w / (double)(saved.x2 - saved.x1);
	screen.y_delta = saved.texture->h / (double)(screen.y2 - screen.y1);
	screen.depth = start.y + 0.1;
	return (screen);
}

void		render_sprite(t_point loc, SDL_Surface *tex, t_room *room)
{
	t_world		*world;
	t_wdata		saved;
	t_xy_line	sprite;

	world = get_world();
	sprite = viewer_facing_wall(veci2(loc), world);

	if (sprite.start.y > NEAR_PLANE)
		return ;

	saved.texture = tex;

	//! Only this value is considered for scaling (simple distance).
	saved.scale.start.y = GAME_WIN_HEIGHT / -sprite.start.y;

	saved.x1 = GAME_MIDWIDTH + (sprite.start.x * saved.scale.start.y);
	saved.x2 = GAME_MIDWIDTH + (sprite.stop.x  * saved.scale.start.y);

	//! Ignore impossible.
	if (saved.x1 >= saved.x2 || saved.x2 < 0 || saved.x1 >= GAME_WIN_WIDTH)
		return ;

	calculate_sprite_floor_ceil(room, world, sprite, &saved);

	if (room->lit)
		draw_sprite(saved, init_stripe(saved, sprite.start), set_pixel);
	else
		draw_sprite(saved, init_stripe(saved, sprite.start), set_pixel_dark);

	if (doom_ptr()->game->show_info)
	{
		t_xy_line debug = line_add_offset(sprite,
			vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT - 100));
		drawline(debug, doom_ptr()->game->buff);
	}
}

void		render_sprites(t_doom *doom, int i)
{
	t_effect	*ef;
	t_pickup	*pu;
	t_enemy		*e;

	i = doom->mdl->effect_count;
	ef = doom->mdl->effect_first;
	while (~--i)
	{
		render_sprite(ef->loc, ef->active_sprite, find_effect_room(ef));
		ef = ef->next;
	}
	i = doom->mdl->pickup_count;
	pu = doom->mdl->pickup_first;
	while (~--i)
	{
		render_sprite(pu->loc, pu->active_sprite, find_pickup_room(pu));
		pu = pu->next;
	}
	i = doom->mdl->enemy_count;
	e = doom->mdl->enemy_first;
	while (~--i)
	{
		render_sprite(point(e->x, e->y), e->active_sprite, find_enemy_room(e));
		e = e->next;
	}
}
