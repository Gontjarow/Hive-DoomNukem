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
		ft_die("Fatal error: SDL_mixer failed to load WAV_THUNDER!");
	if (!(doom->mcSteam = Mix_LoadWAV(WAV_STEAM0)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_STEAM0!");
	if (!(doom->mcPlop = Mix_LoadWAV(WAV_PLOP)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_PLOP!");
	if (!(doom->mcSword = Mix_LoadWAV(WAV_SWORD)))
		ft_die("Fatal error: SDL_mixer failed to load WAV_SWORD!");
	doom->quit = 0;
	doom->le_quit = 1;
	doom->le = NULL;
	doom->alphabet_scale = 1;
	doom->selected_le = 1;
	doom->esc_lock = 0;
}

static void 	init_le(t_doom *doom, int argc, char **argv)
{
	doom->le = (t_le*)malloc(sizeof(t_le));
	if (!doom->le)
		ft_die("Fatal error: Mallocing level editor struct failed at init_le.");
	doom->le->parent = doom;
	doom->le->win = SDL_CreateWindow("DoomNukem Level Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LE_WIN_WIDTH, LE_WIN_HEIGHT, 0);
	doom->le->buff = SDL_GetWindowSurface(doom->le->win);
	doom->le->walls = (t_wall*)malloc(sizeof(t_wall));
	if (!doom->le->walls)
		ft_die("Fatal error: Mallocing walls struct failed at init_le.");
	doom->le->wall_count = 0;
	doom->le->wall_begin = NULL;
	doom->le->map_string = NULL;
	doom->le->join_string = NULL;
	doom->le->map_path = NULL;
	doom->le->portalization_a = NULL;
	doom->le->portalization_b = NULL;
	doom->le->new_portal = NULL;
	doom->le->portals = (t_wall*)malloc(sizeof(t_wall));
	doom->le->portal_count = 0;
	if (!doom->le->walls)
		ft_die("Fatal error: Mallocing portals struct failed at init_le.");
	doom->le->portal_begin = NULL;
	doom->le->is_wall_start = 1;
	doom->le->polygon_start_x = -1;
	doom->le->polygon_start_y = -1;
	doom->le->polygon_binding = 0;
	doom->le->portalization_binding = 0;
	doom->le->portalization_ending = 0;
	doom->le->portal_x = -1;
	doom->le->portal_y = -1;
	doom->le->new_portal_x = -1;
	doom->le->new_portal_y = -1;
	doom->le->write_maps = 0;
	if (argc == 2)
	{
		doom->le->write_maps = 1;
		doom->le->map_path = argv[1];
	}
}

static void 	destroy_le(t_doom *doom)
{
	if (doom->le->write_maps && doom->le->wall_count > 0)
		write_mapfile(doom->le);
	SDL_FreeSurface(doom->le->buff);
	SDL_DestroyWindow(doom->le->win);
	doom->le->win = NULL;
	doom->le->buff = NULL;
	free(doom->le->walls);
	doom->le->walls = NULL;
	if (doom->le->map_string)
		free(doom->le->map_string);
	if (doom->le->join_string)
		free(doom->le->join_string);
	doom->le->map_string = NULL;
	doom->le->join_string = NULL;
	free(doom->le);
	doom->le = NULL;
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

	i = 0;

	init_doom(&doom);
	pixels = doom.buff->pixels;
	i = 0;
	k = 0;
	keystates = SDL_GetKeyboardState(NULL);
	Mix_PlayChannel( -1, doom.mcThunder, 0 );
	test = NULL;
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
		if (keystates[SDL_SCANCODE_UP] && (!doom.selected_le))
		{
			doom.selected_le = 1;
			Mix_PlayChannel( -1, doom.mcSteam, 0 );
		}
		else if (keystates[SDL_SCANCODE_DOWN] && (doom.selected_le))
		{
			doom.selected_le = 0;
			Mix_PlayChannel( -1, doom.mcSteam, 0 );
		}
		else if (keystates[SDL_SCANCODE_RETURN] && doom.selected_le && doom.le_quit)
		{
			SDL_MinimizeWindow(doom.win);
			init_le(&doom, argc, argv);
			SDL_UpdateWindowSurface(doom.le->win);
			Mix_PlayChannel( -1, doom.mcSword, 0 );
			doom.le_quit = 0;
		}
		else if (keystates[SDL_SCANCODE_RETURN] && !doom.selected_le)
		{
			Mix_PlayChannel( -1, doom.mcSword, 0 );
			SDL_Delay(300);
			doom.quit = 1;
		}
		while (SDL_PollEvent(&doom.event) != 0)
		{
			if (doom.event.type == SDL_MOUSEMOTION && !doom.le_quit && doom.event.window.windowID == SDL_GetWindowID(doom.le->win))
				le_mouse_motion(&doom);
			else if (doom.event.type == SDL_MOUSEBUTTONDOWN && !doom.le_quit && doom.event.window.windowID == SDL_GetWindowID(doom.le->win))
				le_mouse_down(&doom);
			else if (doom.event.type == SDL_WINDOWEVENT && doom.event.window.event == SDL_WINDOWEVENT_CLOSE && !doom.le_quit && doom.event.window.windowID == SDL_GetWindowID(doom.le->win))
			{
				doom.le_quit = 1;
				destroy_le(&doom);
			}
			else if (doom.event.type == SDL_QUIT || (doom.event.type == SDL_KEYDOWN && doom.event.button.button == SDL_SCANCODE_ESCAPE && doom.le_quit && !doom.esc_lock))
			{
				Mix_PlayChannel( -1, doom.mcSword, 0 );
				SDL_Delay(300);
				doom.quit = 1;
			}
			else if (doom.event.type == SDL_QUIT || (doom.event.type == SDL_KEYDOWN && doom.event.button.button == SDL_SCANCODE_ESCAPE && !doom.le_quit))
			{
				doom.le_quit = 1;
				destroy_le(&doom);
				doom.esc_lock = 40;
				SDL_RestoreWindow(doom.win);
				Mix_PlayChannel( -1, doom.mcSword, 0 );
				doom.buff = SDL_GetWindowSurface(doom.win);
			}
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
		if (!doom.le_quit)
			le_render(&doom);
		if (doom.esc_lock)
			doom.esc_lock--;
		if (frame_ticks < TICKS_PER_FRAME)
		{
			SDL_Delay(TICKS_PER_FRAME - frame_ticks);
		}
	}
	SDL_FreeSurface(doom.buff);
	SDL_FreeSurface(test);
	SDL_DestroyWindow(doom.win);
	if (!doom.le_quit)
		destroy_le(&doom);
	Mix_FreeChunk(doom.mcThunder);
	Mix_FreeChunk(doom.mcSteam);
	Mix_Quit();
	SDL_Quit();
	return (0);
}
