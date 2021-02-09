#include "doom-nukem.h"

static void		print_orient(t_enemy *enemy)
{
	if (enemy->anim.orient == FRONT)
		puts("Enemy orient is FRONT!");
	else if (enemy->anim.orient == BACK)
		puts("Enemy orient is BACK!");
	else if (enemy->anim.orient == LEFT)
		puts("Enemy orient is LEFT!");
	else if (enemy->anim.orient == RIGHT)
		puts("Enemy orient is RIGHT!");
}

void			render_enemies(t_doom *doom)
{
    t_world *world = doom->game->world;
    //! Render enemies.
	// TODO: Z Buffer
	signed	enemy_count = doom->mdl->enemy_count;
	t_enemy	*enemy = doom->mdl->enemy_first;
	while (~--enemy_count)
	{
		double turn_90 = world->player.angle + PI_BY_TWO;
		t_xy epos  = vec2_div(vec2(enemy->x, enemy->y), WORLD_SCALE);
		t_xy left  = vec2_add(epos, vec2(-cos(turn_90) * SPRITE_SCALE, -sin(turn_90) * SPRITE_SCALE));
		t_xy right = vec2_add(epos, vec2(+cos(turn_90) * SPRITE_SCALE, +sin(turn_90) * SPRITE_SCALE));
		left = vec2_sub(left, vec32(world->player.position));
		right = vec2_sub(right, vec32(world->player.position));

		t_xy_line eline = line_xy(left, right, 0xff00ff);
		eline = line_rot(eline, world->player.angle + M_PI);

		// draw
		if (eline.start.y <= NEAR_PLANE)
		{
			t_xy_line scale;

			//! Calculate points scaled by horizontal FOV.
			scale.start.x = GAME_WIN_HEIGHT / -eline.start.y;
			scale.stop.x = GAME_WIN_HEIGHT / -eline.stop.y;

			int left = GAME_MIDWIDTH + (int)(eline.start.x * scale.start.x);
			int right = GAME_MIDWIDTH + (int)(eline.stop.x * scale.stop.x);

			//! Ignore impossible walls.
			if(left >= right || right < 0 || left >= GAME_WIN_WIDTH)
			{
				enemy = enemy->next;
				continue;
			}

			int enemy_id = room_id_from_polymap(doom->mdl->poly_map, enemy->x, enemy->y);

			// TODO: [OLD NOTE by ngontjar]: Define enemy height in a sensible way.
			//  [AFTER EDITING BY krusthol]: In my branch after merge I adjusted some of the values to clean up the visuals
			double ceil = world->sectors[enemy_id].floor + 6 - world->player.position.z;
			double floor = world->sectors[enemy_id].floor - 0.5 - world->player.position.z;

			//! Calculate ceil/floor height and draw vertical lines left-to-right.
			scale.start.y = GAME_WIN_HEIGHT / -eline.start.y;
			scale.stop.y = GAME_WIN_HEIGHT / -eline.stop.y;

			int yawed_start_ceil  = GAME_MIDHEIGHT - (ceil  + eline.start.y * world->player.yaw) * scale.start.y;
			int yawed_start_floor = GAME_MIDHEIGHT - (floor + eline.start.y * world->player.yaw) * scale.start.y;
			//Debugging orient for animations
			{
				//print_orient(enemy);
				/*int new_orient = check_sprite_facing(enemy, get_model());
				if (new_orient != enemy->anim.orient) {
					enemy->anim.orient = new_orient;
					print_orient(enemy);
				}*/
			}
			int x = left;
			if (x < 0)
				x -= x;
			while (x < right && x < GAME_WIN_WIDTH)
			{
				int tex_x = (x - left) * (double)enemy->active_sprite->w / (right - left);
				vertical_sprite(enemy, x, tex_x, vec2(yawed_start_ceil, yawed_start_floor));
				++x;
			}
		}

		t_xy_line debug;
		debug = line_add_offset(eline, vec2(GAME_MIDWIDTH, GAME_WIN_HEIGHT-100));
		if (doom->game->show_info)
			drawline(debug, doom->game->buff);

		enemy = enemy->next;
	}
}
