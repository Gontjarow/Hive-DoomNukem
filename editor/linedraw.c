#include "doom-nukem.h"

void	init_linedraw_data(void *data_ptr)
{
	t_linedraw	*data;

	data = (t_linedraw*)data_ptr;
	*data = (t_linedraw){0};
}

static void	careful_linedraw_to_buffer_fixed(t_linedraw *data, SDL_Surface *buff, uint32_t color, uint32_t *avoid)
{
	t_line	line;

	line.doom = doom_ptr();
	line.buff = buff;
	line.color = color;
	line.avoid[0] = avoid[0];
	line.avoid[1] = avoid[1];
	line.avoid[2] = avoid[2];
	line.avoid[3] = avoid[3];
	line.x1 = data->draw_from_x;
	line.y1 = data->draw_from_y;
	line.x2 = data->draw_to_x;
	line.y2 = data->draw_to_y;
	careful_render_line(&line);
}

void		careful_linedraw_to_buffer(t_linedraw *data, SDL_Surface *buff, uint32_t color, uint32_t *avoid)
{
	t_linedraw		zoomed_line;
	int 			zoom_factor;

	zoom_factor = get_state()->zoom_factor;
	if (zoom_factor == 1)
		return (careful_linedraw_to_buffer_fixed(data, buff, color, avoid));
	zoomed_line.draw_from_x = data->draw_from_x / zoom_factor;
	zoomed_line.draw_from_y = data->draw_from_y / zoom_factor;
	zoomed_line.draw_to_y = data->draw_to_y;
	zoomed_line.draw_to_x = data->draw_to_x;
	careful_linedraw_to_buffer_fixed(&zoomed_line, buff, color, avoid);
}

/* possibly depreceated
void	linedraw_to_buffer_fixed(t_linedraw *data, SDL_Surface *buff, uint32_t color)
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
*/

t_wall	*linedraw_to_wall(t_linedraw *data)
{
	t_wall	*wall;
	t_wall	*ret_wall;

	wall = get_model()->walls;
	ret_wall = wall;
	wall->id = get_model()->wall_count;
	wall->start.x = data->draw_from_x;
	wall->start.y = data->draw_from_y;
	wall->end.x = data->draw_to_x;
	wall->end.y = data->draw_to_y;
	wall->can_portal = 1;
	wall->texture_id = get_state()->selected_weapon_type;
	//printf("Wall->texture_id was set to %d\n", wall->texture_id);
	wall->active_sprite = NULL;
	wall->portal_type = NOT_A_PORTAL;
	wall->open = 0;
	get_model()->wall_count++;
	wall = (t_wall*)malloc(sizeof(t_wall));
	if (!wall)
		ft_die("Fatal error: Could not malloc t_wall at linedraw_to_wall.");
	if (get_model()->wall_count == 1)
		get_model()->wall_first = get_model()->walls;
	get_model()->walls->next = wall;
	get_model()->walls = wall;
		//ft_putendl("Linedraw_to_wall: Added a wall to model->walls");
	return (ret_wall);
}

