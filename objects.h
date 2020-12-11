/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:17:53 by krusthol          #+#    #+#             */
/*   Updated: 2020/12/11 17:00:16 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include <stdint.h>

/*
** *
** Bit and bob structs to construct the game data for the Model etc.
** *
*/

struct 					MixChunk;

typedef struct 			s_point
{
	int 				x;
	int 				y;
}						t_point;

typedef struct 			s_tri_sides
{
	double				a;
	double 				b;
	double				c;
}						t_tri_sides;

typedef struct 			s_health
{
	int 				max;
	int 				cur;
}						t_health;

typedef struct 			s_weapon
{
	int 				type_id;
	int 				dmg;
	int 				ammo_cur;
	int 				ammo_max;
	int 				cooldown;
	int 				reload_time;
	struct Mix_Chunk 	*fire_sound;
	struct Mix_Chunk	*reload_sound;
}						t_weapon;

typedef struct 			s_player
{
	double				rot_horizontal;
	double				rot_vertical;
	double 				x;
	double 				y;
	double				bullet_pos_x;
	double				bullet_pos_y;
	int 				rot;
	int					height;
	int					is_jumping;
	int					is_crouching;
	int					is_running;
	int					mov_speed;
	int					max_speed;
	int					min_speed;
	int					run_lock;
	int					crouch_lock;
	int					rot_speed;
	int					shoot_cd;
	int					reload_time;
	int					weap_id;
	struct s_weapon		weap_arr[3];
	struct s_point		tail;
	struct s_health		hp;
	struct s_weapon		wep;
}						t_player;

typedef struct 			s_enemy
{
	int 				id;
	int 				x;
	int 				y;
	int 				rot;
	int					did_shoot;
	int					who_shot;
	uint32_t			ray_color;
	double				bullet_pos_x;
	double				bullet_pos_y;
	struct s_point		tail;
	struct s_health		hp;
	struct s_weapon		wep;
	struct s_enemy		*next;
}						t_enemy;

typedef struct 			s_wall
{
	struct s_point		start;
	struct s_point		end;
	int 				id;
	struct s_wall		*next;
}						t_wall;

typedef struct 			s_room
{
	struct s_wall		*first_wall;
	int 				first_wall_id;
	int 				id;
	int 				wall_count;
	int 				floor_height;
	int 				roof_height;
	t_point 			visual;
	struct s_room		*next;
}						t_room;

typedef struct 			s_mapfile
{
	char 				*wall_string;
	char 				*room_string;
	char 				*portal_string;
	char 				*enemy_string;
	char 				*player_string;
	char 				*join_string;
	int 				was_filled;
}						t_mapfile;

#endif
