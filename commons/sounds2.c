/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:46:36 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/22 19:52:05 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	load_sounds4(t_doom *doom)
{
	doom->sounds->mcDogDying = Mix_LoadWAV(WAV_DOGDYING);
	if (!(doom->sounds->mcDogDying))
		ft_die("Fatal error: SDL_mixer failed to load WAV_DOGDYING!");
	doom->sounds->mcOutAmmo = Mix_LoadWAV(WAV_OUTAMMO);
	if (!(doom->sounds->mcOutAmmo))
		ft_die("Fatal error: SDL_mixer failed to load WAV_OUTAMMO!");
	doom->sounds->footstep_delay = 0.0;
	doom->sounds->jetpack_delay = 0.0;
}

static void	load_sounds3(t_doom *doom)
{
	doom->sounds->mcAmmoPickup = Mix_LoadWAV(WAV_AMMOPICKUP);
	if (!(doom->sounds->mcAmmoPickup))
		ft_die("Fatal error: SDL_mixer failed to load WAV_AMMOPICKUP!");
	doom->sounds->mcWeaponPickup = Mix_LoadWAV(WAV_WEAPONPICKUP);
	if (!(doom->sounds->mcWeaponPickup))
		ft_die("Fatal error: SDL_mixer failed to load WAV_WEAPONPICKUP!");
	doom->sounds->mcLoading = Mix_LoadWAV(WAV_LOADINGSONG);
	if (!(doom->sounds->mcLoading))
		ft_die("Fatal error: SDL_mixer failed to load WAV_LOADINGSONG!");
	doom->sounds->mcBoss = Mix_LoadWAV(WAV_BOSS);
	if (!(doom->sounds->mcBoss))
		ft_die("Fatal error: SDL_mixer failed to load WAV_BOSS!");
	doom->sounds->mcBackground = Mix_LoadWAV(WAV_BACKGROUND);
	if (!(doom->sounds->mcBackground))
		ft_die("Fatal error: SDL_mixer failed to load WAV_BACKGROUND!");
	doom->sounds->mcJetpack = Mix_LoadWAV(WAV_JETPACK);
	if (!(doom->sounds->mcJetpack))
		ft_die("Fatal error: SDL_mixer failed to load WAV_JETPACK!");
	doom->sounds->mcLaserShot = Mix_LoadWAV(WAV_LASERSHOT);
	if (!(doom->sounds->mcLaserShot))
		ft_die("Fatal error: SDL_mixer failed to load WAV_LASERSHOT!");
	doom->sounds->mcDogShot = Mix_LoadWAV(WAV_DOGSHOT);
	if (!(doom->sounds->mcDogShot))
		ft_die("Fatal error: SDL_mixer failed to load WAV_DOGSHOT!");
	load_sounds4(doom);
}

static void	load_sounds2(t_doom *doom)
{
	doom->sounds->mcShotgunRld = Mix_LoadWAV(WAV_SHOTGUNRLD);
	if (!(doom->sounds->mcShotgunRld))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SHOTGUNRLD!");
	doom->sounds->mcWalking = Mix_LoadWAV(WAV_WALKING);
	if (!(doom->sounds->mcWalking))
		ft_die("Fatal error: SDL_mixer failed to load WAV_WALKING!");
	doom->sounds->mcRunning = Mix_LoadWAV(WAV_RUNNING);
	if (!(doom->sounds->mcRunning))
		ft_die("Fatal error: SDL_mixer failed to load WAV_RUNNING!");
	doom->sounds->mcCrouching = Mix_LoadWAV(WAV_CROUCHING);
	if (!(doom->sounds->mcCrouching))
		ft_die("Fatal error: SDL_mixer failed to load WAV_CROUCHING!");
	doom->sounds->mcEnemyDeath = Mix_LoadWAV(WAV_ENEMYDEATH);
	if (!(doom->sounds->mcEnemyDeath))
		ft_die("Fatal error: SDL_mixer failed to load WAV_ENEMYDEATH!");
	doom->sounds->mcAssaultShot = Mix_LoadWAV(WAV_ASSAULTSHOT);
	if (!(doom->sounds->mcAssaultShot))
		ft_die("Fatal error: SDL_mixer failed to load WAV_ASSAULTSHOT!");
	doom->sounds->mcShotgunShot = Mix_LoadWAV(WAV_SHOTGUNSHOT);
	if (!(doom->sounds->mcShotgunShot))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SHOTGUNSHOT!");
	doom->sounds->mcHealthPickup = Mix_LoadWAV(WAV_HEALTHPICKUP);
	if (!(doom->sounds->mcHealthPickup))
		ft_die("Fatal error: SDL_mixer failed to load WAV_HEALTHPICKUP!");
	load_sounds3(doom);
}

void	load_sounds(t_doom *doom)
{
	doom->sounds = (t_sounds *)malloc(sizeof(t_sounds));
	if (!(doom->sounds))
		ft_die("Fatal error: Failed mallocing doom->sounds at init_doom.");
	doom->sounds->mcThunder = Mix_LoadWAV(WAV_THUNDER);
	if (!(doom->sounds->mcThunder))
		ft_die("Fatal error: SDL_mixer failed to load WAV_THUNDER!");
	doom->sounds->mcSteam = Mix_LoadWAV(WAV_STEAM0);
	if (!(doom->sounds->mcSteam))
		ft_die("Fatal error: SDL_mixer failed to load WAV_STEAM0!");
	doom->sounds->mcPlop = Mix_LoadWAV(WAV_PLOP);
	if (!(doom->sounds->mcPlop))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PLOP!");
	doom->sounds->mcSword = Mix_LoadWAV(WAV_SWORD);
	if (!(doom->sounds->mcSword))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SWORD!");
	doom->sounds->mcPistolRld = Mix_LoadWAV(WAV_PISTOLRLD);
	if (!(doom->sounds->mcPistolRld))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PISTOLRLD!");
	doom->sounds->mcPistolShot = Mix_LoadWAV(WAV_PISTOLSHOT);
	if (!(doom->sounds->mcPistolShot))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PISTOLSHOT!");
	doom->sounds->mcAssaultRld = Mix_LoadWAV(WAV_ASSAULTRLD);
	if (!(doom->sounds->mcAssaultRld))
		ft_die("Fatal error: SDL_mixer failed to load WAV_ASSAULTRLD!");
	load_sounds2(doom);
}
