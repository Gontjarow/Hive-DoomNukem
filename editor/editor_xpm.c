/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_xpm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 17:18:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 17:32:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

SDL_Surface				*zoom_xpm(int zoom_factor)
{
	static SDL_Surface	*singly = NULL;
	static SDL_Surface	*doubly = NULL;
	static SDL_Surface	*triply = NULL;
	static SDL_Surface	*quadly = NULL;

	if (zoom_factor == 1)
		return (singly == NULL ? singly = xpm2surface(XPM_SINGLY) : singly);
	if (zoom_factor == 2)
		return (doubly == NULL ? doubly = xpm2surface(XPM_DOUBLY) : doubly);
	if (zoom_factor == 3)
		return (triply == NULL ? triply = xpm2surface(XPM_TRIPLY) : triply);
	if (zoom_factor == 4)
		return (quadly == NULL ? quadly = xpm2surface(XPM_QUADLY) : quadly);
	ft_die("Fatal error: Could not return zoom_xpm.");
	return (NULL);
}

SDL_Surface				*mode_xpm(t_gui *mode)
{
	static SDL_Surface	*poly = NULL;
	static SDL_Surface	*select = NULL;
	static SDL_Surface	*plant = NULL;
	static SDL_Surface	*pickups = NULL;
	static SDL_Surface	*effect = NULL;

	if (mode == mode_polydraw())
		return (poly == NULL ? poly = xpm2surface(XPM_POLYDRAW) : poly);
	else if (mode == mode_select())
		return (select == NULL ? select = xpm2surface(XPM_SELECT) : select);
	else if (mode == mode_effect())
		return (effect == NULL ? effect = xpm2surface(XPM_EFFECT) : effect);
	else if (mode == mode_planting())
		return (plant == NULL ? plant = xpm2surface(XPM_PLANTING) : plant);
	else if (mode == mode_pickups())
		return (pickups == NULL ? pickups = xpm2surface(XPM_PICKUPS) : pickups);
	ft_die("Fatal error: Could not return mode_xpm.");
	return (NULL);
}

SDL_Surface				*mousehelp_xpm(void)
{
	static SDL_Surface	*mousehelp_xpms = NULL;

	if (mousehelp_xpms == NULL)
		mousehelp_xpms = xpm2surface("img/edt/mousehelp.xpm");
	if (!mousehelp_xpms)
		ft_die("Fatal error: Could not return mousehelp_xpm.");
	return (mousehelp_xpms);
}

SDL_Surface				*keyhelp_xpm(void)
{
	static SDL_Surface	*keyhelp_xpms = NULL;

	if (keyhelp_xpms == NULL)
		keyhelp_xpms = xpm2surface("img/edt/keyhelp.xpm");
	if (!keyhelp_xpms)
		ft_die("Fatal error: Could not return keyhelp_xpm.");
	return (keyhelp_xpms);
}
