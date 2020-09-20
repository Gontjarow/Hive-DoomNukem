/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 16:32:15 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/20 18:39:17 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	find_walls_quantity(t_doom *doom, char *argv)
{
	int		fd;
	int		i;
	char	*line;

	fd = open(argv, O_RDONLY);
	if (fd < 0)
		ft_die("Error: Failed opening file. File is either invalid or does not exist");
	i = 0;
	while (get_next_line(fd, &line) > 0)
		i++;
	doom->minimap_main->wall_count = i - 1;
	printf("Wall count: %d\n", doom->minimap_main->wall_count);
}

static void	draw_world(t_doom *doom)
{
	t_line	line;

	// printf("Result: wall id:%d start.x:%d start.y:%d end.x:%d end.y:%d\n", doom->minimap_main->wall_id,
	// 	doom->minimap_main->start.x, doom->minimap_main->start.y, doom->minimap_main->end.x,
	// 	doom->minimap_main->end.y);
	while (doom->minimap_main->start.x * doom->minimap_main->scale > 400)
		doom->minimap_main->scale -= 0.1;
	while (doom->minimap_main->start.y * doom->minimap_main->scale > 400)
		doom->minimap_main->scale -= 0.1;
	while (doom->minimap_main->end.x * doom->minimap_main->scale > 400)
		doom->minimap_main->scale -= 0.1;
	while (doom->minimap_main->end.y * doom->minimap_main->scale > 400)
		doom->minimap_main->scale -= 0.1;
	line.x1 = doom->minimap_main->start.x * doom->minimap_main->scale;
	line.y1 = doom->minimap_main->start.y * doom->minimap_main->scale;
	line.x2 = doom->minimap_main->end.x * doom->minimap_main->scale;
	line.y2 = doom->minimap_main->end.y * doom->minimap_main->scale;
	line.color = 0xffffffff;
	line.buff = doom->minimap_buff;
	render_line(&line);
}

static void	read_mapfile(t_doom *doom, char *argv)
{
	int		fd;
	int		i;
	int		j;
	char	*hold;
	int		sector;
	char	*line;

	fd = open(argv, O_RDONLY);
	if (fd < 0)
		ft_die("Error: Failed opening file. File is either invalid or does not exist");
	if (!(hold = (char *)malloc(sizeof(char) * 4096)))
		return ;
	find_walls_quantity(doom, argv);
	while (get_next_line(fd, &line) > 0 && doom->minimap_main->wall_count != doom->minimap_main->wall_id + 1)
	{
		sector = 1;
		j = 0;
		while (line[j])
		{
			if ((line[j] >= '0' && line[j] <= '9') && sector == 1)
			{
				i = 0;
				while (line[j] >= '0' && line[j] <= '9')
				{
					hold[i] = line[j];
					j++;
					i++;
				}
				hold[i] = '\0';
				doom->minimap_main->wall_id = ft_atoi(hold);
				sector++;
			}
			else if ((line[j] >= '0' && line[j] <= '9') && sector == 2)
			{	
				i = 0;
				ft_bzero((void*)hold, ft_strlen(hold));
				while (line[j] >= '0' && line[j] <= '9')
				{
					hold[i] = line[j];
					j++;
					i++;
				}
				doom->minimap_main->start.x = ft_atoi(hold);
				ft_bzero((void *)hold, ft_strlen(hold));
				if (line[j] == ',')
				{
					j += 2;
					i = 0;
					while (line[j] >= '0' && line[j] <= '9')
					{
						hold[i] = line[j];
						j++;
						i++;
					}
					doom->minimap_main->start.y = ft_atoi(hold);
				}
				sector++;
			}
			else if ((line[j] >= '0' && line[j] <= '9') && sector == 3)
			{
				i = 0;
				ft_bzero((void*)hold, ft_strlen(hold));
				while (line[j] >= '0' && line[j] <= '9')
				{
					hold[i] = line[j];
					j++;
					i++;
				}
				doom->minimap_main->end.x = ft_atoi(hold);
				ft_bzero((void *)hold, ft_strlen(hold));
				if (line[j] == ',')
				{
					j += 2;
					i = 0;
					while (line[j] >= '0' && line[j] <= '9')
					{
						hold[i] = line[j];
						j++;
						i++;
					}
					doom->minimap_main->end.y = ft_atoi(hold);
				}
				sector++;
			}
			j++;
		}
		draw_world(doom);
	}
}

void		init_minimap(t_doom *doom, char *argv)
{
	// int		i;
	// t_line	*line;
	// t_wall	*wall;
	int			win_x;
	int			win_y;


	if (!(doom->minimap_main = (t_minimap *)malloc(sizeof(t_minimap))))
		return ;	
	doom->minimap_win = SDL_CreateWindow("Debug Minimap", SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, 400, 400, 0);
	if (doom->minimap_win == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window with SDL_CreateWindow on init_minimap.");
	doom->minimap_buff = SDL_GetWindowSurface(doom->minimap_win);
	if (doom->minimap_buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Surface with SDL_GetWindowSurface on init_minimap.");
	doom->minimap_main->scale = 1.0;
	SDL_GetWindowPosition(doom->win, &win_x, &win_y);
	SDL_SetWindowPosition(doom->minimap_win, win_x + WIN_WIDTH, win_y);
	read_mapfile(doom, argv);
	SDL_UpdateWindowSurface(doom->minimap_win);
}
