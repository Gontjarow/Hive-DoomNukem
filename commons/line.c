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

static void		line_quick_draw(t_line *l, int delta_x, int delta_y)
{
	int				abs_deltas[2];
	int				kx;
	int				loc;
	int				line_width;
	uint32_t		col;
	uint32_t		*pixels;

	pixels = l->buff->pixels;
	line_width = l->buff->w;
	col = l->color;

	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	kx = 2 * abs_deltas[1] - abs_deltas[0];
	loc = l->fx + (l->fy * line_width);
	pixels[loc] = col;//set_pixel(l->buff, l->fx, l->fy, l->color);
	while (l->fx < l->px)
	{
		loc++;
		l->fx++;
		if (kx < 0)
			kx += 2 * abs_deltas[1];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc += line_width;//l->fy++;
			else
				loc -= line_width;//l->fy--;
			kx += 2 * (abs_deltas[1] - abs_deltas[0]);
		}
		pixels[loc] = col;//set_pixel(l->buff, l->fx, l->fy, l->color);
	}
}

static void		line_care_draw(t_line *l, int delta_x, int delta_y)
{
	int				abs_deltas[2];
	int				kx;
	int				loc;
	int				line_width;
	uint32_t		col;
	uint32_t		*pixels;

	pixels = l->buff->pixels;
	line_width = l->buff->w;
	col = l->color;
	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	kx = 2 * abs_deltas[1] - abs_deltas[0];
	loc = l->fx + (l->fy * line_width);
	if (!(pixels[loc] == l->avoid[0] || pixels[loc] == l->avoid[1] || pixels[loc] == l->avoid[2]))
		pixels[loc] = col;
	while (l->fx < l->px)
	{
		loc++;
		l->fx++;
		if (kx < 0)
			kx += 2 * abs_deltas[1];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc += line_width;//l->fy++;
			else
				loc -= line_width;//l->fy--;
			kx += 2 * (abs_deltas[1] - abs_deltas[0]);
		}
		if (!(pixels[loc] == l->avoid[0] || pixels[loc] == l->avoid[1] || pixels[loc] == l->avoid[2]))
			pixels[loc] = col;//set_pixel(l->buff, l->fx, l->fy, l->color);
	}
}

static void		line_unpreserve_draw(t_line *l, int delta_x, int delta_y)
{
	int			abs_deltas[2];
	int 		kx;
	int			loc;
	int			line_width;
	uint32_t	col;
	uint32_t	*pixels;

	pixels = l->buff->pixels;
	line_width = l->buff->w;
	col = l->color;
	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	kx = 2 * abs_deltas[1] - abs_deltas[0];
	loc = l->fx + (l->fy * line_width);
	if (pixels[loc] == 0xff000000)
		pixels[loc] = col;
	while (l->fx < l->px)
	{
		loc++;
		l->fx++;
		if (kx < 0)
			kx += 2 * abs_deltas[1];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc += line_width;//l->fy++;
			else
				loc -= line_width;//l->fy--;
			kx += 2 * (abs_deltas[1] - abs_deltas[0]);
		}
		if (pixels[loc] == 0xff000000)
			pixels[loc] = col;
	}
}

static void		line_preserve_draw(t_line *l, int delta_x, int delta_y)
{
	int			abs_deltas[2];
	int 		kx;
	int			loc;
	int			line_width;
	uint32_t	col;
	uint32_t	*pixels;
	uint32_t 	*com_pixels;

	pixels = l->buff->pixels;
	com_pixels = editor_back_buffer()->buff->pixels;
	line_width = l->buff->w;
	col = l->color;
	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	kx = 2 * abs_deltas[1] - abs_deltas[0];
	loc = l->fx + (l->fy * line_width);
	if (com_pixels[loc] == 0xff000000)
		pixels[loc] = col;
	while (l->fx < l->px)
	{
		loc++;
		l->fx++;
		if (kx < 0)
			kx += 2 * abs_deltas[1];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc += line_width;//l->fy++;
			else
				loc -= line_width;//l->fy--;
			kx += 2 * (abs_deltas[1] - abs_deltas[0]);
		}
		if (com_pixels[loc] == 0xff000000)
			pixels[loc] = col;
	}
}

// TODO: Norminettize, plus make caution calculations. Calculate once, that initial loc is not below 0
//	Also calculate, that the last expected pixel will not be off buffer. If it is off the buffer, redirect
//	To calling line_safe instead which is slow but wont crash. Write a warning message to help debug aswell.
//	Also do these steps for the line_quick. Archive depreceated respective functions as well.

static void 	line_quick_mirror(t_line *l, int delta_x, int delta_y)
{
	int			abs_deltas[2];
	int			ky;
	int			loc;
	int			line_width;
	uint32_t	col;
	uint32_t	*pixels;

	pixels = l->buff->pixels;
	line_width = l->buff->w;
	col = l->color;

	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	ky = 2 * abs_deltas[0] - abs_deltas[1];
	loc = l->fx + (l->fy * line_width);
	pixels[loc] = col;//set_pixel(l->buff, l->fx, l->fy, l->color);
	while (l->fy < l->py)
	{
		loc += line_width;
		l->fy++;
		if (ky <= 0)
			ky += 2 * abs_deltas[0];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc++;//l->fx++;
			else
				loc--;//l->fx--;
			ky += 2 * (abs_deltas[0] - abs_deltas[1]);
		}
		pixels[loc] = col;//set_pixel(l->buff, l->fx, l->fy, l->color);
	}
}

static void 	line_care_mirror(t_line *l, int delta_x, int delta_y)
{
	int			abs_deltas[2];
	int			ky;
	int			loc;
	int			line_width;
	uint32_t	col;
	uint32_t	*pixels;

	pixels = l->buff->pixels;
	line_width = l->buff->w;
	col = l->color;

	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	ky = 2 * abs_deltas[0] - abs_deltas[1];
	loc = l->fx + (l->fy * line_width);
	if (!(pixels[loc] == l->avoid[0] || pixels[loc] == l->avoid[1] || pixels[loc] == l->avoid[2]))
		pixels[loc] = col;
	while (l->fy < l->py)
	{
		loc += line_width;
		l->fy++;
		if (ky <= 0)
			ky += 2 * abs_deltas[0];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc++;//l->fx++;
			else
				loc--;//l->fx--;
			ky += 2 * (abs_deltas[0] - abs_deltas[1]);
		}
		if (!(pixels[loc] == l->avoid[0] || pixels[loc] == l->avoid[1] || pixels[loc] == l->avoid[2]))
			pixels[loc] = col;
	}
}

static void		line_preserve_mirror(t_line *l, int delta_x, int delta_y)
{
	int 		abs_deltas[2];
	int 		ky;
	int			loc;
	int			line_width;
	uint32_t	col;
	uint32_t	*pixels;
	uint32_t 	*com_pixels;

	pixels = l->buff->pixels;
	com_pixels = editor_back_buffer()->buff->pixels;
	line_width = l->buff->w;
	col = l->color;
	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	ky = 2 * abs_deltas[0] - abs_deltas[1];
	loc = l->fx + (l->fy * line_width);
	if (com_pixels[loc] == 0xff000000)
		pixels[loc] = col;
	while (l->fy < l->py)
	{
		loc += line_width;
		l->fy++;
		if (ky <= 0)
			ky += 2 * abs_deltas[0];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc ++;
			else
				loc --;
			ky += 2 * (abs_deltas[0] - abs_deltas[1]);
		}
		if (com_pixels[loc] == 0xff000000)
			pixels[loc] = col;
	}
}

static void		line_unpreserve_mirror(t_line *l, int delta_x, int delta_y)
{
	int 		abs_deltas[2];
	int 		ky;
	int			loc;
	int			line_width;
	uint32_t	col;
	uint32_t	*pixels;

	pixels = l->buff->pixels;
	line_width = l->buff->w;
	col = l->color;
	abs_deltas[0] = abs(delta_x);
	abs_deltas[1] = abs(delta_y);
	ky = 2 * abs_deltas[0] - abs_deltas[1];
	loc = l->fx + (l->fy * line_width);
	if (pixels[loc] == 0xff000000)
		pixels[loc] = col;
	while (l->fy < l->py)
	{
		loc += line_width;
		l->fy++;
		if (ky <= 0)
			ky += 2 * abs_deltas[0];
		else
		{
			if ((delta_x < 0 && delta_y < 0) || (delta_x > 0 && delta_y > 0))
				loc ++;
			else
				loc --;
			ky += 2 * (abs_deltas[0] - abs_deltas[1]);
		}
		if (pixels[loc] == 0xff000000)
			pixels[loc] = col;
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
		line_quick_draw(l, delta_x, delta_y);
	}
	else
	{
		if (delta_y >= 0)
			l = quad(2, l);
		else
			l = quad(3, l);
		line_quick_mirror(l, delta_x, delta_y);
	}
}

void			careful_render_line(t_line *l)
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
		line_care_draw(l, delta_x, delta_y);
	}
	else
	{
		if (delta_y >= 0)
			l = quad(2, l);
		else
			l = quad(3, l);
		line_care_mirror(l, delta_x, delta_y);
	}
}

void			preserve_render_line(t_line *l)
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
		line_preserve_draw(l, delta_x, delta_y);
	}
	else
	{
		if (delta_y >= 0)
			l = quad(2, l);
		else
			l = quad(3, l);
		line_preserve_mirror(l, delta_x, delta_y);
	}
}

void			unpreserve_render_line(t_line *l)
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
		line_unpreserve_draw(l, delta_x, delta_y);
	}
	else
	{
		if (delta_y >= 0)
			l = quad(2, l);
		else
			l = quad(3, l);
		line_unpreserve_mirror(l, delta_x, delta_y);
	}
}