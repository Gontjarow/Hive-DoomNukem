/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_2d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:21:22 by krusthol          #+#    #+#             */
/*   Updated: 2021/03/21 14:22:27 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		draw_rect_color(int x[2], int y[2], unsigned int color, SDL_Surface *buff)
{
	unsigned int *pix;
	int i;
	int iter;
	int limit_x;
	int limit_total;

	iter = 0;
	i = x[0];
	limit_x = x[0] + x[1];
	limit_total = x[1] * y[1];
	pix = (unsigned int*)buff->pixels;
	while (iter < limit_total)
	{
		x[0] = i;
		while (x[0] < limit_x)
		{
			int loc;
			//printf("y[0] = %d | x[0] = %d\n", y[0], x[0]);
			loc = (y[0] * buff->w) + x[0];
			pix[loc] = color;
			x[0]++;
			iter++;
		}
		y[0]++;
	}
}

void draw_surface_ignore_alpha(int x, int y, SDL_Surface *surf, SDL_Surface *buff)
{
	uint32_t *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;

	iter = 0;
	i = x;
	limit_x = x + surf->w;
	limit_total = surf->w * surf->h;
	pix[0] = (uint32_t*)buff->pixels;
	pix[1] = (uint32_t*)surf->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			if ((pix[1][iter]) >> 24 != 0xff)
			{
				iter++;
				x++;
				continue;
			}
			pix[0][(y * buff->w) + x++] = pix[1][iter++];
		}
		y++;
	}
}

static void	typewrite(const char *str, SDL_Surface *buff, int x, int y)
{
	static char *order = NULL;
	static char *part = NULL;
	static int 	rounds = 1;
	static int	ticks = 0;

	if (ft_strequ(str, "reset"))
	{
		if (part)
			free(part);
		part = NULL;
		order = NULL;
		rounds = 1;
		ticks = 0;
		return ;
	}
	if (ticks < 4)
		ticks++;
	if (order == NULL)
	{
		order = str;
		part = ft_strnew(1);
		part = ft_strncpy(part, order, 1);
	}
	else if (ticks == 4 && rounds < ft_strlen(order))
	{
		ticks = 0;
		rounds++;
		free(part);
		part = ft_strnew(rounds);
		part = ft_strncpy(part, order, rounds);
	}
	print_glyph_str(part, buff, x, y);
}

void		render_loading_screen(t_doom *doom, const char *label, SDL_Surface *txt_screen, int reset)
{
	int rect_x[2];
	int rect_y[2];
	static int unveiler = 64;
	static int reached_end = 0;

	if (reset)
	{
		unveiler = 64;
		reached_end = 0;
		typewrite("reset", NULL, 0, 0);
		return ;
	}
	draw_surface(0, 0, txt_screen, doom->game->buff);
	rect_x[0] = 0;
	rect_y[0] = unveiler;
	rect_x[1] = GAME_WIN_WIDTH;
	rect_y[1] = GAME_WIN_HEIGHT - unveiler;
	if (unveiler < GAME_WIN_HEIGHT)
		unveiler++;
	else if (unveiler > GAME_WIN_HEIGHT)
		unveiler = GAME_WIN_HEIGHT;
	if (unveiler == GAME_WIN_HEIGHT && !reached_end)
	{
		reached_end = 1;
		typewrite("reset", NULL, 0, 0);
	}
	else
	{
		draw_rect_color(rect_x, rect_y, 0xff000000, doom->game->buff);
	}
	if (reached_end)
	{
		print_glyph_str(label, doom->game->buff, 45, 40);
		typewrite("press space to begin", doom->game->buff, 45, 445);
	}
	else
		typewrite(label, doom->game->buff, 45, 40);
}