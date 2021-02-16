/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atlas.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 13:55:00 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/16 13:59:28 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		copy_from_atlas(int x, int y, SDL_Surface *atlas, SDL_Surface *buff)
{
	int *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;
	int to;
	int from;

	iter = 0;
	i = x;
	limit_x = x + buff->w;
	limit_total = buff->w * buff->h;
	pix[0] = (int*)buff->pixels;
	pix[1] = (int*)atlas->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			from = (atlas->w * y) + x;
			to = iter;
			pix[0][to] = pix[1][from];
			x++;
			iter++;
		}
		y++;
	}
}