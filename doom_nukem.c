/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/11 14:02:23 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ft_die(const char *error_message)
{
	ft_putendl(error_message);
	exit(0);
}

static void		render_animation(t_doom *doom)
{
	unsigned int	*pixels;
	unsigned int 	*reference;
	SDL_Surface     *ani_surface;
	int 			i;
	int 			j;
	int 			k;
	int 			cutoff;

	pixels = doom->buff->pixels;
	ani_surface = doom->ani_thunder.surfaces[doom->ani_thunder.current];
	i = 0;
	k = 0;
	reference = ani_surface->pixels;
	j = (WIN_WIDTH - ani_surface->w) / 2 + ((WIN_HEIGHT - ani_surface->h) / 2 * WIN_WIDTH);
	cutoff = 45 * WIN_WIDTH;
	j += cutoff;
	while (k < ((ani_surface->w * ani_surface->h) - cutoff))
	{
		j = i == ani_surface->w ? j + WIN_WIDTH : j;
		i = i == ani_surface->w ? 0 : i;
		pixels[i++ + j] = reference[k++];
	}
	print_alphabet("hive doom nukem", doom, 512 - 196, 29);
	print_alphabet("by", doom, 512 - 28, 29 + 29);
	print_alphabet("ngontjar krusthol msuarez", doom, 512 - 364, 29 + 29 + 29);
	print_alphabet("sdl sound and png load demo", doom, 512 - 392, 290 + 29 + 29 + 29 + 29);
	print_alphabet("alphabet font rendering", doom, 512 - 322, 290 + 29 + 29 + 29 + 29 + 29);
	print_alphabet("press esc to quit", doom, 512 - 238, 290 + 29 + 29 + 29 + 29 + 29 + 29);
}

static void		load_animation(t_doom *doom)
{
	int i;
	char *path;
	char *join;

	i = 0;
	doom->ani_thunder.frames = 11;
	doom->ani_thunder.current = 0;
	doom->ani_thunder.surfaces = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * doom->ani_thunder.frames);
	if (doom->ani_thunder.surfaces == NULL)
		ft_die("Fatal error: Could not malloc SDL_Surfaces in doom->ani_thunder.");
	while (i < doom->ani_thunder.frames)
	{
		join = ft_itoa(i);
		path = ft_strjoin("img/thunder/", join);
		free(join);
		join = path;
		path = ft_strjoin(path, ".png");
		free(join);
		if (!(doom->ani_thunder.surfaces[i] = (SDL_Surface*)malloc(sizeof(SDL_Surface))))
			ft_die("Fatal error: Could not malloc SDL_Surface in doom->ani_thunder->surfaces.");
		doom->ani_thunder.surfaces[i] = load_texture(doom, path);
		free(path);
		if (doom->ani_thunder.surfaces[i] == NULL)
			ft_die("Fatal error: Could not load_texture for doom->ani_thunder->surfaces.");
		i++;
		ft_putendl("Loaded animation texture.");
	}
}

static void		init_doom(t_doom *doom)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2 video.");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		ft_die("Fatal error: Failed initialization of SDL2 audio.");
	if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
		ft_die("Fatal error: SDL_mixer could not initialize!");
	doom->win = SDL_CreateWindow("Hive-DoomNukem", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	doom->buff = SDL_GetWindowSurface(doom->win);
	if (doom->win == NULL || doom->buff == NULL)
		ft_die("Fatal error: Failed initialization of SDL_Window or SDL_Surface with SDL_CreateWindow and SDL_GetWindowSurface.");
	if (!(doom->mcThunder = Mix_LoadWAV(WAV_THUNDER)))
		ft_die("Fatal error: SDL_mixer failed to load THUNDER_WAV!");
	doom->quit = 0;
}

int		main(int argc, char **argv)
{
	t_doom			doom;
	const Uint8* 	keystates;
	unsigned int	*pixels;
	unsigned int 	*reference;
	uint32_t 		frame_start;
	uint32_t 		app_start;
	SDL_Surface		*test;
	int 			i;
	int 			j;
	int 			k;
	int				dir_horz;
	int 			dir_vert;

	i = 0;
	dir_horz = 1;
	dir_vert = 1;
	(void)argc;
	init_doom(&doom);
	pixels = doom.buff->pixels;
	i = 0;
	k = 0;
	dir_horz = 1;
	dir_vert = 1;
	keystates = SDL_GetKeyboardState(NULL);
	Mix_PlayChannel( -1, doom.mcThunder, 0 );
	test = NULL;
	// Disabled argv loading/default img_bluebricks demo in favor of thunder animation demo
	//test = (argc > 1) ? load_texture(&doom, argv[1]) : load_texture(&doom, IMG_BLUEBRICKS);
	test = load_texture(&doom, IMG_THUNDER0);
	if (test)
	{

		ft_putstr("Loaded a texture: ");
		ft_putendl(argv[1]);
		ft_putnbr(test->w);
		ft_putstr(" width, height ");
		ft_putnbr(test->h);
		ft_putendl(" px");
		if (test->w > WIN_WIDTH || test->h > WIN_HEIGHT)
			ft_die("Fatal error: Too large sized texture loaded to display on window.");
		// Obfuscated minimal code to draw all pixels from "reference" ie. the texture that was loaded.
		// The pixels are read from reference straight into the window pixel buffer.
		// K is the raw index for reading all single pixels from reference
		// I is the rolling pixel index offset for always moving a pixel to the right every increment on the column
		// Centering offset and total offset two-in-one, J is the 'total' destination pixel index offset as row changes
		reference = test->pixels;
		j = (WIN_WIDTH - test->w) / 2 + ((WIN_HEIGHT - test->h) / 2 * WIN_WIDTH);
		while (k < (test->w * test->h))
		{
			j = i == test->w ? j + WIN_WIDTH : j;
			i = i == test->w ? 0 : i;
			pixels[i++ + j] = reference[k++];
		}
	}
	else
		ft_die("Fatal error: Texture loading failure.");
	load_alphabet(&doom);
	load_animation(&doom);
	i = 0;
	app_start = SDL_GetTicks();
	while (!doom.quit)
	{
		frame_start = SDL_GetTicks();
		if (keystates[SDL_SCANCODE_RETURN])
		{
			i += dir_horz;
			if (i >= (WIN_HEIGHT * WIN_WIDTH) || i <= 0)
				dir_horz *= -1;
			else
				pixels[i] = 0xffffffff;
		}
		if (keystates[SDL_SCANCODE_SPACE])
		{
			i += dir_vert * WIN_WIDTH;
			if (i >= (WIN_HEIGHT * WIN_WIDTH) || i <= 0)
				dir_vert *= -1;
			else
				pixels[i] = 0xffffffff;
		}
		while (SDL_PollEvent(&doom.event) != 0)
		{
			if (doom.event.type == SDL_QUIT || doom.event.button.button == SDL_SCANCODE_ESCAPE)
				doom.quit = 1;
		}
		SDL_UpdateWindowSurface(doom.win);
		// Delay until next frame
		uint32_t frame_ticks = SDL_GetTicks() - frame_start;
		if (doom.ani_thunder.current < doom.ani_thunder.frames && frame_start - app_start > (unsigned int)doom.ani_thunder.current * 66)
		{
			doom.ani_thunder.current++;
			if (doom.ani_thunder.current == doom.ani_thunder.frames)
			{
				doom.ani_thunder.current = 0;
				app_start = frame_start;
			}
			render_animation(&doom);
		}
		if (frame_ticks < TICKS_PER_FRAME)
		{
			SDL_Delay(TICKS_PER_FRAME - frame_ticks);
		}
	}
	SDL_FreeSurface(doom.buff);
	SDL_FreeSurface(test);
	SDL_DestroyWindow(doom.win);
	Mix_FreeChunk(doom.mcThunder);
	Mix_Quit();
	SDL_Quit();
	return (0);
}
