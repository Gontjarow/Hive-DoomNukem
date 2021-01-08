/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 15:19:54 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/18 15:20:18 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

int 			tail_degree_rot(t_point location, t_point *tail)
{
	double	result;
	int		x;
	int 	y;

	x = tail->x - location.x;
	y = tail->y - location.y;
	result = atan2(y, x) * 180.0 / M_PI;;
	result += 180.0;
	return ((int)result);
}
