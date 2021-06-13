#include "doom-nukem.h"

t_point			relative_position(int x, int y, t_state *state)
{
	int relative_x;
	int relative_y;

	relative_x = state->scroll_x;
	relative_y = state->scroll_y;
	x *= state->zoom_factor;
	y *= state->zoom_factor;
	relative_x += x;
	relative_y += y;
	//printf("relativized position to  ( %d | %d )\n", relative_x, relative_y);
	return ((t_point){relative_x, relative_y});
}

t_point         scroll_adjusted(t_point point)
{
    return (scrolled_position(point.x, point.y, get_state()));
}

t_point			scrolled_position(int x, int y, t_state *state)
{
	int relative_x;
	int relative_y;

	relative_x = x - state->scroll_x;
	relative_y = y - state->scroll_y;
	relative_x /= state->zoom_factor;
	relative_y /= state->zoom_factor;
	//printf("scrolled position ( %d | %d ) to ( %d | %d )\n", x, y, relative_x, relative_y);
	return ((t_point){relative_x, relative_y});
}

uint32_t	type_colors(int type)
{
	static uint32_t t_colors[2] = { COLOR_PLAYER, COLOR_ENEMY };

	return (t_colors[type]);
}

uint32_t	pickup_color(int flavor)
{
	if (flavor == PICKUP_HEALTH)
		return (COLOR_HEALTH_PICKUP);
	if (flavor == PICKUP_AMMO)
		return (COLOR_AMMO_PICKUP);
	if (flavor == PICKUP_WEAPON)
		return (COLOR_WEAPON_PICKUP);
	if (flavor == PICKUP_KEY)
		return (COLOR_KEY_PICKUP);
	ft_putendl("Warning: Could not match pickup flavor to a color at pickup_color.");
	return (COLOR_LINE);
}

int			valid_line(t_line *line)
{
	if (line->x1 < 0 || line->x1 >= EDT_WIN_WIDTH)
		return (0);
	if (line->x2 < 0 || line->x2 >= EDT_WIN_WIDTH)
		return (0);
	if (line->y1 < 0 || line->y1 >= EDT_WIN_HEIGHT)
		return (0);
	if (line->y2 < 0 || line->y2 >= EDT_WIN_HEIGHT)
		return (0);
	return (1);
}
