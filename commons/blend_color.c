/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blend_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 17:32:59 by ngontjar          #+#    #+#             */
/*   Updated: 2020/02/03 22:32:33 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static uint32_t argb_32_uint(t_argb color)
{
	return (color.a << 24 | color.r << 16 | color.g << 8 | color.b);
}

static t_argb new_32_argb(uint32_t color)
{
	return ((t_argb){
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >> 8) & 0xFF,
			(color) & 0xFF
	});
}

t_argb new_argb(int color)
{
	return ((t_argb){
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >> 8) & 0xFF,
			(color) & 0xFF
	});
}

uint32_t 	invert_color(uint32_t color)
{
	t_argb	invert;

	invert = new_32_argb(color);
	invert.r = 255 - invert.r;
	invert.g = 255 - invert.g;
	invert.b = 255 - invert.b;
	return (argb_32_uint(invert));
}

int blend_argb_original(int foreground, int background)
{
	t_argb fg;
	t_argb bg;
	t_argb blend;
	double ratio;

	fg = new_argb(foreground);
	bg = new_argb(background);

	if (bg.a == 255)
		return (foreground);
	if (fg.a == 255)
		return (background);

	// printf("FG | A %3hhu, R %3hhu, G %3hhu, B %3hhu\n", fg.a, fg.r, fg.g, fg.b);
	// printf("BG | A %3hhu, R %3hhu, G %3hhu, B %3hhu\n", bg.a, bg.r, bg.g, bg.b);

	blend.a = (bg.a == 0 ? 0 : bg.a + fg.a);

	ratio = fg.a / 255.0;
	blend.r = (fg.r * (1 - ratio)) + (bg.r * ratio);
	blend.g = (fg.g * (1 - ratio)) + (bg.g * ratio);
	blend.b = (fg.b * (1 - ratio)) + (bg.b * ratio);

	// printf("=  | A %3hhu, R %3hhu, G %3hhu, B %3hhu\n\n", blend.a, blend.r, blend.g, blend.b);

	return (blend.a << 24 | blend.r << 16 | blend.g << 8 | blend.b);
}

uint32_t blend_argb_ratio(uint32_t foreground, uint32_t background, double ratio)
{
	t_argb fg;
	t_argb bg;
	t_argb blend;

	fg = new_32_argb(foreground);
	bg = new_32_argb(background);

	blend.a = 255 * ratio;
	blend.r = (fg.r * (1 - ratio)) + (bg.r * ratio);
	blend.g = (fg.g * (1 - ratio)) + (bg.g * ratio);
	blend.b = (fg.b * (1 - ratio)) + (bg.b * ratio);
	return (blend.a << 24 | blend.r << 16 | blend.g << 8 | blend.b);
}

