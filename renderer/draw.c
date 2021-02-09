#include "renderer.h"

void			drawline(t_xy_line line, SDL_Surface *surface)
{
	t_xy	length;
	t_xy	ratio;
	t_xy	pos;
	signed	pixels;

	length = vec2(
		fabs(line.stop.x - line.start.x),
		fabs(line.stop.y - line.start.y));
	ratio.x = (line.start.y != line.stop.y) ? (length.x / length.y) : 1;
	ratio.y = (line.start.x != line.stop.x) ? (length.y / length.x) : 1;
	ratio.x = (ratio.x > ratio.y) ? 1 : ratio.x;
	ratio.y = (ratio.y > ratio.x) ? 1 : ratio.y;
	pos = line.start;
	pixels = (length.x > length.y) ? length.x : length.y;
	while (pixels-- > 0)
	{
		set_pixel(surface, pos.x, pos.y, line.color);
		pos.x += ratio.x * ((line.start.x < line.stop.x) ? 1 : -1);
		pos.y += ratio.y * ((line.start.y < line.stop.y) ? 1 : -1);
	}
}

void			draw_box(t_xy center, int radius, int color, SDL_Surface *surface)
{
	center.x = center.x - radius;
	center.y = center.y - radius;

	int size = radius * 2;
	for (int x = 0; x < size; ++x)
	for (int y = 0; y < size; ++y)
		set_pixel(surface, center.x + x, center.y + y, color);
}

void			vertical_line(int column, int start, int end, int color)
{
	int *pixel;
	int draw;

	pixel = (int*)doom_ptr()->game->buff->pixels;
	start = clamp(start, 0, GAME_WIN_HEIGHT - 1);
	end = clamp(end, 0, GAME_WIN_HEIGHT - 1);

	// Always draw at least one pixel.
	draw = 1 + (end - start);
	while (draw-- > 0)
	{
		pixel[GAME_WIN_WIDTH * (start + draw) + column] = color;
	}
}

static unsigned	texture_pixel(SDL_Surface *tex, int x, int y)
{
	unsigned	*sprite;
	unsigned	pixel;
	unsigned	size;

	sprite = tex->pixels;
	pixel = tex->w * y + x;
	size = tex->w * tex->h;
	if ((pixel < size) && ((sprite[pixel] >> 24) != BYTE_TRANSPARENT))
		return(sprite[pixel]);
	else
		return (COLOR_TRANSPARENT);
}

void			vertical_wall(int screen_x, double tex_x, t_xy range, SDL_Surface *tex)
{
	unsigned	*pixels;
	unsigned	color;
	double		y_step;
	double		tex_y;

	tex_y = 0;
	y_step = tex->h / (range.y - range.x);
	if (range.x < 0)
	{
		tex_y += y_step * -range.x;
		range.x = 0;
	}
	pixels = doom_ptr()->game->buff->pixels;
	tex_x = clamp(tex_x, 0, 1);
	while (range.x <= range.y && range.x < GAME_WIN_HEIGHT)
	{
		color = texture_pixel(tex, tex_x * tex->w, tex_y);
		if (color != COLOR_TRANSPARENT)
			pixels[GAME_WIN_WIDTH * (int)range.x + screen_x] = color;
		tex_y += y_step;
		range.x++;
	}
}

void			vertical_floor(int screen_x, t_xy floor_pos, t_xy range, SDL_Surface *tex, t_doom *doom)
{
	unsigned	*pixels;
	unsigned	color;
	t_xy		tex_pos;

	pixels = doom_ptr()->game->buff->pixels;

	// pixel in current FOV
	t_xy current_pixel = floor_pos;

	while (range.x <= range.y && range.x < GAME_WIN_HEIGHT)
	{
		// player -> current_pixel
		tex_pos = vec2_sub(current_pixel, vec32(doom->game->world->player.position));
		// align with the global world (undo FOV rotation)
		tex_pos = vec2_rot(tex_pos, -(doom->game->world->player.angle + M_PI));
		// this pixel's location in the global world
		tex_pos = vec2_add(tex_pos, vec32(doom->game->world->player.position));

		// stick within texture coordinates
		// note: global world corresponds with repeating texture space
		tex_pos.x = fmod(tex_pos.x, tex->w);
		tex_pos.y = fmod(tex_pos.y, tex->h);

		// wrap around texture coordinates
			 if (tex_pos.x < 0)       tex_pos.x += tex->w;
		else if (tex_pos.x >= tex->w) tex_pos.x -= tex->w;

			 if (tex_pos.y < 0)       tex_pos.y += tex->h;
		else if (tex_pos.y >= tex->h) tex_pos.y -= tex->h;

		ft_assert(0 <= tex_pos.x && tex_pos.x <= tex->w, "tex_pos 0<=X<=w");
		ft_assert(0 <= tex_pos.y && tex_pos.y <= tex->h, "tex_pos 0<=Y<=h");

		color = texture_pixel(tex, tex_pos.x, tex_pos.y);
		if (color != COLOR_TRANSPARENT)
			pixels[GAME_WIN_WIDTH * (int)range.x + screen_x] = color;

		current_pixel.y++;
		range.x++;
	}
}

void			vertical_sprite(t_enemy *enemy, int screen_x, int tex_x, t_xy range)
{
	unsigned	*pixels;
	unsigned	color;
	double		y_step;
	double		tex_y;

	tex_y = 0;
	y_step = (double)enemy->active_sprite->h / (range.y - range.x);
	if (range.x < 0)
	{
		tex_y += y_step * -range.x;
		range.x = 0;
	}
	pixels = doom_ptr()->game->buff->pixels;
	while (range.x <= range.y && range.x < GAME_WIN_HEIGHT)
	{
		color = texture_pixel(enemy->active_sprite, tex_x, tex_y);
		if (color != COLOR_TRANSPARENT)
			pixels[GAME_WIN_WIDTH * (int)range.x + screen_x] = color;
		tex_y += y_step;
		range.x++;
	}
}

double			*get_zbuffer()
{
	static double *zbuffer = NULL;

	if (zbuffer == NULL)
		zbuffer = malloc(GAME_WIN_WIDTH * GAME_WIN_HEIGHT * sizeof(*zbuffer));
	return (zbuffer);
}

int				zbuffer_ok(int index, double depth)
{
	double *zbuffer;

	zbuffer = get_zbuffer();
	if (zbuffer[index] > depth)
	{
		zbuffer[index] = depth;
		return (1);
	}
	// printf("%f  %f\n", zbuffer[index], depth);
	return (0);
}

// Temporary until walls have their own textures:
SDL_Surface		*get_bricks_tex(t_doom *doom)
{
	static SDL_Surface *bricks = NULL;

	if (!bricks)
	{
		bricks = load_texture(doom, "img/vertical.png");
		if (bricks == NULL)
			ft_die("Fatal error: Could not load texture!");
	}

	return (bricks);
}

// Temporary until walls have their own textures:
SDL_Surface		*get_border_tex(t_doom *doom)
{
	static SDL_Surface *bricks = NULL;

	if (!bricks)
	{
		bricks = load_texture(doom, "img/border.png");
		if (bricks == NULL)
			ft_die("Fatal error: Could not load texture!");
	}

	return (bricks);
}