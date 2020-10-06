/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:17:53 by krusthol          #+#    #+#             */
/*   Updated: 2020/10/06 04:30:06 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

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
	int 				x;
	int 				y;
	int 				rot;
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

typedef struct 			s_mapfile
{
	char 				*wall_string;
	char 				*portal_string;
	char 				*enemy_string;
	char 				*player_string;
	char 				*join_string;
	int 				was_filled;
}						t_mapfile;

#endif
