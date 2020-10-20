/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:41:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/14 18:02:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void 		init_edt(t_doom *doom, int argc, char **argv)
{
	int opened;

	doom->edt = (t_editor*)malloc(sizeof(t_editor));
	if (!doom->edt)
		ft_die("Fatal error: Mallocing level editor struct failed at init_edt.");
	doom->edt->parent = doom;
	doom->edt->win = SDL_CreateWindow("DoomNukem Level Editor", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 0);
	doom->edt->buff = SDL_GetWindowSurface(doom->edt->win);
	flood_window(doom->edt->buff, 0xff000000);
	doom->edt->walls = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->walls)
		ft_die("Fatal error: Mallocing walls struct failed at init_edt.");
	doom->edt->rooms = (t_room*)malloc(sizeof(t_room));
	if (!doom->edt->rooms)
		ft_die("Fatal error: Mallocing rooms struct failed at init_edt.");
	doom->edt->portals = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->edt->portals)
		ft_die("Fatal error: Mallocing portals struct failed at init_edt.");
	doom->edt->enemies = (t_enemy*)malloc(sizeof(t_enemy));
	if (!doom->edt->enemies)
		ft_die("Fatal error: Mallocing enemies struct failed at init_edt.");
	doom->edt->wall_begin = NULL;
	doom->edt->portal_begin = NULL;
	doom->edt->enemy_first = NULL;
	doom->edt->room_first = NULL;
	doom->edt->wall_string = NULL;
	doom->edt->room_string = NULL;
	doom->edt->portal_string = NULL;
	doom->edt->join_string = NULL;
	doom->edt->player_string = NULL;
	doom->edt->enemy_string = NULL;
	doom->edt->map_path = NULL;
	doom->edt->portalization_a = NULL;
	doom->edt->portalization_b = NULL;
	doom->edt->new_portal = NULL;
	doom->edt->wall_count = 0;
	doom->edt->room_count = 0;
	doom->edt->portal_count = 0;
	doom->edt->enemy_count = 0;
	doom->edt->player_set = 0;
	doom->edt->enemy_set = 0;
	doom->edt->polygon_binding = 0;
	doom->edt->portalization_binding = 0;
	doom->edt->portalization_ending = 0;
	doom->edt->write_maps = 0;
	doom->edt->player.x = -1;
	doom->edt->player.y = -1;
	doom->edt->player.rot = -1;
	doom->edt->is_wall_start = 1;
	doom->edt->polygon_start_x = -1;
	doom->edt->polygon_start_y = -1;
	doom->edt->portal_x = -1;
	doom->edt->portal_y = -1;
	doom->edt->new_portal_x = -1;
	doom->edt->new_portal_y = -1;
	doom->edt->load_map = 0;
	doom->edt->write_maps = 0;
	doom->edt->overwrite_map = 0;
	if (argc == 2)
	{
		opened = open(argv[1], O_RDONLY);
		if (opened > 1)
		{
			ft_putendl("Hive-DoomNukem: Loading map enabled. Map file detected.");
			doom->edt->load_map = 1;
			doom->edt->overwrite_map = 1;
			close(opened);
		}
		else
		{
			ft_putendl("Hive-DoomNukem: Saving maps enabled. Map file will be written, when closing Level Editor.");
			doom->edt->write_maps = 1;
		}
		doom->edt->map_path = argv[1];
	}
}

void 		destroy_edt(t_doom *doom)
{
	// INCOMPLETE!!! MUST DESTRYOY LINKED LIST STRUCTURES FROM BEGINNING TO END
	// - krusthol
	if (doom->edt->write_maps && doom->edt->wall_count > 0)
		write_mapfile(doom->edt);
	if (doom->edt->overwrite_map)
		overwrite_mapfile(doom->edt);
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	//ft_putendl("Window SDL destructed");
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	if (!doom->mdl)
	{
		free(doom->edt->walls);
		doom->edt->walls = NULL;
		doom->edt->wall_begin = NULL;
		free(doom->edt->rooms);
		doom->edt->rooms = NULL;
		doom->edt->room_first = NULL;
		free(doom->edt->portals);
		doom->edt->portals = NULL;
		doom->edt->portal_begin = NULL;
		free(doom->edt->enemies);
		doom->edt->enemies = NULL;
		doom->edt->enemy_first = NULL;
	}
	if (doom->edt->wall_string)
		free(doom->edt->wall_string);
	if (doom->edt->room_string)
		free(doom->edt->room_string);
	if (doom->edt->portal_string)
		free(doom->edt->portal_string);
	if (doom->edt->enemy_string)
		free(doom->edt->enemy_string);
	if (doom->edt->player_string)
		free(doom->edt->player_string);
	if (doom->edt->join_string)
		free(doom->edt->join_string);
	doom->edt->wall_string = NULL;
	doom->edt->room_string = NULL;
	doom->edt->portal_string = NULL;
	doom->edt->enemy_string = NULL;
	doom->edt->player_string = NULL;
	doom->edt->join_string = NULL;
	doom->edt->portalization_a = NULL;
	doom->edt->portalization_b = NULL;
	doom->edt->new_portal = NULL;
	doom->edt->map_path = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

void 		edt_mouse_motion(t_doom *doom)
{
	doom->edt->polygon_binding = 0;
	if (doom->edt->polygon_start_x > 0 && doom->edt->polygon_start_y > 0)
	{
		if (abs(doom->event.motion.x - doom->edt->polygon_start_x) < 16)
		{
			if (abs(doom->event.motion.y - doom->edt->polygon_start_y) < 16)
				doom->edt->polygon_binding = 1;
		}
	}
}

static void set_portalization_xy(t_editor *edt)
{
	if (edt->portalization_binding)
	{
		if (edt->portalization_a->end.x == edt->portalization_b->start.x &&
			edt->portalization_a->end.y == edt->portalization_b->start.y)
		{
			edt->portal_x = edt->portalization_a->end.x;
			edt->portal_y = edt->portalization_a->end.y;
			//ft_putendl("Set Portalization(TM).");
		}
		else
		{
			ft_putendl("Warning: Portalization A / B comparison error.");
			edt->portal_x = -1;
			edt->portal_y = -1;
			edt->portalization_binding = 0;
		}
	}
	else
	{
		edt->portal_x = -1;
		edt->portal_y = -1;
	}
}

static void test_begin_portalization(int x, int y, t_editor *edt)
{
	int 	wc;
	t_wall	*wall;

	wc = edt->wall_count;
	wall = edt->wall_begin;
	edt->portalization_binding = 0;
	while (wc--)
	{
		if ((abs(x - wall->start.x) < 16) && (abs(y - wall->start.y) < 16))
		{
			edt->portalization_binding = 1;
			edt->portalization_b = wall;
			//ft_putendl("Portalization_binding due to start.x, start.y.");
		}
		else if ((abs(x - wall->end.x) < 16) && (abs(y - wall->end.y) < 16))
		{
			edt->portalization_binding = 1;
			edt->portalization_a = wall;
			//ft_putendl("Portalization_binding due to end.x, end.y.");
		}
		wall = wall->next;
	}
	set_portalization_xy(edt);
}

static void test_end_portalization(int x, int y, t_editor *edt)
{
	edt->new_portal = NULL;
	if ((abs(x - edt->portalization_a->start.x) < 16) && (abs(y - edt->portalization_a->start.y) < 16))
	{
		edt->new_portal = edt->portalization_a;
		edt->new_portal_x = edt->portalization_a->start.x;
		edt->new_portal_y = edt->portalization_a->start.y;
	}
	else if ((abs(x - edt->portalization_b->end.x) < 16) && (abs(y - edt->portalization_b->end.y) < 16))
	{
		edt->new_portal = edt->portalization_b;
		edt->new_portal_x = edt->portalization_b->end.x;
		edt->new_portal_y = edt->portalization_b->end.y;
	}
	//ft_putendl("Tested end_portalization");
}

static void expand_polygon(int x, int y, t_editor *edt)
{
	t_wall *next_wall;

	if (edt->is_wall_start)
	{
		//ft_putstr("Testing begin Portalization(TM): ");
		test_begin_portalization(x, y, edt);
		edt->walls->id = edt->wall_count;
		if (edt->portalization_binding)
		{
			edt->walls->start.x = edt->portal_x;
			edt->walls->start.y = edt->portal_y;
			//ft_putendl("Portalization started.");
		}
		else
		{
			//ft_putendl("Negative.");
			edt->walls->start.x = x;
			edt->walls->start.y = y;
		}
		edt->polygon_start_x = x;
		edt->polygon_start_y = y;
		edt->is_wall_start = 0;
	}
	else if (edt->polygon_binding)
	{
		//ft_putendl("Polygon binding.");
		edt->walls->end.x = edt->polygon_start_x;
		edt->walls->end.y = edt->polygon_start_y;
		expand_wall_string(edt);
		edt->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (edt->wall_count == 1)
			edt->wall_begin = edt->walls;
		edt->walls->next = next_wall;
		edt->walls = next_wall;
		edt->is_wall_start = 1;
		record_room(edt);
		print_walls(edt);
	}
	else
	{
		edt->walls->end.x = x;
		edt->walls->end.y = y;
		if (edt->portalization_ending)
		{
			//ft_putstr("Testing end Portalization(TM): ");
			test_end_portalization(x, y, edt);
			if (edt->new_portal)
			{
				edt->walls->end.x = edt->new_portal_x;
				edt->walls->end.y = edt->new_portal_y;
				record_portal(edt);
				edt->portalization_ending = 0;
				edt->portalization_a = NULL;
				edt->portalization_b = NULL;
				edt->is_wall_start = 1;
				//ft_putendl("Portal created.");
			}
			else
			{
				//ft_putendl("Negative.");
			}
		}
		expand_wall_string(edt);
		edt->wall_count++;
		next_wall = (t_wall*)malloc(sizeof(t_wall));
		if (!next_wall)
			ft_die("Fatal error: Could not malloc t_wall at record_wall.");
		if (edt->wall_count == 1)
			edt->wall_begin = edt->walls;
		edt->walls->next = next_wall;
		edt->walls = next_wall;
		edt->walls->id = edt->wall_count;
		edt->walls->start.x = x;
		edt->walls->start.y = y;
		// PORTALIZATION SUCCEEDED, THEN WE ADD ONE MORE WALL TO COMPLETE THE ROOM POLYGONALLY
		if (edt->is_wall_start)
		{
			edt->walls->start.x = edt->new_portal->start.x;
			edt->walls->start.y = edt->new_portal->start.y;
			edt->walls->end.x = edt->new_portal->end.x;
			edt->walls->end.y = edt->new_portal->end.y;
			expand_wall_string(edt);
			edt->wall_count++;
			next_wall = (t_wall*)malloc(sizeof(t_wall));
			if (!next_wall)
				ft_die("Fatal error: Could not malloc t_wall at record_wall.");
			edt->walls->next = next_wall;
			edt->walls = next_wall;
			edt->walls->id = edt->wall_count;
			record_room(edt);
		}
		print_walls(edt);
	}
}

static void edt_left_click(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	color = 0xffffffff;
	i = doom->event.motion.x;
	j = doom->event.motion.y * EDT_WIN_WIDTH;
	expand_polygon(doom->event.motion.x, doom->event.motion.y, doom->edt);
	if (doom->edt->polygon_binding)
	{
		i = doom->edt->polygon_start_x;
		j = doom->edt->polygon_start_y * EDT_WIN_WIDTH;
		color = 0xff00ff00;
	}
	if (doom->edt->portalization_binding)
	{
		i = doom->edt->portal_x;
		j = doom->edt->portal_y * EDT_WIN_WIDTH;
		color = 0xffff0000;
		doom->edt->portalization_binding = 0;
		doom->edt->portalization_ending = 1;
	}
	if (doom->edt->new_portal_x >= 0)
	{
		i = doom->edt->new_portal_x;
		j = doom->edt->new_portal_y * EDT_WIN_WIDTH;
		color = 0xffff0000;
		doom->edt->new_portal_x = -1;
		doom->edt->new_portal_y = -1;
	}
	pixels = doom->edt->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + EDT_WIN_WIDTH] = color;
	pixels[i + j - EDT_WIN_WIDTH] = color;
	Mix_PlayChannel( -1, doom->sounds->mcPlop, 0 );
}

static void edt_right_click(t_doom *doom)
{
	unsigned int *pixels;
	int j;
	int i;
	uint32_t color;

	color = 0xff00ff00;
	if (doom->edt->player_set < 1)
		color = 0xffffff00;
	i = doom->event.motion.x;
	j = doom->event.motion.y * EDT_WIN_WIDTH;
	if (doom->edt->player_set < 1)
	{
		record_player(doom->event.motion.x, doom->event.motion.y, doom->edt);
		Mix_PlayChannel( -1, doom->sounds->mcSword, 0);
		if (doom->edt->player_set == 1)
			return (circle_player(doom));
	}
	else
	{
		record_enemy(doom->event.motion.x, doom->event.motion.y, doom->edt);
		Mix_PlayChannel( -1, doom->sounds->mcSteam, 0);
		if (doom->edt->enemy_set == 0)
			return (circle_enemy(doom));
	}
	pixels = doom->edt->buff->pixels;
	pixels[i + j] = color;
	pixels[i + j - 1] = color;
	pixels[i + j + 1] = color;
	pixels[i + j + EDT_WIN_WIDTH] = color;
	pixels[i + j - EDT_WIN_WIDTH] = color;
}

void 		edt_mouse_down(t_doom *doom)
{
	if (doom->event.button.button == SDL_BUTTON_RIGHT)
	{
		edt_right_click(doom);
	}
	else if (doom->event.button.button == SDL_BUTTON_LEFT)
	{
		edt_left_click(doom);
	}
}

void		edt_render(t_doom *doom)
{
	SDL_UpdateWindowSurface(doom->edt->win);
}