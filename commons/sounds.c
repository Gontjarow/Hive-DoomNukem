/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 16:26:08 by krusthol          #+#    #+#             */
/*   Updated: 2020/12/18 20:52:33 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

// MS TODO: Crouching sound already cropped at crouching.wav | enemy dying sound | 

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
	if (!(doom->sounds->mcPistolRld = Mix_LoadWAV(WAV_PISTOLRLD)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PISTOLRLD!");
	if (!(doom->sounds->mcPistolShot = Mix_LoadWAV(WAV_PISTOLSHOT)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PISTOLSHOT!");
	if (!(doom->sounds->mcAssaultRld = Mix_LoadWAV(WAV_ASSAULTRLD)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_ASSAULTRLD!");
	if (!(doom->sounds->mcSmgRld = Mix_LoadWAV(WAV_SMGRLD)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SMGRLD!");
	if (!(doom->sounds->mcWalking = Mix_LoadWAV(WAV_WALKING)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_WALKING!");
	if (!(doom->sounds->mcRunning = Mix_LoadWAV(WAV_RUNNING)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_RUNNING!");
	doom->sounds->footstep_delay = 0;
}

void	destroy_sounds(t_doom *doom)
{
	Mix_FreeChunk(doom->sounds->mcThunder);
	Mix_FreeChunk(doom->sounds->mcSteam);
	Mix_FreeChunk(doom->sounds->mcSword);
	Mix_FreeChunk(doom->sounds->mcPlop);
	Mix_FreeChunk(doom->sounds->mcPistolRld);
	Mix_FreeChunk(doom->sounds->mcPistolShot);
	Mix_FreeChunk(doom->sounds->mcAssaultRld);
	Mix_FreeChunk(doom->sounds->mcSmgRld);
	Mix_FreeChunk(doom->sounds->mcWalking);
	Mix_FreeChunk(doom->sounds->mcRunning);
	doom->sounds->mcThunder = NULL;
	doom->sounds->mcSteam = NULL;
	doom->sounds->mcPlop = NULL;
	doom->sounds->mcSword = NULL;
	free(doom->sounds);
	doom->sounds = NULL;
}
