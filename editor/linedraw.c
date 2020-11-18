#include "doom-nukem.h"

void	init_linedraw_data(void *data_ptr)
{
	t_linedraw	*data;

	data = (t_linedraw*)data_ptr;
	*data = (t_linedraw){0};
}

void	linedraw_to_buffer_safe(t_linedraw *data, SDL_Surface *buff, uint32_t color)
{
	t_line	line;

	line.doom = doom_ptr();
	line.buff = buff;
	line.color = color;
	line.x1 = data->draw_from_x;
	line.y1 = data->draw_from_y;
	line.x2 = data->draw_to_x;
	line.y2 = data->draw_to_y;
	render_line_safe(&line);
}

void	linedraw_to_buffer(t_linedraw *data, SDL_Surface *buff, uint32_t color)
{
	t_line	line;

	line.doom = doom_ptr();
	line.buff = buff;
	line.color = color;
	line.x1 = data->draw_from_x;
	line.y1 = data->draw_from_y;
	line.x2 = data->draw_to_x;
	line.y2 = data->draw_to_y;
	render_line(&line);
}

void	linedraw_to_wall(t_linedraw *data)
{
	t_wall	*wall;

	wall = get_model()->walls;
	wall->id = get_model()->wall_count;
	wall->start.x = data->draw_from_x;
	wall->start.y = data->draw_from_y;
	wall->end.x = data->draw_to_x;
	wall->end.y = data->draw_to_y;
	get_model()->wall_count++;
	wall = (t_wall*)malloc(sizeof(t_wall));
	if (!wall)
		ft_die("Fatal error: Could not malloc t_wall at linedraw_to_wall.");
	if (get_model()->wall_count == 1)
		get_model()->wall_first = get_model()->walls;
	get_model()->walls->next = wall;
	get_model()->walls = wall;
		//ft_putendl("Linedraw_to_wall: Added a wall to model->walls");
}

