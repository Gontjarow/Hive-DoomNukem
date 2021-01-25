/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 21:42:16 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/22 19:40:22 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H

# include "doom-nukem.h"
# define MWIN_WIDTH 1024
# define MWIN_HEIGHT 512

typedef struct		s_minimap
{
	struct SDL_Window	*win;
	struct SDL_Surface	*buff;
	double				scale;
	int					sprite_num;
	int					enemy_ray_timeout;
	uint32_t			enemy_ray_color;
	int					player_ray_timeout;
	uint32_t			player_ray_color;
}					t_minimap;

void				init_minimap(t_doom *doom);
void				update_minimap(t_doom *doom);
void				print_minimap_player(t_doom *doom);
void				print_minimap_walls(t_doom *doom);
void				print_minimap_enemies(t_doom *doom);
void				print_minimap_pickups(t_doom *doom);
void				print_minimap_weapons(t_doom *doom);
void				print_minimap_mult_sprite(t_doom *doom, SDL_Surface **sprite, int i);		// testing only
void				print_minimap_single_sprite(t_doom *doom, SDL_Surface *sprite);				// testing only
void				destroy_minimap(t_doom *doom);

#endif
