/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 18:07:53 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/20 18:09:44 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

#include "objects.h"

/*
** *
** Level Editor struct
** *
*/

struct 					s_player;
struct 					s_point;
typedef struct 			s_doom t_doom;

typedef struct 			s_editor
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	struct s_wall		*walls;
	struct s_wall		*wall_begin;
	struct s_wall		*portals;
	struct s_wall		*portal_begin;
	int 				wall_count;
	int 				portal_count;
	int 				enemy_count;
	struct s_player		player;
	struct s_enemy		*enemies;
	struct s_enemy		*enemy_first;
	struct s_point		tail;
	struct s_point		last_enemy;
	int 				is_wall_start;
	char 				*wall_string;
	char 				*portal_string;
	char 				*enemy_string;
	char 				*player_string;
	char 				*join_string;
	int 				polygon_start_x;
	int 				polygon_start_y;
	int 				polygon_binding;
	int 				portalization_binding;
	int 				portalization_ending;
	int					portal_x;
	int 				portal_y;
	int					new_portal_x;
	int					new_portal_y;
	int 				player_set;
	int 				enemy_set;
	struct s_wall		*portalization_a;
	struct s_wall		*portalization_b;
	struct s_wall		*new_portal;
	int 				load_map;
	int 				write_maps;
	int 				overwrite_map;
	char 				*map_path;
	struct s_doom		*parent;
}						t_editor;

void			transfer_model_to_editor(t_doom *doom);
void 			init_edt(t_doom *doom, int argc, char **argv);
void	 		destroy_edt(t_doom *doom);
void			edt_mouse_motion(t_doom *doom);
void 			edt_mouse_down(t_doom *doom);
void			edt_render(t_doom *doom);

#endif
