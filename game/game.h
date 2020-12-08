/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/08 20:33:50 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

#include "doom-nukem.h"

double          deg_to_rad(int deg);
unsigned int    check_location(t_doom *doom, int x, int y);

int             player_shoots(t_doom *doom);

int             player_collision_with_enemies(t_doom *doom);
int             enemy_collision_with_enemies(t_doom *doom, t_enemy *enemy);
int             check_hit(t_doom *doom);
int             point_circle(double px, double py, double cx, double cy);
int             line_point(double x1, double y1, double x2, double y2, double px, double py);
double          dist(double px, double py, double x1, double y1);

void            rotate_enemy_towards_player(t_doom *doom);
void            move_enemy_towards_player(t_doom *doom);

void            update_player_weapon(t_doom *doom);

#endif