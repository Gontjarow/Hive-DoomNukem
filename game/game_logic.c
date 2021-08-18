/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 18:25:47 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 19:20:33 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	render_chapter_screen(t_doom *doom, int chapter)
{
	if (chapter == -1)
		render_loading_screen(doom, "level loaded and ready to play",
			doom->sprites->txt_loading_none, 0);
	else if (chapter == 0)
		render_loading_screen(doom, "first chapter",
			doom->sprites->txt_loading_0, 0);
	else if (chapter == 1)
		render_loading_screen(doom, "second chapter",
			doom->sprites->txt_loading_1, 0);
	else if (chapter == 2)
		render_loading_screen(doom, "third chapter",
			doom->sprites->txt_loading_2, 0);
}

void	handle_show_loading(t_doom *doom)
{
	if (doom->game->won_the_game == -1)
		return (render_losing_screen(doom, 0));
	if (doom->game->won_the_game == 1)
		return (render_winning_screen(doom, 0));
	if (!Mix_Playing(7))
		Mix_PlayChannel(7, doom->sounds->mcLoading, -1);
	if (doom->mdl->chain != NULL || doom->chapter_index > 0)
		render_chapter_screen(doom, doom->chapter_index);
	else
		render_chapter_screen(doom, -1);
	if (doom->keystates[SDL_SCANCODE_RETURN])
	{
		Mix_HaltChannel(7);
		doom->game->show_loading = 0;
		if (doom->mdl->chain != NULL || doom->chapter_index > 0)
			doom->chapter_index++;
		if (!Mix_Playing(7) && doom->chapter_index != 3)
			Mix_PlayChannel(7, doom->sounds->mcBackground, -1);
		else if (!Mix_Playing(7) && doom->chapter_index == 3)
			Mix_PlayChannel(7, doom->sounds->mcBoss, -1);
	}
	SDL_UpdateWindowSurface(doom->game->win);
}

void	check_losing_condition(t_doom *doom)
{
	if (doom->game->won_the_game != 0)
		return ;
	if (doom->mdl->player.hp.cur < 1)
	{
		if (Mix_Playing(7))
			Mix_HaltChannel(7);
		doom->game->show_loading = 1;
		doom->game->won_the_game = -1;
		render_losing_screen(doom, 1);
	}
}
