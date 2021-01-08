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

//		COMPLETED FOR EDITOR IN GENERAL
//			GRID SYSTEM ON TOP OF EVERYTHING ELSE // DONE
//				MODULO OFFSETTED BY SCROLL_X, SCROLL_Y, STEPPING DIVIDED BY ZOOMFACTOR // DONE
//				DRAW VERY LAST ON SCREEN AREA // DONE
//				ONLY ON BLACK PIXELS STRAIGHT TO BUFFER // DONE
//				TOGGLE WITH G // DONE
//				ADD TO AVOID[X] COLORS THE COLOR_GRID_LINE VALUE // DONE
//				DECOUPLED FROM EVERYTHING EXCEPT EDITOR BACK BUFFER // DONE
//				VARY STEPPING WITH KEYS T AND B // DONE
//				EXCLUDE INFO BOXES AND SCROLL BAR AREAS FROM DRAWING // DONE
//			PREVIEW TRAILING TAIL FOR ROTATABLES PLANTING // DONE
//			ENEMIES DOUBLE CLICK DELETE // DONE
//				LEFT CLICK THEN LEFT CLICK AGAIN TO DELETE ENEMY // DONE
//			PICKUPS DOUBLE CLICK DELETE // DONE
//				LEFT CLICK THEN LEFT CLICK AGAIN TO DELETE ENEMY // DONE

//	TODO BIG NOTE TO SELF - CHECK IF TRUE!
//  	EDITOR COULD GUARANTEE CLOCKWISE ARRANGEMENT OF WALL NODES IN ROOMS

//	TODO FOR UPDATE OPTIONS
//		SELECTION MODE FOR COMPLETE ROOMS // DONE
//			SELECTING ANOTHER OR DESELECTION // DONE
//			RENDER BARYMETRIC CENTER FOR SELECTED ROOM // DONE
//			RENDER DIGITS VISIBLE FOR SELECTED ROOMS ROOF
//				ON TOP OF THE BARYMETRIC VISUAL SQUARE
//			RENDER DIGITS VISIBLE FOR SELECTED ROOMS FLOOR
//				AT THE BOTTOM OF THE BARYMETRIC VISUAL SQUARE
//		FIX ZOOMED, SCROLLED FUNCTIONALITY FOR DOUBLE CLICK DELETIONS
//		INPUT SYSTEM FOR HEIGHT OF BOTH SELECTED ROOM ROOF AND FLOOR
//			E,R,D,F KEYBOARD STEPPING
//			EDITOR SPAWNS NEW ROOMS WITH THE LAST VALUES USED OR MODIFIED
//		DELETION OF THE SELECTED ROOM
//		ADJUST ROTATION FOR ROTATABLES // POSTPONE
//			SINGLE CLICK THEN SINGLE CLICK AGAIN TO ASSIGN TAIL // POSTPONE
//			PLANTING -> LEFT CLICK (CLOSE TO A PLANTING) -> LEFT CLICK // POSTPONE

// TODO FOR NEW FEATURES
//		MODE TO PORTALIZE A WALL AND EXTEND A NEW ROOM FROM IT
//		ADD LEVEL EXITS IN THE EDITOR WITH EITHER A NAME OF MAP TO LOAD OR BLANK TO SKIP TO VICTORY SCREEN
// 		EFFECTORS WILL BE TRIANGLES IN DIFFERENT COLORS IN THE EDITOR VIEW

// TODO TECH DEBT, OPTIMIZATION AND FORBIDDEN DEBT
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

// COMPLETED UPGRADE TO JSON LIKE PARSING FOR MAP DATA IN AND OUT, BEFORE ADDING
//		PICKUPS, EFFECTORS DATA TYPES, EXPANDING THE OLD DATA WITH MORE FIELDS ETC // DONE

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
	doom->edt_state = get_state();
	doom->edt->map = NULL;
	doom->edt->map_supplied = (argc == 2) ? 1 : 0;
	doom->edt->map_path = (argc == 2) ? argv[1] : NULL;
	if (argc == 2)
		doom->edt->map = init_mapfile();
}

void				destroy_edt(t_doom *doom)
{
	if (doom->edt->map_supplied)
	{
		create_strings_from_model(doom->mdl, doom->edt->map);
		if (doom->mdl->player.x == -1 || doom->mdl->player.y == -1)
			ft_putendl("Warning: Player position not supplied, did not save mapfile.");
		else if (doom->map->was_filled && !overwrite_mapfile(doom->edt->map_path, doom->edt->map))
			ft_putendl("Warning: Could not save mapfile, when attempted overwriting, write_mapfile failed.");
		else if (!write_mapfile(doom->edt->map_path, doom->edt->map))
			ft_putendl("Warning: Could not save mapfile, write_mapfile failed.");
		if (doom->edt->map != NULL)
			destroy_mapfile(doom->edt->map);
	}
	// TODO TECHNICAL DEBT, SEPARATE OUT OF THIS FUNCTION, PERHAPS INTO MENU ITEM WHICH KILLS EDITOR
	if (doom->map_data_initialized && doom->map->was_filled)
	{
		destroy_mapfile(doom->map);
		doom->map_data_initialized = 0;
			//ft_putendl("Destroyed mapfile!!!");
	}
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
}

static void 		edt_gridify(void)
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
		/* whole line gridification style
		 if (y % grid_sz == sy_mod)
			if (pixels[loc] == 0xff000000)
				pixels[loc] = col; */
		loc++;
	}
}

void				edt_render(t_doom *doom)
{
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