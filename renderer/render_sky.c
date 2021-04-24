#include "renderer.h"

/*
** Copy src into screen buffer,
** starting from x-th pixel on the X axis
** and up to w pixels on the Y axis.
*/
static void	blit(SDL_Surface *src, int x, int w, SDL_Surface *buff)
{
	int _x;
	int _y;

	_y = 0;
	while (_y < GAME_WIN_HEIGHT && (_y < src->h))
	{
		_x = 0;
		while (_x < GAME_WIN_WIDTH && (_x < w) && (_x + x < src->w))
		{
			set_pixel(buff, _x, _y, get_pixel(src, x + _x, _y));
			++_x;
		}
		++_y;
	}
}

#define TEX_CENTER_ANGLE 180
#define FOV_BY_TWO 45

void	render_sky(t_doom *doom)
{
	SDL_Surface *panorama;
	double	pixels_per_angle;
	int		angle;
	int		start;
	int 	stop;

	panorama = get_panorama_tex(doom);
	pixels_per_angle = panorama->w / 360.0;
	angle = get_model()->player.rot - 180;
	start = (TEX_CENTER_ANGLE + (angle - FOV_BY_TWO));
	stop = (TEX_CENTER_ANGLE + (angle + FOV_BY_TWO));
	blit(panorama, start * pixels_per_angle, GAME_WIN_WIDTH, doom->game->buff);
	if (start < 0)
	{
		start += 360;
		stop = 360;
		blit(panorama, start * pixels_per_angle,
			(stop - start) * pixels_per_angle, doom->game->buff);
	}
	else if (stop > 360 - FOV_BY_TWO)
	{
		start -= 360;
		blit(panorama, start * pixels_per_angle,
			(stop - start) * pixels_per_angle, doom->game->buff);
	}
}
