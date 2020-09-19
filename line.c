/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 18:47:22 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/19 06:12:05 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static t_line	*quad(int quad_mode, t_line *l)
{
	l->fx = l->x1;
	l->fy = l->y1;
	l->px = l->x2;
	l->py = -1;
	if (quad_mode == 1)
	{
		l->fx = l->x2;
		l->fy = l->y2;
		l->px = l->x1;
		l->py = -1;
	}
	else if (quad_mode == 2)
	{
		l->px = -1;
		l->py = l->y2;
	}
	else if (quad_mode == 3)
	{
		l->fx = l->x2;
		l->fy = l->y2;
		l->px = -1;
		l->py = l->y1;
	}
	return (l);
}

static void		line_draw(t_line *l, int delta_x, int delta_y)
{
	int	abs_deltas[2];
	int kx;

	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	kx = 2 * abs_deltas[1] - abs_deltas[0];
	set_pixel(l->buff, l->fx, l->fy, l->color);
	while (l->fx < l->px)
	{
		l->fx++;
		if (kx < 0)
			kx += 2 * abs_deltas[1];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				l->fy++;
			else
				l->fy--;
			kx += 2 * (abs_deltas[1] - abs_deltas[0]);
		}
		set_pixel(l->buff, l->fx, l->fy, l->color);
	}
}

static void		line_mirror(t_line *l, int delta_x, int delta_y)
{
	int abs_deltas[2];
	int ky;

	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	ky = 2 * abs_deltas[0] - abs_deltas[1];
	set_pixel(l->buff, l->fx, l->fy, l->color);
	while (l->fy < l->py)
	{
		l->fy++;
		if (ky <= 0)
			ky += 2 * abs_deltas[0];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				l->fx++;
			else
				l->fx--;
			ky += 2 * (abs_deltas[0] - abs_deltas[1]);
		}
		set_pixel(l->buff, l->fx, l->fy, l->color);
	}
}

/* Line algorithm, Bresenham.
 */

void			render_line(t_line *l)
{
	int delta_x;
	int	delta_y;

	delta_x = l->x2 - l->x1;
	delta_y = l->y2 - l->y1;
	if (abs(delta_y) <= abs(delta_x))
	{
		if (delta_x >= 0)
			l = quad(0, l);
		else
			l = quad(1, l);
		line_draw(l, delta_x, delta_y);
	}
	else
	{
		if (delta_y >= 0)
			l = quad(2, l);
		else
			l = quad(3, l);
		line_mirror(l, delta_x, delta_y);
	}
}

void ft_draw(unsigned int *pixel, t_xyz start, t_xyz end, int color)
{
	int max = GAME_WIN_WIDTH * GAME_WIN_HEIGHT;
	int index;
	t_xyz dir = vec3_sub(end, start);

	t_xyz abs = (t_xyz){fabs(dir.x), fabs(dir.y), 0};
	float step = (abs.x > abs.y) ? abs.x : abs.y;
	dir = vec3_div(dir, step);

	int i = 0;
	while (i <= step)
	{
		index = (int)start.x + (int)start.y * GAME_WIN_WIDTH;
		if (index >= 0 && index < max)
			pixel[index] = color;
		start.x += dir.x;
		start.y += dir.y;
		++i;
	}
}
