/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 15:19:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 16:24:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

double			angle_abc(t_point a, t_point b, t_point c)
{
	t_point		ab;
	t_point		cb;
	double		dot_product;
	double		cross_product;

	ab.x = b.x - a.x;
	ab.y = b.y - a.y;
	cb.x = b.x - c.x;
	cb.y = b.y - c.y;
	dot_product = ((double)(ab.x * cb.x) + (double)(ab.y * cb.y));
	cross_product = ((double)(ab.x * cb.y) - (double)(ab.y * cb.x));
	return (atan2(cross_product, dot_product));
}

int 			tail_degree_rot(t_point location, t_point *tail)
{
	double		result;
	int			x;
	int 		y;

	x = tail->x - location.x;
	y = tail->y - location.y;
	result = atan2(y, x) * 180.0 / M_PI;;
	result += 180.0;
	return ((int)result);
}
