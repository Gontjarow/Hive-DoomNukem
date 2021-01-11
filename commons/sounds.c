/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 16:26:08 by krusthol          #+#    #+#             */
/*   Updated: 2021/01/11 19:53:38 by msuarez-         ###   ########.fr       */
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
	if (!(doom->sounds->mcCrouching = Mix_LoadWAV(WAV_CROUCHING)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_CROUCHING!");
	if (!(doom->sounds->mcEnemyDeath = Mix_LoadWAV(WAV_ENEMYDEATH)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_ENEMYDEATH!");
	if (!(doom->sounds->mcAssaultShot = Mix_LoadWAV(WAV_ASSAULTSHOT)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_ASSAULTSHOT!");
	if (!(doom->sounds->mcSmgShot = Mix_LoadWAV(WAV_SMGSHOT)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SMGSHOT!");
	if (!(doom->sounds->mcHealthPickup = Mix_LoadWAV(WAV_HEALTHPICKUP)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_HEALTHPICKUP!");
	if (!(doom->sounds->mcAmmoPickup = Mix_LoadWAV(WAV_AMMOPICKUP)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_AMMOPICKUP!");
	if (!(doom->sounds->mcWeaponPickup = Mix_LoadWAV(WAV_WEAPONPICKUP)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_WEAPONPICKUP!");
	doom->sounds->footstep_delay = 0;
}

void	destroy_sounds(t_doom *doom)
{
	Mix_FreeChunk(doom->sounds->mcThunder);
	Mix_FreeChunk(doom->sounds->mcSteam);
	Mix_FreeChunk(doom->sounds->mcSword);
	Mix_FreeChunk(doom->sounds->mcPlop);
	// commented out because of segfault when pressing ESC on main menu
	// Mix_FreeChunk(doom->sounds->mcPistolRld);
	// Mix_FreeChunk(doom->sounds->mcPistolShot);
	// Mix_FreeChunk(doom->sounds->mcAssaultRld);
	// Mix_FreeChunk(doom->sounds->mcAssaultShot);
	// Mix_FreeChunk(doom->sounds->mcSmgRld);
	// Mix_FreeChunk(doom->sounds->mcSmgShot);
	// Mix_FreeChunk(doom->sounds->mcWalking);
	// Mix_FreeChunk(doom->sounds->mcRunning);
	// Mix_FreeChunk(doom->sounds->mcCrouching);
	// Mix_FreeChunk(doom->sounds->mcEnemyDeath);
	// Mix_FreeChunk(doom->sounds->mcAssaultShot);
	// Mix_FreeChunk(doom->sounds->mcHealthPickup);
	// Mix_FreeChunk(doom->sounds->mcAmmoPickup);
	doom->sounds->mcThunder = NULL;
	doom->sounds->mcSteam = NULL;
	doom->sounds->mcPlop = NULL;
	doom->sounds->mcSword = NULL;
	free(doom->sounds);
	doom->sounds = NULL;
}
