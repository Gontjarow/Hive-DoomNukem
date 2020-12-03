#include "doom-nukem.h"
/*
void	print_characters(t_editor *edt)
{
	t_enemy *enemy;
	t_line	line;
	int 	ec;

	if (edt->player_set == 1)
	{
		line.x1 = edt->player.x;
		line.y1 = edt->player.y;
		line.x2 = edt->player.tail.x;
		line.y2 = edt->player.tail.y;
		line.color = 0xffffff00;
		line.buff = edt->buff;
		render_line(&line);
	}
	ec = edt->enemy_count;
	if (ec == 0)
		return ;
	enemy = edt->enemy_first;
	while (ec--)
	{
		line.x1 = enemy->x;
		line.y1 = enemy->y;
		line.x2 = enemy->tail.x;
		line.y2 = enemy->tail.y;
		line.color = 0xff00ff00;
		line.buff = edt->buff;
		render_line(&line);
		enemy = enemy->next;
	}
}

void	print_portals(t_editor *edt)
{
	int		pc;
	t_wall	*portal;
	t_line	line;

	pc = edt->portal_count;
	if (pc == 0)
		return ;
	portal = edt->portal_begin;
	//printf("Portals | portal_count %d\n------------------\n\n", pc);
	while (pc--)
	{
		//printf("Portal id: %d | start: %d, %d | end: %d, %d\n",
		//	   portal->id, portal->start.x, portal->start.y, portal->end.x, portal->end.y);
		line.x1 = portal->start.x;
		line.y1 = portal->start.y;
		line.x2 = portal->end.x;
		line.y2 = portal->end.y;
		line.color = 0xffff0000;
		line.buff = edt->buff;
		render_line(&line);
		portal = portal->next;
	}
}

void	print_walls(t_editor *edt)
{
	int		wc;
	t_wall	*wall;
	t_line	line;

	wc = edt->wall_count;
	wall = edt->wall_begin;
	//printf("Walls | wall_count %d\n------------------\n\n", wc);
	while (wc--)
	{
		//printf("Wall id: %d | start: %d, %d | end: %d, %d\n",
		//		wall->id, wall->start.x, wall->start.y, wall->end.x, wall->end.y);
		line.x1 = wall->start.x;
		line.y1 = wall->start.y;
		line.x2 = wall->end.x;
		line.y2 = wall->end.y;
		line.color = 0xffffffff;
		line.buff = edt->buff;
		render_line(&line);
		wall = wall->next;
	}
	print_characters(edt);
	print_portals(edt);
}

void	circle_rooms(t_doom *doom)
{
	int		rc;
	t_room	*room;

	if (doom->edt->room_count == 0)
		return;
	rc = doom->edt->room_count;
	room = doom->edt->room_first;
	while (rc--)
	{
		room->visual.x = 0;
		room->visual.y = 0;
		find_visual_xy(doom->edt, room);
		if (room->visual.x > 0 && room->visual.y > 0)
			circle_room(doom, room);
		room = room->next;
	}
}
*/

void	draw_scroll_bars_to_backbuffer(t_state *state)
{
	t_line	line;

	line.doom = doom_ptr();
	line.color = 0xffffffff;
	line.buff = editor_back_buffer()->buff;
	// Lines that bound the bottom and left edge of screen
	line.x1 = 0;
	line.y1 = EDT_WIN_HEIGHT - 10;
	line.x2 = EDT_WIN_WIDTH - 10;
	line.y2 = line.y1;
	render_line(&line);
	line.x1 = line.x2;
	line.y1 = 0;
	line.y2 = EDT_WIN_HEIGHT - 10;
	render_line(&line);
	// Line that represents the horizontal scroll
	line.x1 = (int)((float)state->scroll_x * 0.25f);
	line.x2 = line.x1 + (int)((float)EDT_WIN_WIDTH * 0.25f * (float)state->zoom_factor) - 5;
	line.y1 = EDT_WIN_HEIGHT - 5;
	line.y2 = line.y1;
	render_line(&line);
	// Line that represents the vertical scroll
	line.y1 = (int)((float)state->scroll_y * 0.25f);
	line.y2 = line.y1 + (int)((float)EDT_WIN_HEIGHT * 0.25f * (float)state->zoom_factor) - 5;
	line.x1 = EDT_WIN_WIDTH - 5;
	line.x2 = line.x1;
	render_line(&line);
}
/*
void 	circle_visual(SDL_Surface *buff, t_point *visual, uint32_t color)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = buff->pixels;
	radius = 3;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)visual->x + x + (((int)visual->y + y) * EDT_WIN_WIDTH)] = color;
			x++;
		}
		y++;
		x = -radius;
	}
}

void	circle_room(t_doom *doom, t_room *room)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 3;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)room->visual.x + x + (((int)room->visual.y + y) * EDT_WIN_WIDTH)] = 0xffffffff;
			x++;
		}
		y++;
		x = -radius;
	}
}

void	circle_player(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)doom->edt->player.x + x + (((int)doom->edt->player.y + y) * EDT_WIN_WIDTH)] = 0xffffff00;
			x++;
		}
		y++;
		x = -radius;
	}
}

void	circle_enemy(t_doom *doom)
{
	unsigned int *pixels;
	int radius;
	int x;
	int y;

	pixels = doom->edt->buff->pixels;
	radius = 6;
	y = -radius;
	x = -radius;
	while (y <= radius)
	{
		while (x <= radius)
		{
			if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
				pixels[(int)doom->edt->last_enemy.x + x + (((int)doom->edt->last_enemy.y + y) * EDT_WIN_WIDTH)] = 0xff00ff00;
			x++;
		}
		y++;
		x = -radius;
	}
}
*/
