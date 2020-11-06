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

void	flood_window(SDL_Surface *buff, uint32_t color)
{
	unsigned int	*pixels;
	int all;

	all = buff->w * buff->h;
	pixels = buff->pixels;
	while (~--all)
	{
		pixels[all] = color;
	}
}

void 	set_pixel(SDL_Surface *buff, int x, int y, uint32_t color)
{
	uint32_t	*pixels;
	int 		location;
	int 		limit;

	location = x + buff->w * y;
	limit = buff->w * buff->h;
	pixels = buff->pixels;
	if (location >= limit || location < 0)
		return (ft_putendl("Warning: set_pixel called to set a pixel outside buffer memory area. Operation was blocked."));
	pixels[location] = color;
}

