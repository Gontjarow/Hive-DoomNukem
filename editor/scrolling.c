#include "doom-nukem.h"

void				draw_scroll_bars_to_backbuffer(t_state *state)
{
	t_line	line;

	line.doom = doom_ptr();
	line.color = 0xffffffff;
	line.buff = editor_back_buffer()->buff;
	// Lines that bound the bottom and left edge of screen
	line.x1 = 0;
	line.y1 = EDT_WIN_HEIGHT - 10;
	line.x2 = EDT_WIN_WIDTH - 10;
	line.y2 = line.y1;
	render_line(&line);
	line.x1 = line.x2;
	line.y1 = 0;
	line.y2 = EDT_WIN_HEIGHT - 10;
	render_line(&line);
	// Line that represents the horizontal scroll
	line.x1 = (int)((float)state->scroll_x * 0.25f);
	line.x2 = line.x1 + (int)((float)EDT_WIN_WIDTH * 0.25f * (float)state->zoom_factor) - 5;
	line.y1 = EDT_WIN_HEIGHT - 5;
	line.y2 = line.y1;
	render_line(&line);
	// Line that represents the vertical scroll
	line.y1 = (int)((float)state->scroll_y * 0.25f);
	line.y2 = line.y1 + (int)((float)EDT_WIN_HEIGHT * 0.25f * (float)state->zoom_factor) - 5;
	line.x1 = EDT_WIN_WIDTH - 5;
	line.x2 = line.x1;
	render_line(&line);
	editor_back_buffer()->rendering_on = 1;
}

/*
 * Call this function like accelerate_scroll(get_state(), SDL_SCANCODE_LEFT) to scroll left,
 * and call this function like accelerate_scroll(NULL, 0) when no key is pressed to decelerate.
 */
void				accelerate_scroll(t_state *state, SDL_Scancode direction)
{
	static int acceleration = 0;

	if (acceleration > 0)
		acceleration--;
	if (state == NULL && direction == 0)
	{
		acceleration /= 2;
		return;
	}
	acceleration += 2 * state->zoom_factor;
	if (direction == SDL_SCANCODE_RIGHT)
	{
		state->scroll_x += acceleration;
		//ft_putendl("Increasing Scroll_X!");
	}
	else if (direction == SDL_SCANCODE_LEFT)
	{
		state->scroll_x -= acceleration;
		//ft_putendl("Decreasing Scroll_X!");
	}
	else if (direction == SDL_SCANCODE_DOWN)
	{
		state->scroll_y += acceleration;
		//ft_putendl("Increasing Scroll_Y!");
	}
	else if (direction == SDL_SCANCODE_UP)
	{
		state->scroll_y -= acceleration;
		//ft_putendl("Decreasing Scroll_Y!");
	}
}

static inline void	set_point(t_point *point, int x, int y)
{
	point->x = x;
	point->y = y;
}

void				confine_scroll(t_state *state)
{
	static t_point	previous;
	int 			zf_mult;

	if (state->scroll_x == previous.x && state->scroll_y == previous.y)
		return ;
	state->scroll_x = (state->scroll_x < 0) ? 0 : state->scroll_x;
	state->scroll_y = (state->scroll_y < 0) ? 0 : state->scroll_y;
	if (state->zoom_factor == 4)
	{
		state->scroll_x = 0;
		state->scroll_y = 0;
	}
	else
	{
		zf_mult = 4 - state->zoom_factor;
		if (state->scroll_x > EDT_WIN_WIDTH * zf_mult)
			state->scroll_x = EDT_WIN_WIDTH * zf_mult;
		if (state->scroll_y > EDT_WIN_HEIGHT * zf_mult)
			state->scroll_y = EDT_WIN_HEIGHT * zf_mult;
	}
	set_point(&previous, state->scroll_x, state->scroll_y);
	redraw_editor_to_backbuffer(0xffffffff);
}

/*
 * Looks horrible, but it simply handles the 8 possible directions with a waterfall early exit.
 * If both left and right pressed, right dominates. If both up and down pressed, up dominates.
 * If none of right, left, up, down pressed, decceleration happens.
 */
void	 		handle_keyboard_scrolling(t_doom *doom)
{
	if (doom->keystates[SDL_SCANCODE_RIGHT] && doom->keystates[SDL_SCANCODE_UP])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_RIGHT);
		accelerate_scroll(get_state(), SDL_SCANCODE_UP);
	}
	else if (doom->keystates[SDL_SCANCODE_RIGHT] && doom->keystates[SDL_SCANCODE_DOWN])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_RIGHT);
		accelerate_scroll(get_state(), SDL_SCANCODE_DOWN);
	}
	else if (doom->keystates[SDL_SCANCODE_LEFT] && doom->keystates[SDL_SCANCODE_UP])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_LEFT);
		accelerate_scroll(get_state(), SDL_SCANCODE_UP);
	}
	else if (doom->keystates[SDL_SCANCODE_LEFT] && doom->keystates[SDL_SCANCODE_DOWN])
	{
		accelerate_scroll(get_state(), SDL_SCANCODE_LEFT);
		accelerate_scroll(get_state(), SDL_SCANCODE_DOWN);
	}
	else if (doom->keystates[SDL_SCANCODE_RIGHT])
		accelerate_scroll(get_state(), SDL_SCANCODE_RIGHT);
	else if (doom->keystates[SDL_SCANCODE_LEFT])
		accelerate_scroll(get_state(), SDL_SCANCODE_LEFT);
	else if (doom->keystates[SDL_SCANCODE_UP])
		accelerate_scroll(get_state(), SDL_SCANCODE_UP);
	else if (doom->keystates[SDL_SCANCODE_DOWN])
		accelerate_scroll(get_state(), SDL_SCANCODE_DOWN);
	else
		accelerate_scroll(NULL, 0);
	confine_scroll(get_state());
}
