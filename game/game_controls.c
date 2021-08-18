/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_controls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 18:19:21 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 18:19:42 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	game_mouse_motion(t_doom *doom)
{
	static double	yaw = 0.0;
	int				x;
	int				y;
	int				delta;

	SDL_GetRelativeMouseState(&x, &y);
	yaw = clamp(yaw - y * 0.01f, - (M_PI / 8), (M_PI / 8));
	doom->mdl->player.yaw = yaw;
	delta = abs(x);
	if (x > 0)
		get_model()->player.rot += delta;
	if (x < 0)
		get_model()->player.rot -= delta;
	if (doom_ptr()->mdl->player.rot < 0)
		doom_ptr()->mdl->player.rot = 359 + (doom_ptr()->mdl->player.rot);
	if (doom_ptr()->mdl->player.rot >= 360)
		doom_ptr()->mdl->player.rot = 0 + (doom_ptr()->mdl->player.rot) - 360;
	update_player_tail(doom_ptr(), deg_to_rad(doom_ptr()->mdl->player.rot));
}

void	game_mouse_updown(t_doom *doom)
{
	if (doom->event.type == SDL_MOUSEBUTTONUP
		&& doom->event.button.button == SDL_BUTTON_LEFT)
	{
		doom->mdl->player.shooting = 0;
		doom->mdl->player.has_fired = 0;
	}
	if (doom->event.type == SDL_MOUSEBUTTONDOWN
		&& doom->event.button.button == SDL_BUTTON_LEFT)
	{
		doom->mdl->player.shooting = 1;
	}
}
