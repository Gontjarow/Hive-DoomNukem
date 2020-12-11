#include "doom-nukem.h"

static unsigned int code_from_point(t_point *point, SDL_Surface *buff)
{
	unsigned int 	code;

	code = 0;
	if (point->x < 0)
		code |= (1 << 0);
	else if (point->x >= buff->w)
		code |= (1 << 1);
	if (point->y < 0)
		code |= (1 << 2);
	else if (point->y >= buff->h)
		code |= (1 << 3);
	return (code);
}

static int			clip_wall_to_buff(t_wall *clip, t_wall *wall, SDL_Surface *buff)
{
	unsigned int	start_code;
	unsigned int	end_code;
	unsigned int 	code;
	double 			new_x;
	double 			new_y;
	int		 		infinite_loop_guard;

	infinite_loop_guard = 0;
	start_code = code_from_point(&wall->start, buff);
	end_code = code_from_point(&wall->end, buff);
	if ((start_code & end_code) != 0)
	{
			//ft_putendl("Debug: Trivial REJECT!");
		return (0);
	}
	while (!(start_code == 0 && end_code == 0))
	{
		infinite_loop_guard++;
		if (infinite_loop_guard > 9)
		{
			ft_putendl("Warning: Escaped from an infinitely repeating loop from the Cohen-Sutherland algorithm.");
			return (0);
		}
		code = start_code != 0 ? start_code : end_code;
		if ((code & (1 << 0)) != 0)
		{
			new_x = 0;
			new_y = (((double)wall->end.y - (double)wall->start.y) / ((double)wall->end.x - (double)wall->start.x))
					* (new_x - (double)wall->start.x) +	(double)wall->start.y;
		}
		else if ((code & (1 << 1)) != 0)
		{
			new_x = (buff->w - 1);
			new_y = (((double)wall->end.y - (double)wall->start.y) / ((double)wall->end.x - (double)wall->start.x))
					* (new_x - (double)wall->start.x) +	(double)wall->start.y;
		}
		else if ((code & (1 << 3)) != 0)
		{
			new_y = (buff->h - 1);
			new_x = (((double)wall->end.x - (double)wall->start.x) / ((double)wall->end.y - (double)wall->start.y))
					* (new_y - (double)wall->start.y) +	(double)wall->start.x;
		}
		else if ((code & (1 << 2)) != 0)
		{
			new_y = 0;
			new_x = (((double)wall->end.x - (double)wall->start.x) / ((double)wall->end.y - (double)wall->start.y))
					* (new_y - (double)wall->start.y) +	(double)wall->start.x;
		}
		if (code == start_code)
		{
			clip->start.x = (int)new_x;
			clip->start.y = (int)new_y;
			clip->end.x = wall->end.x;
			clip->end.y = wall->end.y;
				//ft_putendl("Updated startpoint with clip_wall_to_buff!");
		}
		else
		{
			clip->start.x = wall->start.x;
			clip->start.y = wall->start.y;
			clip->end.x = (int)new_x;
			clip->end.y = (int)new_y;
				//ft_putendl("Updated endpoint with clip_wall_to_buff!");
		}
				//printf("OLD || From (%d, %d) to (%d, %d) || NEW || From (%d, %d) to (%d, %d)\n",
				// wall->start.x, wall->start.y, wall->end.x, wall->end.y,
				// clip->start.x, clip->start.y, clip->end.x, clip->end.y);
		wall = clip;
		start_code = code_from_point(&wall->start, buff);
		end_code = code_from_point(&wall->end, buff);
	}
		//ft_putendl("clip_wall_to_buff COMPLETED!");
	return (1);
}

static void			wall_to_buffer_clipped(t_wall *wall, SDL_Surface *buff, uint32_t color)
{
	t_line line;

	line.doom = doom_ptr();
	line.buff = buff;
	line.color = color;
	line.x1 = wall->start.x;
	line.y1 = wall->start.y;
	line.x2 = wall->end.x;
	line.y2 = wall->end.y;
	render_line(&line);
}

t_wall				*wall_by_id(int id)
{
	int		wc;
	t_wall 	*wall;

	if (id < 0)
	{
		ft_putendl("Warning: Wall_by_id was requested to return a wall with id less than 0. Returned NULL.");
		return (NULL);
	}
	if (get_model()->wall_count != 0)
		wc = get_model()->wall_count;
	if (wc <= id)
	{
		ft_putendl("Warning: Wall_by_id was requested to return a wall with id equal or less than wall_count of get_model(). Returned NULL.");
		return (NULL);
	}
	wall = get_model()->wall_first;
	while (wc--)
	{
		if (wall->id == id)
			return (wall);
		wall = wall->next;
	}
	ft_putendl("Warning: Wall_by_id could not find a wall with the requested id. Returned NULL.");
	return (NULL);
}

static void			wall_to_buffer_fixed(t_wall *wall, SDL_Surface *buff, uint32_t color)
{
	t_line			line;
	t_wall 			clipped_wall;

	line.doom = doom_ptr();
	line.buff = buff;
	line.color = color;
	line.x1 = wall->start.x;
	line.y1 = wall->start.y;
	line.x2 = wall->end.x;
	line.y2 = wall->end.y;
	if ((line.x1 >= 0 && line.x1 < buff->w) && (line.y1 >= 0 && line.y1 < buff->h)
		&& (line.x2 >= 0 && line.x2 < buff->w) && (line.y2 >= 0 && line.y2 < buff->h))
		render_line(&line);
	else if (clip_wall_to_buff(&clipped_wall, wall, buff))
		wall_to_buffer_clipped(&clipped_wall, buff, 0xffffffff);
}

void 				wall_to_buffer(t_wall *wall, SDL_Surface *buff, uint32_t color)
{
	t_wall			adjusted_wall;
	int 			zoom_factor;
	int 			sx;
	int 			sy;

	zoom_factor = get_state()->zoom_factor;
	sx = get_state()->scroll_x;
	sy = get_state()->scroll_y;	
	if (zoom_factor == 1 && sx == 0 && sy == 0)
		return (wall_to_buffer_fixed(wall, buff, color));
	adjusted_wall.start.x = (wall->start.x - sx) / zoom_factor;
	adjusted_wall.start.y = (wall->start.y - sy) / zoom_factor;
	adjusted_wall.end.x = (wall->end.x - sx) / zoom_factor;
	adjusted_wall.end.y = (wall->end.y - sy) / zoom_factor;
	return (wall_to_buffer_fixed(&adjusted_wall, buff, color));
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