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

// TODO BIG NOTE TO SELF
//  EDITOR MUST GUARANTEE CLOCKWISE ARRANGEMENT OF WALL NODES IN ROOMS
//  https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order/

// COMPLETED
//		EXPLORE DIFFERENT RESOLUTIONS IN EDITOR WINDOW // DONE
//		LET USER SCROLL UP AND DOWN IN THE EDITOR WITH ARROW KEYS // DONE
//		Stage 1: Offset_x, Offset_y, Live in same memory space as Zoom_factor // DONE
//		Stage 2: These offsets can be between 0-(Max limit depending on Zoom-factor) both X, Y
//		Stage 3: For Zoom Factor 1: Between 0 and 3*EDT_WINDOW_WIDTH / HEIGHT, X / Y
//								 2: Between 0 and 2*EDT_WINDOW_WIDTH / HEIGHT, X / Y
//								 4: Between 0 and 0, X, Y // ... DONE
//		Stage 4: For rendering the correct space displace all walls' X&Y values by the offsets // DONE
//		Stage 5: Test if this works as imagined // DONE
//		Stage 6: Debug the program to detect where it is crashing and why, and fix that // TECH DEBT
//		I think it is crashing because sometimes it infinite loops during Cohen-Sutherland when
//		the clipped line crosses two segments? Possibly? It should be protected, aborted, modified to
//		escape such scenario or resolve it correctly...
//		Solution for now: Add a limit of 10 loops before autoexiting the infinite looping part. // DONE
//		Stage 7: Adjust the sensitivity and acceleration of the keyboard control // DONE
//		Stage 8: Add visual scrollbars to let user know the amount of scrolling applied // DONE
//			Technical debt: Can be done with XPM graphics propably later, implemented with linedraws.

// TODO NEXT FEATURE
//		MODE TO ADD ENEMY AND PLAYER OBJECTS // IN PROGRESS
//			RECORD POSITIONS TO MODEL // <-- YOU ARE HERE !
//		ENABLE RECORDING OF ROOMS AGAIN
//		ENABLE SAVING TO MAPFILE AGAIN
//		MODE TO PORTALIZE A WALL AND EXTEND A NEW ROOM FROM IT

void				init_edt(t_doom *doom)
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
}

void				destroy_edt(t_doom *doom)
{
	SDL_FreeSurface(doom->edt->buff);
	SDL_DestroyWindow(doom->edt->win);
	doom->edt->win = NULL;
	doom->edt->buff = NULL;
	doom->edt->parent = NULL;
	free(doom->edt);
	doom->edt = NULL;
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
    }
    else if (editor_back_buffer()->rendering_on)
    {
        flood_buffer(doom->edt->buff, 0xff000000);
        SDL_BlitSurface(editor_back_buffer()->buff, NULL, doom->edt->buff, NULL);
        editor_back_buffer()->rendering_on = 0;
    }
	SDL_UpdateWindowSurface(doom->edt->win);
}