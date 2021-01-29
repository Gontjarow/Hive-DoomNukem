/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c		                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 15:11:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/06/11 15:12:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

// TODO FOR EDITOR
//		ENABLE MANUAL SINGLE WALL PORTAL CREATION
//      ENABLE TEXTURING ROOMS IN EDITOR
//		CONSIDER SLOPING (DELEGATE TO NGONTJAR)
//		PLAY AROUND WITH FLIGHT PACK ITEM FOR FLYING
//      CONSIDER "EVERY ROOM MUST HAVE A LIGHT SWITCH" MANDATORY RULE

//	TODO FOR EFFECTORS
//		EFFECTORS WILL BE [EFFECT] TOKEN TYPE AND FOUND IN MDL->EFFECT
//		MAP_DECODE AND MAP_ENCODE ADDITIONS FOR [EFFECT] TYPE
//		EFFECT WILL HAVE A ID, TYPE [ENUMERATED INTEGER], LOC(ATION).X, LOC(ATION).Y, TARGET(LOCATION).X, TARGET(LOCATION).Y, TARGET_ID
//		EXAMPLE DATA ROW
//		[EFFECT] ID = 0 | TYPE = 0 | LOC.X = 0 | LOC.Y = 0 | TARGET.X = 0 | TARGET.Y = 0 | TARGET_ID = 0
// 		EFFECTORS WILL BE TRIANGLES IN DIFFERENT COLORS IN THE EDITOR VIEW
//			EFFECTOR TRIANGLE WILL BE POINTING UP FOR ORIGIN AND DOWN FOR DESTINATION OR TARGET
//			EFFECTOR TRIANGLE WILL BE NOTED AND MATCHED WITH ID DIGITS BELOW TRIANGLE AND LIMITED TO 99
//			FIRST EFFECTOR ADDS LEVEL EXITS IN THE EDITOR
//				IF NO LEVEL EXIT DEFINED, REMIND AND WARN ABOUT DEBUGGING MAP
//			SECOND EFFECTOR WILL BE LIGHT SOURCES WITH LIGHT INTENSITY AND RADIUS VALUES
//			THIRD EFFECTOR WILL BE A SWITCH, WITH LOCATION AND TARGET TYPE AND ID (TOGGLING LIGHT OR OPENING BLOCKING DOOR)
//			FOURTH EFFECTOR WILL BE A ONE WAY TELEPORT, WITH LOCATION AND DESTINATION POINTS
//			FOR A BLANK CHAIN_TO_MAPFILE VALUE THE MAP CAMPAIGN TRIGGERS CAMPAIGN VICTORY FROM THE LEVEL EXIT

//		TODO FOR PORTALIZATION
//	 			PORTAL VIRTUAL, PORTAL WINDOW AND PORTAL DOOR TYPES VIA SELECTION MODE
//				STRICTLY ALLOW ONLY FIRST WALLS TO ACT AS PORTAL HINGES FOR SLOPING FLOOR AND CEILING
//					STRICTLY ALLOW ONLY PERPENDICULAR WALLS TO FIRST WALL ACT AS ADDITIONAL PORTAL HINGES WHEN A PORTAL HINGE EXISTS ALREADY
//				PRIORITIZE SELECTION OF PORTAL TYPES BEFORE WALL TYPES TO ALLOW DELETION OF PORTALS
//			DELETION OF A PORTAL OR AN EFFECTOR WITH A LINKED SWITCH TO IT WILL SCAN AND DELETE THE LINKED SWITCH ALSO
//				CAREFUL CODING OF THIS FEATURE MUST BE CONSIDERED
//			TEXTURE RESOURCES
//				EACH WALL MUST BE MATCHED TO A TEXTURE ID
//				EDITOR ALLOWS FOR EXTERNAL TEXTURES TO BE LOADED AND ASSIGNED IDS TO
//				WHOLE ROOMS WALL TEXTURES CAN BE ASSIGNED IN SELECTION MODE
//				SINGLE WALLS TEXTURES CAN BE ASSIGNED IN SELECTION MODE
//				EACH MAP FILE WILL BE A BUNDLE AND BLOB OF
//					MAP DATA, N LINES (TOML-LIKE MAP FORMAT)
//					TEXTURES, N AMOUNT FOR N LINES (XPM)
//					UI GRAPHICS AND AUDIO FIXED AMOUNT FOR N LINES (XPM, WAV)
//				BINARY REQUIRES A MAPFILE TO LOAD AND PLAY A MAP
//				WHEN MAPFILES AND EDITOR RESOURCES FOUND, ALLOW FULL MENU FOR EITHER LEVEL SELECT OR EDITING
//				WHEN MAPFILES FOUND BUT NO EDITOR RESOURCES FOUND, ALLOW LEVEL SELECT AND GAME LOADING
//				WHEN NO MAPFILES FOUND, BUT EDITOR RESOURCES FOUND, ALLOW BINARY BOOTING TO EDITOR
//				WHEN NO MAPFILES OR EDITOR RESOURCES FOUND, REFUSE BINARY BOOTING

// TODO FOR FIXING TECH DEBT, OPTIMIZING AND REMOVING FORBIDDEN FUNCTIONS
//		THE SOLUTION FOR INFINITE LOOPING CLIP DETECTION: LIMITED TO 10 ITERATIONS
//		SCROLL BARS AS LINES TO INDICATE LOCATION SCROLLED TO: DID NOT USE RECT DRAWING
//  	SDL_BLIT ON UNOPTIMIZED SDL_SURFACES

// PORTALIZATION IDEA NOTES BELOW

// Idea for portalization:
// Create 2 rooms with approximate link wall between them close to each other.
// Switch to Room Placing and click one of the rooms. Click one wall to designate as portaliziable
// Move this room and watch the portalizable wall magnetize a nearby wall points to comply with the
// moved rooms wallpoints, linking them.

// Idea for portalization, alternative additional:
// During wall drawing, designate portal wall by bypassing Red Impermissible drawing. Finish portal
// by confirming 2nd portalized point, from where the wall drawing for the ongoing room continues.
// Middle click should cancel and reset the whole ongoing room, right click should stop drawing but
// preserve so far drewn walls and portals.

// Idea for finishing up unclosed rooms:
// Wall drawing snaps to unclosed room endpoint/startpoints, allowing expansion, tying up.

// Map saving pre-check:
// Enemy, player collision checks, inside room checks, room closedness checks. Error console (later GUI)
// and prevent closing of editor when in errors.

// Map saving advanced pre-check:
// Check for wall intersections, and disallow. As above, error console (later GUI) and prevent
// closing and saving, until resolved.

void				init_edt(t_doom *doom, int argc, char **argv)
{
	doom->edt = (t_editor*)malloc(sizeof(t_editor));
	if (!doom->edt)
		ft_die("Fatal error: Mallocing level editor struct failed at init_edt.");
	doom->edt->parent = doom;
	doom->edt->win = SDL_CreateWindow("DoomNukem Level Editor", SDL_WINDOWPOS_CENTERED,
									  SDL_WINDOWPOS_CENTERED, EDT_WIN_WIDTH, EDT_WIN_HEIGHT, 0);
	if (!doom->edt->win)
		ft_die("Fatal error: Could not create DoomNukem Level Editor window.");
	doom->edt->buff = SDL_GetWindowSurface(doom->edt->win);
	if (!doom->edt->buff)
		ft_die("Fatal error: Could not retrieve buffer of Level Editor window.");
	flood_buffer(doom->edt->buff, 0xff000000);
	//doom->edt_state = get_state();
	get_state()->gui->activate(get_state());
	doom->edt->map = NULL;
	doom->edt->map_supplied = (argc == 2) ? 1 : 0;
	doom->edt->map_path = (argc == 2) ? argv[1] : NULL;
	if (argc == 2)
		doom->edt->map = init_mapfile();
}

static void 	draw_confirmation(char *input, t_doom *doom)
{
    get_state()->gui->change_zoom(get_state());
    flood_buffer(doom->edt->buff, 0xff000000);
    SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
    editor_back_buffer()->rendering_on = 0;
    edt_gridify();
    print_glyph_str("saved map as ", doom->edt->buff, 480, 860);
    print_glyph_str(input, doom->edt->buff, 922, 860);
    print_glyph_str("saved map as ", editor_back_buffer()->buff, 480, 860);
    print_glyph_str(input,editor_back_buffer()->buff, 922, 860);
}

static void 	draw_input(t_doom *doom)
{
    get_state()->gui->change_zoom(get_state());
    flood_buffer(doom->edt->buff, 0xff000000);
    SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
    editor_back_buffer()->rendering_on = 0;
    edt_gridify();
    print_glyph_str(STRING_ENTER_MAPFILE, doom->edt->buff, 80, 50);
    print_glyph_str(STRING_VALID_CHAR_INFO, doom->edt->buff, 90, 90);
}

static void 	read_keystate_input(char *arr, int *i, t_doom *doom)
{
    SDL_Scancode	alphabet[27] = { SDL_SCANCODE_A, SDL_SCANCODE_B,
                                     SDL_SCANCODE_C, SDL_SCANCODE_D,	SDL_SCANCODE_E, SDL_SCANCODE_F,
                                     SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_I, SDL_SCANCODE_J,
                                     SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_M, SDL_SCANCODE_N,
                                     SDL_SCANCODE_O, SDL_SCANCODE_P, SDL_SCANCODE_Q, SDL_SCANCODE_R,
                                     SDL_SCANCODE_S, SDL_SCANCODE_T, SDL_SCANCODE_U, SDL_SCANCODE_V,
                                     SDL_SCANCODE_W, SDL_SCANCODE_X, SDL_SCANCODE_Y, SDL_SCANCODE_Z,
                                     SDL_SCANCODE_PERIOD };
    char 			chars[27] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                                  'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                  'w', 'x', 'y', 'z', '.'};
    int 			x;

    if (doom->keystates[SDL_SCANCODE_BACKSPACE] && *i > 0)
    {
        //ft_putchar('\n');
        arr[(*i) - 1] = '\0';
        (*i)--;
        //ft_putstr(arr);
        draw_input(doom);
        print_glyph_str(arr, doom->edt->buff, 100, 130);
        return ;
    }
    x = 0;
    while (x < 27)
    {
        if (*i == 255)
            break ;
        if (doom->keystates[alphabet[x]])
        {
            arr[*i] = chars[x];
            (*i)++;
            draw_input(doom);
            print_glyph_str(arr, doom->edt->buff, 100, 130);
        }
        x++;
    }
}

static char         *ask_to_save(t_doom *doom)
{
        uint32_t	input_ticks;
        int 		finished;
        int 		i;
        char 		input[255];

        //puts("Listening for input:\n");
        draw_input(doom);
        finished = 0;
        while (finished < 255)
            input[finished++] = '\0';
        finished = 0;
        i = 0;
        input_ticks = SDL_GetTicks();
        while (!finished)
        {
            if (SDL_GetTicks() - input_ticks < 64)
                continue ;
            SDL_PumpEvents();
            doom->keystates = SDL_GetKeyboardState(NULL);
            if (doom->keystates[SDL_SCANCODE_RETURN] || doom->keystates[SDL_SCANCODE_ESCAPE] || i == 255)
                finished = 1;
            read_keystate_input(&input, &i, doom);
            input_ticks = SDL_GetTicks();
            SDL_UpdateWindowSurface(doom_ptr()->edt->win);
        }
        draw_confirmation(&input, doom);
        return (ft_strdup(&input));
}

void			    destroy_edt(t_doom *doom)
{
	if (doom->edt->map_supplied)
	{
		create_strings_from_model(doom->mdl, doom->edt->map);
		if (doom->mdl->player.x == -1 || doom->mdl->player.y == -1)
			ft_putendl("Warning: Player position not supplied, did not save mapfile.");
		else if (doom->map->was_filled)
		{
			if (edt_handle_confirm_save(doom) == 1)
				if (!(overwrite_mapfile(doom->edt->map_path, doom->edt->map)))
					ft_putendl("Warning: Could not save mapfile, when attempted overwriting, write_mapfile failed.");
		}
		else if (!write_mapfile(doom->edt->map_path, doom->edt->map))
			ft_putendl("Warning: Could not save mapfile, write_mapfile failed.");
		if (doom->edt->map != NULL)
			destroy_mapfile(doom->edt->map);
        doom->edt->map = NULL;
	}
	else if (doom->mdl->player.x == -1 || doom->mdl->player.y == -1)
    {
        ft_putendl("Warning: Player position not supplied, did not save mapfile.");
    }
	else
    {
        doom->edt->map = init_mapfile();
        create_strings_from_model(doom->mdl, doom->edt->map);
	    if (doom->edt->map_path)
	        free(doom->edt->map_path);
	    doom->edt->map_path = ask_to_save(doom);
        if (!write_mapfile(doom->edt->map_path, doom->edt->map))
            ft_putendl("Warning: Could not save mapfile, write_mapfile failed.");
        if (doom->edt->map != NULL)
            destroy_mapfile(doom->edt->map);
        doom->edt->map = NULL;
        SDL_Delay(500);
        doom->menu->update_argc_argv = 1;
        doom->menu->added_arg = ft_strdup(doom->edt->map_path);
        free(doom->edt->map_path);
    }
	// TODO TECHNICAL DEBT, SEPARATE OUT OF THIS FUNCTION, PERHAPS INTO MENU ITEM WHICH KILLS EDITOR
	if (doom->map_data_initialized && doom->map->was_filled)
	{
		destroy_mapfile(doom->map);
		doom->map_data_initialized = 0;
		doom->map->was_filled = 0;
			//ft_putendl("Destroyed mapfile by calling destroy_mapfile and setting map_data_initialized to 0");
	}
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

void			edt_gridify(void)
{
	int 		y;
	int			loc;
	int 		end;
	int 		grid_sz;
	int 		sx_mod;
	int 		sy_mod;
	uint32_t	col;
	uint32_t	*pixels;
	uint32_t 	*ebbp;

	if (!get_state()->grid_on)
		return ;
	loc = 0;
	col = COLOR_GRID_LINE;
	end = (EDT_WIN_HEIGHT * EDT_WIN_WIDTH) - 1;
	pixels = doom_ptr()->edt->buff->pixels;
	ebbp = editor_back_buffer()->buff->pixels;
	grid_sz = get_state()->grid_size / get_state()->zoom_factor;
	sx_mod = grid_sz - get_state()->scroll_x % grid_sz;
	if (sx_mod == grid_sz)
		sx_mod = 0;
	sy_mod = grid_sz - get_state()->scroll_y % grid_sz;
	if (sy_mod == grid_sz)
		sy_mod = 0;
	while (loc < end)
	{
		if ((loc % grid_sz == sx_mod) && (y % grid_sz == sy_mod) && EDT_WIN_HEIGHT - y > 10)
			if (pixels[loc] == 0xff000000)
			{
				pixels[loc] = col;
				if (ebbp[loc] == 0xff000000)
					ebbp[loc] = col;
			}
		if (loc % EDT_WIN_WIDTH == EDT_WIN_WIDTH - 10)
		{
			y++;
			loc += 10;
		}
		loc++;
	}
}

void				edt_render(t_doom *doom)
{
	if (get_state()->cooldown > 1)
		get_state()->cooldown--;
	else if (get_state()->cooldown == 1)
	{
		get_state()->cooldown = 0;
		get_state()->gui->change_zoom(get_state());
	}
	edt_keystate_input(doom);
    if (editor_front_buffer()->rendering_on)
    {
        flood_buffer(mixing_surface(), 0xff000000);
        SDL_BlitSurface(editor_back_buffer()->buff, NULL, mixing_surface(), NULL);
        SDL_BlitSurface(editor_front_buffer()->buff, NULL, mixing_surface(), NULL);
        flood_buffer(doom->edt->buff, 0xff000000);
        SDL_BlitSurface(mixing_surface(), NULL, doom->edt->buff, NULL);
        editor_front_buffer()->rendering_on = 0;
        editor_back_buffer()->rendering_on = 0;
        edt_gridify();
    }
    else if (editor_back_buffer()->rendering_on)
    {
        flood_buffer(doom->edt->buff, 0xff000000);
        SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
        editor_back_buffer()->rendering_on = 0;
		edt_gridify();
    }
	SDL_UpdateWindowSurface(doom->edt->win);
}