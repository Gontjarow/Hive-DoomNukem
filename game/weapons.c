/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 20:21:46 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/11 16:46:23 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void        init_player_weapon(t_doom *doom)
{
    doom->mdl->player.weap_arr[0].ammo_cur = 12;
    doom->mdl->player.weap_arr[0].ammo_max = 12;
    doom->mdl->player.weap_arr[0].cooldown = 10;
    doom->mdl->player.weap_arr[0].dmg = 20;
    doom->mdl->player.weap_arr[0].reload_time = 15;
    // doom->mdl->player.weap_arr[0].fire_sound = "";       // WIP
    // doom->mdl->player.weap_arr[0].reload_sound = "";     // WIP

    doom->mdl->player.weap_arr[1].ammo_cur = 30;
    doom->mdl->player.weap_arr[1].ammo_max = 30;
    doom->mdl->player.weap_arr[1].cooldown = 5;
    doom->mdl->player.weap_arr[1].dmg = 25;
    doom->mdl->player.weap_arr[1].reload_time = 25;
    // doom->mdl->player.weap_arr[1].fire_sound = "";       // WIP
    // doom->mdl->player.weap_arr[1].reload_sound = "";     // WIP

    doom->mdl->player.weap_arr[2].ammo_cur = 40;
    doom->mdl->player.weap_arr[2].ammo_max = 40;
    doom->mdl->player.weap_arr[2].cooldown = 2;
    doom->mdl->player.weap_arr[2].dmg = 30;
    doom->mdl->player.weap_arr[2].reload_time = 35;
    // doom->mdl->player.weap_arr[1].fire_sound = "";       // WIP
    // doom->mdl->player.weap_arr[1].reload_sound = "";     // WIP
}
