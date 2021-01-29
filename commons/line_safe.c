/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 18:47:22 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/15 15:31:27 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static t_line	*quad_safe(int quad_mode, t_line *l)
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

static void		line_draw_safe(t_line *l, int delta_x, int delta_y)
{
	int	abs_deltas[2];
	int kx;

	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	kx = 2 * abs_deltas[1] - abs_deltas[0];
	set_pixel_safe(l->buff, l->fx, l->fy, l->color);
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
		if (set_pixel_safe(l->buff, l->fx, l->fy, l->color) == 0)
			return ;
	}
}

static void		line_mirror_safe(t_line *l, int delta_x, int delta_y)
{
	int abs_deltas[2];
	int ky;

	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	ky = 2 * abs_deltas[0] - abs_deltas[1];
	set_pixel_safe(l->buff, l->fx, l->fy, l->color);
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
		if (set_pixel_safe(l->buff, l->fx, l->fy, l->color) == 0)
			return ;
	}
}

/* Line algorithm, Bresenham.
 */

void			render_line_safe(t_line *l)
{
	int delta_x;
	int	delta_y;

	delta_x = l->x2 - l->x1;
	delta_y = l->y2 - l->y1;
	if (abs(delta_y) <= abs(delta_x))
	{
		if (delta_x >= 0)
			l = quad_safe(0, l);
		else
			l = quad_safe(1, l);
		line_draw_safe(l, delta_x, delta_y);
	}
	else
	{
		if (delta_y >= 0)
			l = quad_safe(2, l);
		else
			l = quad_safe(3, l);
		line_mirror_safe(l, delta_x, delta_y);
	}
}