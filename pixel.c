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
	int 			i;
	int 			j;

	i = x;
	j = buff->w * y;
	pixels = buff->pixels;
	pixels[i + j] = color;
}

