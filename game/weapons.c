/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 20:21:46 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/08 20:39:05 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void        update_player_weapon(t_doom *doom)
{
    if (doom->mdl->player.wep.type_id == 1)          // Pistol?
    {
        doom->mdl->player.wep.ammo_cur = 12;
        doom->mdl->player.wep.ammo_max = 12;
        doom->mdl->player.wep.cooldown = 10;
        doom->mdl->player.wep.dmg = 20;
        doom->mdl->player.wep.reload_time = 15;
        // doom->mdl->player.wep.fire_sound = "";       WIP
        // doom->mdl->player.wep.reload_sound = "";     WIP
    }
    else if (doom->mdl->player.wep.type_id == 2)     // SubMachine?
    {
        doom->mdl->player.wep.ammo_cur = 30;
        doom->mdl->player.wep.ammo_max = 30;
        doom->mdl->player.wep.cooldown = 5;
        doom->mdl->player.wep.dmg = 25;
        doom->mdl->player.wep.reload_time = 25;
        // doom->mdl->player.wep.fire_sound = "";       WIP
        // doom->mdl->player.wep.reload_sound = "";     WIP
    }
    else if (doom->mdl->player.wep.type_id == 3)    // Assault Rifle?
    {
        doom->mdl->player.wep.ammo_cur = 40;
        doom->mdl->player.wep.ammo_max = 40;
        doom->mdl->player.wep.cooldown = 1;
        doom->mdl->player.wep.dmg = 30;
        doom->mdl->player.wep.reload_time = 35;
        // doom->mdl->player.wep.fire_sound = "";       WIP
        // doom->mdl->player.wep.reload_sound = "";     WIP
    }
    else if (doom->mdl->player.wep.type_id == 4)    // Shotgun?
    {
        doom->mdl->player.wep.ammo_cur = 12;
        doom->mdl->player.wep.ammo_max = 12;
        doom->mdl->player.wep.cooldown = 20;
        doom->mdl->player.wep.dmg = 50;
        doom->mdl->player.wep.reload_time = 40;
        // doom->mdl->player.wep.fire_sound = "";       WIP
        // doom->mdl->player.wep.reload_sound = "";     WIP
    }
}