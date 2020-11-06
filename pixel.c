/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 15:51:22 by krusthol          #+#    #+#             */
/*   Updated: 2020/11/06 15:26:27 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	flood_buffer(SDL_Surface *buff, uint32_t color)
{
	uint32_t	*pixels;
	int			range;

	pixels = buff->pixels;
	range = buff->w * buff->h;
	while (~--range)
		pixels[range] = color;
}

void 	set_pixel(SDL_Surface *buff, int x, int y, uint32_t color)
{
	uint32_t	*pixels;
	int 		location;
	int 		maximum_limit;

	pixels = buff->pixels;
	location = x + buff->w * y;
	maximum_limit = buff->w * buff->h;
	if (location >= maximum_limit || location < 0)
		return (ft_putendl("Warning: set_pixel called to set a pixel outside buffer memory area. Operation was blocked."));
	pixels[location] = color;
}

