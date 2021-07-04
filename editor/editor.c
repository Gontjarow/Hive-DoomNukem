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

// BLUE EFFECTOR IS THE KEYPANEL TERMINAL WHICH TAKES ACCES KEYS AND UNLOCKS A LINKED DOOR_PORTAL
//		THE KEYPANEL TERMINAL SITS ON A WALL, BUT NEEDS TO BE LINKED WITH A SECOND EFFECTOR, WHICH HAS UPSIDEDOWN TRIANGLE
//		THE SECOND EFFECTOR SITS ON A DOOR_PORTAL TO MARK THE TARGET

// VOID PIXELS BEYOND CLOSED DOORS IN POLYMAP, WHICH ON OPEN FILL WITH THE CORRECT ROOM_ID COLOR

// KEYPANEL SCHEME: FIRST, UPWARD EFFECTOR IS THE PANEL, IF GIVEN THE SECOND EFFECTOR, SET TARGET_ID -1
// SECOND DOWNWARD EFFECTOR IS THE TARGET DOOR, IF NEVER GIVEN, DESTROY THE FIRST, UPWARD EFFECTOR
// SECOND EFFECTOR TARGET ID IS THE PORTAL ID OF THE DOOR_PORTAL
// ADD DISALLOWING PLACING MORE KEYPANEL EFFECTORS ON THE SAME WALL
// ADD DISALLOWING PLACING MORE KEYPANEL EFFECTORS ON THE SAME DOOR_PORTAL
// FINISH SHOULD PUTS COMMENTED LOCATIONS IN CODE TO HANDLE THE FOLLOWING_UP_ON LOGIC IN EFFECT_LOGIC

// LIGHTSWITCH KNOB NEEDS TO GET SPAWNED ONE PER ROOM AUTOMATICALLY IF NOT DEFINED IN EDITOR, FIRST NON-PORTAL WALL IN EACH ROOM
// 		AUTOCREATE A CENTRAL ROOF LAMP FOR ROOMS IN THE BARYMETRIC CENTERPOINT

// LAST NEW EFFECTOR WILL BE AN ELEVATOR SWITCH THAT RAISES THE HOSTING ROOM FLOOR AND CEILING

// FLESH OUT CAN_PORTAL IN EDITOR->
//		IF PLACED EFFECTORS ON WALLS, CAN_PORTAL = 0 ? OR JUST DESTROY EFFECTORS ?
//		MAINLY CAN_PORTAL = 0 FOR ADJACENT WALLS ? OR TOO RESTRICTIVE ?
//		IF THESE IDEAS ARE NOT USEFUL, REMOVE CAN_PORTAL FROM CODEBASE AND RELY ON POLYDRAW OVERDRAW CHECKS FOR VALIDITY
// VALIDATE NON OVERLAPPING ROOMS BY CHECKING POLYMAP FOR OVERDRAW (IGNORING EDGE LINES)
// 		FOOTPRINT POLYMAP CHECK COLOR IS 0 BUT IGNORE EDGE LINES (WHITE PIXELS)

// TODO MISC FIXES
//		GIVE CAPTURED MOUSE CURSOR BACK ALWAYS WHEN GAME IS STOPPED
//		GIVE FEEDBACK ON HITS ALWAYS WITH HITSCAN CURSOR OR OTHER EASY WAY
//		COMBO COUNTER 2D
//			- COMBO HITS AWARD COMBO
//			- COMBO IS BROKEN BY MISSING SHOTS
//			- COMBO IS AWARDED IN MINIMUM FORM BY MORE HITS
//			- COMBO IS AWARDED IN MAXIMUM FORM BY MORE HITS ON DIFFERENT ENEMY THAN LAST ENEMY
//			- COMBO IS AWARDED IN BONUS FORM BY FASTER COMBO AND MULTIPLE HITS WITH SAME SHOTGUN ROUND
//			- MAXIMAL COMBO REQUIRES ALL WEAPONS, RELOADED AND LOADED, THEN SWAPPING THEM, AND PLAYING IN A ROOM WITH LOTS OF ENEMIES
//			- A RELOAD BREAKS A COMBO!
//			- JAPANESE CASHING MASHINE SOUND EFFECTS, CRAZY 2D/3D UI AND ADDICTIVE GAMEPLAY ELEMENT WITH HIGH SCORING!
//		ALLOW DISCARD MAP INSTEAD OF ASK FOR SAVENAME WITH ESC
//		CREATE OR REUSE A VICTORY/DEATH SCREEN FOR WHEN THE GAME IS LOST OR WON
//		CHECK THAT FOR A BLANK CHAIN_TO_MAPFILE VALUE THE MAP CAMPAIGN TRIGGERS CAMPAIGN VICTORY FROM THE LEVEL EXIT
//		SWAP FROM OBSOLETE KEYBOARD INPUT IN CHAIN MAP TO MODERN ONE AS PER ASKING SAVE MAP DIALOG
//		FIX THE MOST COMMON EDITOR BUGS
//		DISABLE KNEEHEIGHT METHOD FOR WHEN IN FLIGHT AFTER FLYING OR JUMPING FOR CROSSING ROOMS
//		BLOCKING CROSSING OF ROOM BORDERS WHEN FOREHEAD HITTING TO THE TOP CEILING OF CROSSING
//		SCROLL BARS AS RECTS INSTEAD OF LINES TO INDICATE LOCATION SCROLLED TO
//		BIND UP AND DOWN TO FORWARD AND BACKWARD
//		STRICTLY ALLOW ONLY FIRST WALLS TO ACT AS PORTAL HINGES FOR SLOPING FLOOR AND CEILING
//		STRICTLY ALLOW ONLY PERPENDICULAR WALLS TO FIRST WALL ACT AS ADDITIONAL PORTAL HINGES WHEN A PORTAL HINGE EXISTS ALREADY
//		IN EDITOR: DELETION OF A PORTAL OR AN EFFECTOR WITH A LINKED SWITCH TO IT WILL SCAN AND DELETE THE LINKED SWITCH ALSO
//			CAREFUL CODING OF THIS FEATURE MUST BE CONSIDERED
//		TEXTURE RESOURCES EACH WALL MUST BE MATCHED TO A TEXTURE ID
//		EDITOR ALLOWS FOR PREVIEW OF EXTERNAL TEXTURES TO BE SHOWN
//		WHOLE ROOMS WALL TEXTURES CAN BE ASSIGNED IN SELECTION MODE
//		SINGLE WALLS TEXTURES CAN BE ASSIGNED IN SELECTION MODE
//		EACH MAP FILE WILL BE A BUNDLE AND BLOB OF
//			MAP DATA, N LINES (TOML-LIKE MAP FORMAT)
//			TEXTURES, N AMOUNT FOR N LINES (XPM)
//		BINARY REQUIRES A MAPFILE TO LOAD AND PLAY A MAP
//			WHEN MAPFILES AND EDITOR RESOURCES FOUND, ALLOW FULL MENU FOR EITHER LEVEL SELECT OR EDITING
//			WHEN MAPFILES FOUND BUT NO EDITOR RESOURCES FOUND, ALLOW LEVEL SELECT AND GAME LOADING
//			WHEN NO MAPFILES FOUND, BUT EDITOR RESOURCES FOUND, ALLOW BINARY BOOTING TO EDITOR
//			WHEN NO MAPFILES OR EDITOR RESOURCES FOUND, REFUSE BINARY BOOTING

// TODO FINISHING STEPS - PHASE IV
//		ENABLE JOINED WALL PORTAL CREATION
//		CREATE A DIGITAL CLOCK STYLE FONT WITH THE DIGITAL CLOCK PROCEDURAL GENERATION IDEA
//		EDITOR SNAP TO GRID
//		PLAY SOUNDS AT TYPEWRITER
//		PIVOT FROM SPRINTF FOR MAP_ENCODE FUNCTION FAMILY
//		BUNDLING OF THE RESOURCES INTO A SINGLE FILE
//		BUNDLE ALL UI GRAPHICS AND AUDIO TO BINARY
//  	NEEDS FIX TO SWITCH AWAY FROM SDL_BLIT ON UNOPTIMIZED SDL_SURFACES TO CUSTOM FUNCTION
//		ADD MISSING INFOBOXES, HELP AND GUI HINTS AND DOCUMENTATION FOR MAP EDITOR VIEW
//		NORMINETTING THE PROJECT CODE AND RETURNING TO SCHOOL REPOSITORY

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