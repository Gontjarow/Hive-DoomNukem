/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 15:51:22 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/18 23:37:21 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	set_pixel(SDL_Surface *buff, int x, int y, uint32_t color)
{
	unsigned int	*pixels;
	int				i;
	int				j;
	int				index;

	i = x;
	j = y * buff->w;
	index = i + j;
	if ((index >= 0) && (index < buff->w * buff->h))
	{
		pixels = buff->pixels;
		pixels[index] = color;
	}
}
