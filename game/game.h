/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2020/12/18 16:46:05 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "doom-nukem.h"

double			deg_to_rad(int deg);
unsigned int	check_location(t_doom *doom, int x, int y);

int				player_shoots(t_doom *doom);
int				player_collision_with_enemies(t_doom *doom);
int				check_hit(t_doom *doom);
int				point_circle(double px, double py, double cx, double cy);
int				line_point(t_doom *doom, t_coord p);
int				is_point_circle(t_enemy *enemy, t_doom *doom);
int				player_check_distance(t_enemy *enemy, t_coord closest);
double			dist(double px, double py, double x1, double y1);
t_coord			player_check_segment(t_doom *doom, t_enemy *enemy);
void			player_update_weapons(t_doom *doom);
void			init_player_weapon(t_doom *doom);
void			handle_player_movement(t_doom *doom);
void			handle_player_action(t_doom *doom);

void			rotate_enemy_towards_player(t_doom *doom);
void			move_enemy_towards_player(t_doom *doom);
void			enemy_update_cooldown(t_doom *doom);
void			enemy_shoot_the_player(t_doom *doom, t_enemy *enemy);
int				check_hit_on_player(t_doom *doom, t_enemy *enemy);
int				calc_dist(t_enemy *enemy, t_doom *doom, t_point old);
int				enemy_collision_with_enemies(t_doom *doom, t_enemy *enemy);

#endif
