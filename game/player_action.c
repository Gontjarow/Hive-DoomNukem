/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_action.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 16:43:51 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/18 17:30:17 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	player_jumps(t_doom *doom)
{
	if (doom->mdl->player.is_jumping == 0 &&
		doom->mdl->player.is_crouching == 0)
	{
		while (doom->mdl->player.is_jumping == 0)
		{
			doom->mdl->player.height += 10;
			if (doom->mdl->player.height == 200)
				doom->mdl->player.is_jumping = 1;
		}
		while (doom->mdl->player.is_jumping == 1)
		{
			doom->mdl->player.height -= 10;
			if (doom->mdl->player.height == 100)
				doom->mdl->player.is_jumping = 0;
		}
	}
}

static void	player_crouch(t_doom *doom)
{
	if (!doom->mdl->player.crouch_lock)
	{
		doom->mdl->player.crouch_lock = 1;
		doom->mdl->player.is_crouching = 1;
		doom->mdl->player.mov_speed = 5;
		while (doom->mdl->player.height != 50)
			doom->mdl->player.height -= 10;
	}
	else
	{
		doom->mdl->player.crouch_lock = 0;
		doom->mdl->player.is_crouching = 0;
		doom->mdl->player.mov_speed = 10;
		while (doom->mdl->player.height != 100)
			doom->mdl->player.height += 10;
	}
}

static void	player_run(t_doom *doom)
{
	if (!doom->mdl->player.run_lock)
	{
		doom->mdl->player.run_lock = 1;
		doom->mdl->player.is_running = 1;
		while (doom->mdl->player.mov_speed != doom->mdl->player.max_speed)
			doom->mdl->player.mov_speed++;
	}
	else
	{
		doom->mdl->player.is_running = 0;
		doom->mdl->player.run_lock = 0;
		while (doom->mdl->player.mov_speed != 10)
			doom->mdl->player.mov_speed--;
	}
}

void		handle_player_action(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_SPACE])
		player_jumps(doom);
	if (doom->keystates[SDL_SCANCODE_LCTRL] && !doom->mdl->player.crouch_lock)
		player_crouch(doom);
	if (!doom->keystates[SDL_SCANCODE_LCTRL] && doom->mdl->player.crouch_lock)
		player_crouch(doom);
	if (doom->keystates[SDL_SCANCODE_LSHIFT] && !doom->mdl->player.run_lock
		&& !doom->mdl->player.crouch_lock)
		player_run(doom);
	if (!doom->keystates[SDL_SCANCODE_LSHIFT] && doom->mdl->player.run_lock)
		player_run(doom);
}
