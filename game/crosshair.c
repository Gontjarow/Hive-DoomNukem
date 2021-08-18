/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crosshair.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 18:38:23 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/18 18:38:31 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static void	render_legacy_ch(t_model *mdl)
{
	t_line	crosshair;

	crosshair.buff = mdl->parent->game->buff;
	crosshair.color = 0x00000000;
	crosshair.x1 = GAME_MIDWIDTH - 10;
	crosshair.x2 = GAME_MIDWIDTH - 2;
	crosshair.y1 = GAME_MIDHEIGHT;
	crosshair.y2 = GAME_MIDHEIGHT;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH + 2;
	crosshair.x2 = GAME_MIDWIDTH + 10;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH;
	crosshair.x2 = GAME_MIDWIDTH;
	crosshair.y1 = GAME_MIDHEIGHT - 10;
	crosshair.y2 = GAME_MIDHEIGHT - 2;
	render_line(&crosshair);
	crosshair.y1 = GAME_MIDHEIGHT + 2;
	crosshair.y2 = GAME_MIDHEIGHT + 10;
	render_line(&crosshair);
}

static void	render_pistol_ch(t_model *mdl)
{
	t_line	crosshair;

	crosshair.buff = mdl->parent->game->buff;
	crosshair.color = 0x00ffffff;
	crosshair.x1 = GAME_MIDWIDTH - 10;
	crosshair.x2 = GAME_MIDWIDTH - 2;
	crosshair.y1 = GAME_MIDHEIGHT;
	crosshair.y2 = GAME_MIDHEIGHT;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH + 2;
	crosshair.x2 = GAME_MIDWIDTH + 10;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH;
	crosshair.x2 = GAME_MIDWIDTH;
	crosshair.y1 = GAME_MIDHEIGHT - 10;
	crosshair.y2 = GAME_MIDHEIGHT - 2;
	render_line(&crosshair);
	crosshair.y1 = GAME_MIDHEIGHT + 2;
	crosshair.y2 = GAME_MIDHEIGHT + 10;
	render_line(&crosshair);
}

static void	render_assault_rifle_ch(t_model *mdl)
{
	t_line	crosshair;

	crosshair.buff = mdl->parent->game->buff;
	crosshair.color = 0x00ffffff;
	crosshair.x1 = GAME_MIDWIDTH - 14;
	crosshair.x2 = GAME_MIDWIDTH - 4;
	crosshair.y1 = GAME_MIDHEIGHT;
	crosshair.y2 = GAME_MIDHEIGHT;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH + 4;
	crosshair.x2 = GAME_MIDWIDTH + 14;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH;
	crosshair.x2 = GAME_MIDWIDTH;
	crosshair.y1 = GAME_MIDHEIGHT + 3;
	crosshair.y2 = GAME_MIDHEIGHT + 7;
	render_line(&crosshair);
}

static void	render_shotgun_ch(t_model *mdl)
{
	t_line	crosshair;

	crosshair.buff = mdl->parent->game->buff;
	crosshair.color = 0x00ffffff;
	crosshair.x1 = GAME_MIDWIDTH - 25;
	crosshair.x2 = GAME_MIDWIDTH - 5;
	crosshair.y1 = GAME_MIDHEIGHT;
	crosshair.y2 = GAME_MIDHEIGHT;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH + 5;
	crosshair.x2 = GAME_MIDWIDTH + 25;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH - 5;
	crosshair.x2 = GAME_MIDWIDTH - 5;
	crosshair.y1 = GAME_MIDHEIGHT;
	crosshair.y2 = GAME_MIDHEIGHT + 10;
	render_line(&crosshair);
	crosshair.x1 = GAME_MIDWIDTH + 5;
	crosshair.x2 = GAME_MIDWIDTH + 5;
	render_line(&crosshair);
}

void	render_crosshair(t_model *mdl)
{
	if (mdl->player.weap_id == PISTOL)
		render_pistol_ch(mdl);
	else if (mdl->player.weap_id == SHOTGUN)
		render_shotgun_ch(mdl);
	else if (mdl->player.weap_id == ASSAULT_RIFLE)
		render_assault_rifle_ch(mdl);
}
