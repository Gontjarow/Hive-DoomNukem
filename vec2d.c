#include "doom_nukem.h"

void	modify_line_length(int len_mod, t_point *start, t_point *end, t_point *new_end)
{
	double dx;
	double dy;
	double length;

	dx = end->x - start->x;
	dy = end->y - start->y;
	if (!(dx == 0 && dy == 0))
		length = sqrt((dx * dx) + (dy * dy));
	if (length != 0)
	{
		dx = dx / length;
		dy = dy / length;
	}
	dx *= len_mod;
	dy *= len_mod;
	new_end->x = start->x + dx;
	new_end->y = start->y + dy;
}

