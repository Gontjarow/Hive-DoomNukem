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

	{
		// printf("normal blit      (angle %4i start %4i stop %4i)\n", angle, start, stop);

		SDL_Rect	dstrect;
		dstrect.x = 0;
		dstrect.y = 0;
		dstrect.w = GAME_WIN_WIDTH;
		dstrect.h = GAME_WIN_HEIGHT;

		SDL_Rect	srcrect = dstrect;
		srcrect.x = start * pixels_per_angle;
		SDL_BlitSurface(bg, &srcrect, doom->game->buff, &dstrect);
	}

	if (start < 0)
	{
		start += 360;
		stop = 360;
		// printf("start < 0        (angle %4i start %4i stop %4i)\n", angle, start, stop);

		SDL_Rect	dstrect;
		dstrect.x = 0;
		dstrect.y = 0;
		dstrect.w = GAME_WIN_WIDTH;
		dstrect.h = GAME_WIN_HEIGHT;

		SDL_Rect	srcrect = dstrect;
		srcrect.x = start * pixels_per_angle;
		srcrect.w = (stop - start) * pixels_per_angle;
		// printf("                 (x %4i, w %4i) (x %4i)\n", srcrect.x, srcrect.w, dstrect.x);
		SDL_BlitSurface(bg, &srcrect, doom->game->buff, &dstrect);
	}

	if (stop > 360 - FOV_BY_TWO)
	{
		start -= 360;
		// printf("stop > 315       (angle %4i start %4i stop %4i)\n", angle, start, stop);

		SDL_Rect	dstrect;
		dstrect.x = 0;
		dstrect.y = 0;
		dstrect.w = GAME_WIN_WIDTH;
		dstrect.h = GAME_WIN_HEIGHT;

		SDL_Rect	srcrect = dstrect;
		srcrect.x = start * pixels_per_angle;
		srcrect.w = (stop - start) * pixels_per_angle;
		// printf("                 (x %4i, w %4i) (x %4i)\n", srcrect.x, srcrect.w, dstrect.x);
		SDL_BlitSurface(bg, &srcrect, doom->game->buff, &dstrect);
	}
}
