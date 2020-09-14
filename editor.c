/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:41:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/14 18:02:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void 	le_mouse_motion(t_doom *doom)
{
	if (doom)
		return ;
}

void 	le_mouse_down(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;

	i = doom->event.motion.x;
	j = doom->event.motion.y * LE_WIN_WIDTH;
	pixels = doom->le_buff->pixels;
	pixels[i + j] = 0xffffffff;
	pixels[i + j - 1] = 0xffffffff;
	pixels[i + j + 1] = 0xffffffff;
	pixels[i + j + LE_WIN_WIDTH] = 0xffffffff;
	pixels[i + j - LE_WIN_WIDTH] = 0xffffffff;
	Mix_PlayChannel( -1, doom->mcPlop, 0 );
}

void	le_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->le_win);
}