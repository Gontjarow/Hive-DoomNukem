#include "renderer.h"

#define TEX_CENTER_ANGLE 180
#define FOV_BY_TWO 45

void	render_sky(t_doom *doom)
{
	SDL_Surface *bg = get_panorama_tex(doom);

	double	pixels_per_angle = bg->w / 360.0;
	int	angle	= get_model()->player.rot - 180;
	int	start	= (TEX_CENTER_ANGLE + (angle - FOV_BY_TWO));
	int stop	= (TEX_CENTER_ANGLE + (angle + FOV_BY_TWO));
	printf("\n\n");

	{
		// printf("normal blit      (angle %4i start %4i stop %4i)\n", angle, start, stop);

		SDL_Rect	screen;
		screen.x = 0;
		screen.y = 0;
		screen.w = GAME_WIN_WIDTH;
		screen.h = GAME_WIN_HEIGHT;

		SDL_Rect	image = screen;
		image.x = start * pixels_per_angle;
		SDL_BlitSurface(bg, &image, doom->game->buff, &screen);
	}

	if (start < 0)
	{
		// printf("start < 0        (angle %4i start %4i stop %4i)\n", angle, start, stop);
		start += 360;
		stop = 360;

		SDL_Rect	screen;
		screen.x = 0;
		screen.y = 0;
		screen.w = GAME_WIN_WIDTH;
		screen.h = GAME_WIN_HEIGHT;

		SDL_Rect	image = screen;
		image.x = start * pixels_per_angle;
		image.w = (stop - start) * pixels_per_angle;
		// printf("                 (x %4i, w %4i) (x %4i)\n", image.x, image.w, screen.x);
		SDL_BlitSurface(bg, &image, doom->game->buff, &screen);
	}
	else if (stop > 360 - FOV_BY_TWO)
	{
		// printf("stop > 315       (angle %4i start %4i stop %4i)\n", angle, start, stop);
		start -= 360;

		SDL_Rect	screen;
		screen.x = 0;
		screen.y = 0;
		screen.w = GAME_WIN_WIDTH;
		screen.h = GAME_WIN_HEIGHT;

		SDL_Rect	image = screen;
		image.x = start * pixels_per_angle;
		image.w = (stop - start) * pixels_per_angle;
		// printf("                 (x %4i, w %4i) (x %4i)\n", image.x, image.w, screen.x);
		SDL_BlitSurface(bg, &image, doom->game->buff, &screen);
	}
}
