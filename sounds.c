/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 16:26:08 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/22 16:28:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	load_sounds(t_doom *doom)
{
	if (!(doom->sounds = (t_sounds*)malloc(sizeof(t_sounds))))
		ft_die("Fatal error: Failed mallocing doom->sounds at init_doom.");
	if (!(doom->sounds->mcThunder = Mix_LoadWAV(WAV_THUNDER)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_THUNDER!");
	if (!(doom->sounds->mcSteam = Mix_LoadWAV(WAV_STEAM0)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_STEAM0!");
	if (!(doom->sounds->mcPlop = Mix_LoadWAV(WAV_PLOP)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PLOP!");
	if (!(doom->sounds->mcSword = Mix_LoadWAV(WAV_SWORD)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SWORD!");
}

void	destroy_sounds(t_doom *doom)
{
	Mix_FreeChunk(doom->sounds->mcThunder);
	Mix_FreeChunk(doom->sounds->mcSteam);
	Mix_FreeChunk(doom->sounds->mcSword);
	Mix_FreeChunk(doom->sounds->mcPlop);
	doom->sounds->mcThunder = NULL;
	doom->sounds->mcSteam = NULL;
	doom->sounds->mcPlop = NULL;
	doom->sounds->mcSword = NULL;
	free(doom->sounds);
	doom->sounds = NULL;
}
