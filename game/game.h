/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:59:05 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/05 17:51:52 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

#include "doom-nukem.h"

# define GAME_WIN_WIDTH 1024
# define GAME_WIN_HEIGHT 512
# define GAME_MIDWIDTH (GAME_WIN_WIDTH / 2)
# define GAME_MIDHEIGHT (GAME_WIN_HEIGHT / 2)
# define GAME_POLYMAP_WIDTH 6400
# define GAME_POLYMAP_HEIGHT 3600

typedef	struct			s_world t_world;
typedef struct			s_game
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct s_model		*mdl;
	struct s_world		*world;
	struct s_doom		*parent;
	char 				*map_path;
	int 				map_supplied;
}						t_game;

double			deg_to_rad(int deg);
unsigned int	check_location(t_doom *doom, int x, int y);

int				player_shoots(t_doom *doom);
int				player_collision_with_enemies(t_doom *doom);
int				player_collision_with_pickup(t_doom *doom, t_pickup *pickup);
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
void			handle_pickup(t_doom *doom);
void			game_print_alphabet(const char *str, t_doom *doom, int x, int y);
void			game_print_numbers(const char *str, t_doom *doom, int x, int y);
void			init_hud(t_doom *doom);
void            render_hud(t_doom *doom);
void			handle_game_hud(t_doom *doom);
void			validate_player_position(t_doom *doom, t_coord old);
t_pickup		*delete_first(t_doom *doom);
t_pickup		*delete_node(t_pickup *cur, t_pickup *prev, t_pickup *pickup);

void			update_player_tail(t_doom *doom, double rad);
void			rotate_enemy_towards_player(t_doom *doom, t_enemy *enemy);
void			move_enemy_towards_player(t_doom *doom, t_enemy *enemy);
void			enemy_update_cooldown(t_doom *doom, t_enemy *enemy);
void			enemy_shoot_the_player(t_doom *doom, t_enemy *enemy);
void			handle_enemy_ai(t_doom *doom);
void			handle_enemy_shooting(t_doom *doom, t_enemy *enemy);
void			handle_enemy_movement(t_enemy *enemy, t_doom *doom, t_point old);
void			ai_assignment(t_doom *doom);
int				check_hit_on_player(t_doom *doom, t_enemy *enemy);
int				enemy_collision(t_doom *doom, t_enemy *enemy);
int				calc_distance(t_enemy *enemy, t_doom *doom);

void			animation_switch(t_enemy *enemy, t_doom *doom);
void			animate_ranged_back_walk(t_enemy *enemy, t_doom *doom);
void			animate_ranged_front_walk(t_enemy *enemy, t_doom *doom);
void			animate_ranged_side_walk(t_enemy *enemy, t_doom *doom);
void			animate_ranged_death(t_enemy *enemy, t_doom *doom);
void			animate_ranged_front_attack(t_enemy *enemy, t_doom *doom);
void			animate_ranged_side_attack(t_enemy *enemy, t_doom *doom);
void			init_enemy_sprite(t_doom *doom);

#endif
