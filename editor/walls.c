#include "doom-nukem.h"

void 				wall_to_buffer(t_wall *wall, SDL_Surface *buff, uint32_t color)
{
	t_line			line;

	line.doom = doom_ptr();
	line.buff = buff;
	line.color = color;
	line.x1 = wall->start.x;
	line.y1 = wall->start.y;
	line.x2 = wall->end.x;
	line.y2 = wall->end.y;
	render_line(&line);
}

void				x_walls_to_buffer(int x, t_wall *wall, SDL_Surface *buff, uint32_t color)
{
	t_wall *iter;

	iter = wall;
	while (x--)
	{
		wall_to_buffer(iter, buff, color);
		iter = iter->next;
	}
}

void 				relink_model_walls(t_wall *relinking_wall)
{
	int				wc;
	t_wall			*wall;

	wc = get_model()->wall_count;
	if (wc == 0)
	{
		get_model()->walls = relinking_wall;
		return ;
	}
	wall = get_model()->wall_first;
	while (wc-- > 1)
		wall = wall->next;
	wall->next = relinking_wall;
	get_model()->walls = wall->next;
}