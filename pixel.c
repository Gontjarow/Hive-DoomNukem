/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 15:51:22 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/15 15:56:27 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void 	set_pixel(SDL_Surface *buff, int x, int y, uint32_t color)
{
	unsigned int	*pixels;
	int 			location;
	int 			limit;

	location = x + buff->w * y;
	limit = buff->w * buff->h;
	pixels = buff->pixels;
	if (location >= limit || location < 0)
	{
		//printf("Warning: Set pixel tried to set pixel at %d, %d which is outside buffer memory area. Operation was blocked.\n", x, y);
		return ;
	}
	pixels[location] = color;
}

