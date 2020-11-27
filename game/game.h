/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2020/11/27 18:54:19 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

#include "doom-nukem.h"

double          deg_to_rad(int deg);
unsigned int    check_location(t_doom *doom, int x, int y);

int             player_shoots(t_doom *doom);

int             player_collision_with_enemies(t_doom *doom);
int             check_hit(t_doom *doom);

void            rotate_enemy_towards_player(t_doom *doom);

#endif