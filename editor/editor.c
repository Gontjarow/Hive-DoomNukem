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

//	RECENTLY COMPLETED
//		CHECK THAT CHAINING MAPS WORKS

//	TODO FOR EDITOR
//		CHECK THAT USING LEVEL EXITS WORKS
//      ENABLE TEXTURING ROOMS IN EDITOR
//		ENABLE JOINED WALL PORTAL CREATION
//		CREATE A DIGITAL CLOCK STYLE FONT WITH THE DIGITAL CLOCK PROCEDURAL GENERATION IDEA
//		DEFERRED PIVOT FROM SPRINTF FOR MAP_ENCODE FUNCTION FAMILY
//			DEFERRED AUTOCREATE A CENTRAL ROOF LAMP FOR ROOMS IN THE BARYMETRIC CENTERPOINT
// 			DEFERRED AUTOCREATE A LIGHTSWITCH FOR CENTRAL ROOF LAMPS IN THE FIRST WALL OF ROOMS
//			DEFERRED PLAY AROUND WITH A FLIGHT PACK ITEM FOR FLOATING
//			DEFERRED SHOOTABLE CENTRAL ROOF LAMP TO PERMANENTLY DARKEN A ROOM
//			DEFERRED BUNDLING OF THE RESOURCES INTO A SINGLE FILE
//			DEFERRED BUNDLE ALL UI GRAPHICS AND AUDIO TO BINARY
//			DEFERRED NORMING THE PROJECT CODE

//	TODO FOR EFFECTORS
//			EFFECTOR TRIANGLE WILL BE NOTED AND MATCHED WITH ID DIGITS BELOW TRIANGLE AND LIMITED TO 99
//			SECOND EFFECTOR WILL BE A KEY THAT CAN BE PICKED UP TO ENABLE OPENING OF DOORS
//			THIRD EFFECTOR WILL BE LIGHT SOURCES WITH LIGHT INTENSITY AND RADIUS VALUES
//			FOURTH EFFECTOR WILL BE A SWITCH, WITH LOCATION AND TARGET TYPE AND ID (TOGGLING LIGHT OR OPENING BLOCKING DOOR)
//			FIFTH EFFECTOR WILL BE A ONE WAY TELEPORT, WITH LOCATION AND DESTINATION POINTS
//			FOR A BLANK CHAIN_TO_MAPFILE VALUE THE MAP CAMPAIGN TRIGGERS CAMPAIGN VICTORY FROM THE LEVEL EXIT

//		TODO FOR PORTALIZATION
//	 			PORTAL VIRTUAL, PORTAL WINDOW AND PORTAL DOOR TYPES VIA SELECTION MODE
//				STRICTLY ALLOW ONLY FIRST WALLS TO ACT AS PORTAL HINGES FOR SLOPING FLOOR AND CEILING
//					STRICTLY ALLOW ONLY PERPENDICULAR WALLS TO FIRST WALL ACT AS ADDITIONAL PORTAL HINGES WHEN A PORTAL HINGE EXISTS ALREADY
//				PRIORITIZE SELECTION OF PORTAL TYPES BEFORE WALL TYPES TO ALLOW DELETION OF PORTALS
//			IN EDITOR: DELETION OF A PORTAL OR AN EFFECTOR WITH A LINKED SWITCH TO IT WILL SCAN AND DELETE THE LINKED SWITCH ALSO
//				CAREFUL CODING OF THIS FEATURE MUST BE CONSIDERED
//			TEXTURE RESOURCES
//				EACH WALL MUST BE MATCHED TO A TEXTURE ID
//				EDITOR ALLOWS FOR EXTERNAL TEXTURES TO BE LOADED AND ASSIGNED IDS TO
//				WHOLE ROOMS WALL TEXTURES CAN BE ASSIGNED IN SELECTION MODE
//				SINGLE WALLS TEXTURES CAN BE ASSIGNED IN SELECTION MODE
//				EACH MAP FILE WILL BE A BUNDLE AND BLOB OF
//					MAP DATA, N LINES (TOML-LIKE MAP FORMAT)
//					TEXTURES, N AMOUNT FOR N LINES (XPM)
//				BINARY REQUIRES A MAPFILE TO LOAD AND PLAY A MAP
//				WHEN MAPFILES AND EDITOR RESOURCES FOUND, ALLOW FULL MENU FOR EITHER LEVEL SELECT OR EDITING
//				WHEN MAPFILES FOUND BUT NO EDITOR RESOURCES FOUND, ALLOW LEVEL SELECT AND GAME LOADING
//				WHEN NO MAPFILES FOUND, BUT EDITOR RESOURCES FOUND, ALLOW BINARY BOOTING TO EDITOR
//				WHEN NO MAPFILES OR EDITOR RESOURCES FOUND, REFUSE BINARY BOOTING

// TODO FOR FIXING TECH DEBT, OPTIMIZING AND REMOVING FORBIDDEN FUNCTIONS
//		DEFERRED FIXING THE SOLUTION FOR INFINITE LOOPING CLIP DETECTION: LIMITED TO 10 ITERATIONS
//		DEFERRED SCROLL BARS AS LINES TO INDICATE LOCATION SCROLLED TO: DID NOT USE RECT DRAWING
//  	NEEDS FIX TO SWITCH AWAY FROM SDL_BLIT ON UNOPTIMIZED SDL_SURFACES TO CUSTOM FUNCTION

//	DONE FOR EFFECTORS
// 		EFFECTORS WILL BE TRIANGLES IN THE EDITOR VIEW // DONE
//			EFFECTOR TRIANGLE WILL BE POINTING UP FOR ORIGIN AND DOWN FOR DESTINATION OR TARGET // DONE
//			TRIANGLES WILL BE IN DIFFERENT COLORS BASED ON EFFECTOR TYPE // DONE
//			FIRST EFFECTOR ADDS LEVEL EXITS IN THE EDITOR // DONE
//				IF NO LEVEL EXIT DEFINED, REMIND AND WARN ABOUT DEBUGGING MAP // DONE

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
		{
			destroy_mapfile(doom->edt->map);
			doom->edt->map = NULL;
		}
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
		{
			destroy_mapfile(doom->edt->map);
			doom->map = NULL;
		}
        doom->edt->map = NULL;
        SDL_Delay(500);
        doom->menu->update_argc_argv = 1;
        doom->menu->added_arg = ft_strdup(doom->edt->map_path);
        free(doom->edt->map_path);
    }
	//TODO SEPARATE INTO A FUNCTION?
	if (doom->map != NULL && doom->map->was_filled)
	{
		destroy_mapfile(doom->map);
		doom->map = NULL;
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