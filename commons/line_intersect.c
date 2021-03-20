/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 16:19:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/09 16:46:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int				imin(int a, int b)
{
	return (a > b ? b : a);
}

int				imax(int a, int b)
{
	return (a > b ? a : b);
}

int				on_segment(t_point p, t_point q, t_point r)
{
	if (q.x <= imax(p.x, r.x) && q.x >= imin(p.x, r.x) &&
		q.y <= imax(p.y, r.y) && q.y >= imin(p.y, r.y))
		return (1);
	return (0);
}

int				point_orientation(t_point p, t_point q, t_point r)
{
	int			val;

	val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;

	return (val > 0)? 1: 2;
}

int				do_intersect(t_point p1, t_point q1, t_point p2, t_point q2)
{
	int			orient[4];

	orient[0] = point_orientation(p1, q1, p2);
	orient[1] = point_orientation(p1, q1, q2);
	orient[2] = point_orientation(p2, q2, p1);
	orient[3] = point_orientation(p2, q2, q1);
	if (orient[0] != orient[1] && orient[2] != orient[3])
		return (1);
	if (orient[0] == 0 && on_segment(p1, p2, q1))
		return (1);
	if (orient[1] == 0 && on_segment(p1, q2, q1))
		return (1);
	if (orient[2] == 0 && on_segment(p2, p1, q2))
		return (1);
	if (orient[3] == 0 && on_segment(p2, q1, q2))
		return (1);
	return (0);
}