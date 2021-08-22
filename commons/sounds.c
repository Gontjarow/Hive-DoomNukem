/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 16:26:08 by krusthol          #+#    #+#             */
/*   Updated: 2021/08/22 19:52:12 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	destroy_more_sounds(t_doom *doom)
{
	Mix_FreeChunk(doom->sounds->mcThunder);
	Mix_FreeChunk(doom->sounds->mcSteam);
	Mix_FreeChunk(doom->sounds->mcSword);
	Mix_FreeChunk(doom->sounds->mcPlop);
	Mix_FreeChunk(doom->sounds->mcPistolRld);
	Mix_FreeChunk(doom->sounds->mcPistolShot);
	Mix_FreeChunk(doom->sounds->mcAssaultRld);
	Mix_FreeChunk(doom->sounds->mcAssaultShot);
	Mix_FreeChunk(doom->sounds->mcShotgunRld);
	Mix_FreeChunk(doom->sounds->mcShotgunShot);
	Mix_FreeChunk(doom->sounds->mcWalking);
	Mix_FreeChunk(doom->sounds->mcRunning);
	Mix_FreeChunk(doom->sounds->mcCrouching);
	Mix_FreeChunk(doom->sounds->mcEnemyDeath);
	Mix_FreeChunk(doom->sounds->mcHealthPickup);
	Mix_FreeChunk(doom->sounds->mcAmmoPickup);
	Mix_FreeChunk(doom->sounds->mcWeaponPickup);
	Mix_FreeChunk(doom->sounds->mcBoss);
	Mix_FreeChunk(doom->sounds->mcBackground);
	Mix_FreeChunk(doom->sounds->mcLoading);
	Mix_FreeChunk(doom->sounds->mcJetpack);
	Mix_FreeChunk(doom->sounds->mcLaserShot);
	Mix_FreeChunk(doom->sounds->mcDogShot);
	Mix_FreeChunk(doom->sounds->mcDogDying);
	Mix_FreeChunk(doom->sounds->mcOutAmmo);
}

static void	set_sounds_null(t_doom *doom)
{
	doom->sounds->mcThunder = NULL;
	doom->sounds->mcSteam = NULL;
	doom->sounds->mcPlop = NULL;
	doom->sounds->mcSword = NULL;
	doom->sounds->mcPistolRld = NULL;
	doom->sounds->mcPistolShot = NULL;
	doom->sounds->mcAssaultRld = NULL;
	doom->sounds->mcAssaultShot = NULL;
	doom->sounds->mcShotgunRld = NULL;
	doom->sounds->mcShotgunShot = NULL;
	doom->sounds->mcWalking = NULL;
	doom->sounds->mcRunning = NULL;
	doom->sounds->mcCrouching = NULL;
	doom->sounds->mcEnemyDeath = NULL;
	doom->sounds->mcHealthPickup = NULL;
	doom->sounds->mcAmmoPickup = NULL;
	doom->sounds->mcWeaponPickup = NULL;
	doom->sounds->mcBoss = NULL;
	doom->sounds->mcBackground = NULL;
	doom->sounds->mcLoading = NULL;
	doom->sounds->mcJetpack = NULL;
	doom->sounds->mcLaserShot = NULL;
	doom->sounds->mcDogShot = NULL;
	doom->sounds->mcDogDying = NULL;
	doom->sounds->mcOutAmmo = NULL;
}

void	destroy_sounds(t_doom *doom)
{
	// 25 allocated sound chunks -> 25 free chunk calls - done
	destroy_more_sounds(doom);
	set_sounds_null(doom);
	free(doom->sounds);
	doom->sounds = NULL;
}
