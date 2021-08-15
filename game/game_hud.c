/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 20:00:45 by msuarez-          #+#    #+#             */
/*   Updated: 2021/08/15 15:31:09 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void	init_hud(t_doom *doom)
{
	int	n;

	n = 0;
	while (n <= 9)
	{
		doom->mdl->player.weap_arr[n].weap_img = (SDL_Surface *)
			malloc(sizeof(SDL_Surface));
		if (!(doom->mdl->player.weap_arr[n].weap_img))
			ft_die("Error allocating weap_img!\n");
		n++;
	}
}

static void	draw_surface_with_alpha_blurred(int x, int y, SDL_Surface *surf, SDL_Surface *buff)
{
	int *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;
	int	cel_shade;
	double ratio;

	cel_shade = 0;
	ratio = 0.0;
	iter = 0;
	i = x;
	limit_x = x + surf->w;
	limit_total = surf->w * surf->h;
	pix[0] = (int*)buff->pixels;
	pix[1] = (int*)surf->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			if (cel_shade)
			{
				if (pix[1][iter] >> 24 >= 0xff)
					pix[0][(y * buff->w) + x] = blend_argb_ratio(pix[1][iter], pix[0][(y * buff->w) + x], ratio - 0.15);
				cel_shade--;
				if (ratio == 0.48)
					ratio = 0.0;
				if (ratio == 0.5)
					ratio = 0.72;
				x++;
				iter++;
			}
			else
			{
				double modi = -0.35;
				int		dist = 1;
				if ((pix[1][iter] >> 24) >= 0xff && ratio == 0.0)
				{
					cel_shade = 1;
					ratio = 0.5;
					//pix[0][(y * buff->w) + x - 2] = blend_argb_ratio(pix[1][iter], pix[0][(y * buff->w) + x], 0.85 + modi);
					pix[0][(y * buff->w) + x - dist] = blend_argb_ratio(pix[1][iter], pix[0][(y * buff->w) + x], 0.75 + modi);
					//puts("shading begin");
					continue ;
				}
				if (x < limit_x - 3 && pix[1][iter + 1] >> 6 == 0x00 && ratio == 0.72)
				{
					cel_shade = 1;
					ratio = 0.48;
					//pix[0][(y * buff->w) + x + 2] = blend_argb_ratio(pix[1][iter], pix[0][(y * buff->w) + x], 0.85 + modi);
					pix[0][(y * buff->w) + x + dist] = blend_argb_ratio(pix[1][iter], pix[0][(y * buff->w) + x], 0.75 + modi);
					//puts("shading end");
					continue ;
				}
				if ((pix[1][iter] >> 24) >= 0xff) {
					pix[0][(y * buff->w) + x++] = pix[1][iter++];
					//iter++;
					//x++;
				}
				else
				{
					iter++;
					x++;
				}
			}
		}
		ratio = 0.0;
		cel_shade = 0;
		y++;
	}
}

static void	draw_surface_with_alpha_cel_shaded_black(int x, int y, SDL_Surface *surf, SDL_Surface *buff)
{
	int *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;
	int	cel_shade;
	double ratio;

	cel_shade = 0;
	ratio = 0.0;
	iter = 0;
	i = x;
	limit_x = x + surf->w;
	limit_total = surf->w * surf->h;
	pix[0] = (int*)buff->pixels;
	pix[1] = (int*)surf->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			if (cel_shade)
			{
				if (pix[1][iter] >> 24 >= 0xff)
					pix[0][(y * buff->w) + x] = 0xff000000;
				cel_shade--;
				if (ratio == 0.48)
					ratio = 0.0;
				if (ratio == 0.5)
					ratio = 0.72;
				x++;
				iter++;
			}
			else
			{
				if ((pix[1][iter] >> 24) >= 0xff && ratio == 0.0)
				{
					cel_shade = 1;
					ratio = 0.5;
					pix[0][(y * buff->w) + x - 1] = 0xff000000;
					continue ;
				}
				if (x < limit_x - 3 && pix[1][iter + 1] >> 24 == 0x00 && ratio == 0.72)
				{
					cel_shade = 1;
					ratio = 0.48;
					pix[0][(y * buff->w) + x + 1] = 0xff000000;
					continue ;
				}
				if ((pix[1][iter] >> 24) >= 0xff) {
					pix[0][(y * buff->w) + x++] = pix[1][iter++];
				}
				else
				{
					iter++;
					x++;
				}
			}
		}
		ratio = 0.0;
		cel_shade = 0;
		y++;
	}
}

static void	draw_surface_with_alpha_cel_shaded(int x, int y, SDL_Surface *surf, SDL_Surface *buff)
{
	uint32_t *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;
	int	cel_shade;
	double ratio;

	cel_shade = 0;
	ratio = 0.0;
	iter = 0;
	i = x;
	limit_x = x + surf->w;
	limit_total = surf->w * surf->h;
	pix[0] = (uint32_t*)buff->pixels;
	pix[1] = (uint32_t*)surf->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			if (cel_shade)
			{
				uint32_t new;
				new = invert_color(pix[0][(y * buff->w) + x]);
				if (pix[1][iter] >> 24 != 0xff)
					pix[0][(y * buff->w) + x] = blend_argb_ratio(new, pix[0][(y * buff->w) + x], 0.25);
				cel_shade--;
				if (ratio == 0.48)
					ratio = 0.0;
				if (ratio == 0.5)
					ratio = 0.72;
				x++;
				iter++;
			}
			else
			{
				if ((pix[1][iter] >> 24) != 0xff && ratio == 0.0)
				{
					cel_shade = 1;
					ratio = 0.5;
					uint32_t new;
					new = invert_color(pix[0][(y * buff->w) + x - 1]);
					pix[0][(y * buff->w) + x - 1] = blend_argb_ratio(new, pix[0][(y * buff->w) + x - 1], 0.5);
					new = invert_color(pix[0][(y * buff->w) + x - 2]);
					pix[0][(y * buff->w) + x - 2] = blend_argb_ratio(new, pix[0][(y * buff->w) + x - 2], 0.75);
					continue ;
				}
				if (x < limit_x - 3 && pix[1][iter + 1] >> 24 == 0x00 && ratio == 0.72)
				{
					cel_shade = 1;
					ratio = 0.48;
					uint32_t new;
					new = invert_color(pix[0][(y * buff->w) + x + 1]);
					pix[0][(y * buff->w) + x + 1] = blend_argb_ratio(new, pix[0][(y * buff->w) + x + 1], 0.5);
					new = invert_color(pix[0][(y * buff->w) + x + 2]);
					pix[0][(y * buff->w) + x + 2] = blend_argb_ratio(new, pix[0][(y * buff->w) + x + 2], 0.75);
					continue ;
				}
				if ((pix[1][iter] >> 24) != 0xff) {
					pix[0][(y * buff->w) + x++] = pix[1][iter++];
				}
				else
				{
					iter++;
					x++;
				}
			}
		}
		ratio = 0.0;
		cel_shade = 0;
		y++;
	}
}

static void	draw_surface_with_alpha_silhouette(int x, int y, SDL_Surface *surf, SDL_Surface *buff)
{
	int *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;
	int	cel_shade;
	double ratio;

	cel_shade = 0;
	ratio = 0.0;
	iter = 0;
	i = x;
	limit_x = x + surf->w;
	limit_total = surf->w * surf->h;
	pix[0] = (int*)buff->pixels;
	pix[1] = (int*)surf->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			if (cel_shade)
			{
				if (pix[1][iter] >> 24 >= 0xff)
					pix[0][(y * buff->w) + x] = blend_argb_ratio(pix[1][iter], pix[0][(y * buff->w) + x], ratio);
				cel_shade--;
				if (ratio == 0.72)
					ratio = 0.0;
				if (ratio == 0.5)
					ratio = 0.72;
				if (ratio == 0.5)
					ratio -= 0.25;
				if (ratio == 0.48)
					ratio += 0.24;
				x++;
				iter++;
			}
			else
			{
				if ((pix[1][iter] >> 24) >= 0xff && ratio == 0.0)
				{
					cel_shade = 2;
					ratio = 0.5;
					//puts("shading begin");
					continue ;
				}
				if (x < limit_x - 3 && pix[1][iter + 3] >> 24 == 0x00 && ratio == 0.72)
				{
					cel_shade = 2;
					ratio = 0.48;
					//puts("shading end");
					continue ;
				}
				if ((pix[1][iter] >> 24) >= 0xff) {
					pix[0][(y * buff->w) + x++] = pix[1][iter++];
					//iter++;
					//x++;
				}
				else
				{
					iter++;
					x++;
				}
			}
		}
		ratio = 0.0;
		cel_shade = 0;
		y++;
	}
}

void	handle_player_health_bar(t_doom *doom)
{
	float	percentage;
	int		pos;

	percentage = doom->mdl->player.hp.cur / 10;
	pos = (int)percentage;
	doom->mdl->player.active_health_bar = doom->sprites->txt_health_bar[pos];
	draw_surface(0, WIN_HEIGHT - 35, doom->mdl->player.active_health_bar,
		doom->game->hud_location);
}

void	handle_player_fuel_bar(t_doom *doom)
{
	float	percentage;
	int		pos;

	percentage = doom->mdl->player.weap_arr[JETPACK].ammo_cur / 100;
	pos = (int)percentage;
	doom->mdl->player.active_fuel_bar = doom->sprites->txt_fuel_bar[pos];
	draw_surface(0, WIN_HEIGHT - 75, doom->mdl->player.active_fuel_bar,
		doom->game->hud_location);
}

static int	handle_space_ammo_bar(t_doom *doom, int space)
{
	if (doom->mdl->player.weap_id == SHOTGUN)
		space -= 20;
	else
		space -= 10;
	return (space);
}

void	handle_player_ammo_bar(t_doom *doom)
{
	int		ammo;
	int		i;
	int		space;

	ammo = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_cur;
	i = 1;
	space = 0;
	while (i <= ammo)
	{
		if (doom->mdl->player.reload_time <= 0)
		{
			if (doom->mdl->player.weap_id == PISTOL)
				draw_ignore_alpha((WIN_WIDTH - 10) + space, WIN_HEIGHT - 40,
					doom->sprites->txt_pistol_bar, doom->game->hud_location);
			else if (doom->mdl->player.weap_id == SHOTGUN)
				draw_ignore_alpha(WIN_WIDTH - 10 + space - 10, WIN_HEIGHT - 45,
					doom->sprites->txt_shotgun_bar, doom->game->hud_location);
			else if (doom->mdl->player.weap_id == ASSAULT_RIFLE)
				draw_ignore_alpha((WIN_WIDTH - 10) + space, WIN_HEIGHT - 40,
					doom->sprites->txt_rifle_bar, doom->game->hud_location);
			space = handle_space_ammo_bar(doom, space);
		}
		i++;
	}
}

void	handle_clip_bar(t_doom *doom)
{
	int		clips;
	int		i;
	int		space;

	clips = doom->mdl->player.weap_arr[doom->mdl->player.weap_id].ammo_res;
	space = 0;
	i = 0;
	while (i < clips)
	{
		draw_ignore_alpha((WIN_WIDTH - 20) + space, WIN_HEIGHT - 80,
			doom->sprites->txt_clip_bar, doom->game->hud_location);
		space -= 20;
		i++;
	}
}

void	handle_weapon_bar_shaded(t_doom *doom)
{
	if (doom->mdl->player.weap_id == PISTOL)
		draw_surface_with_alpha_cel_shaded(WIN_WIDTH - 150, 0,
			doom->mdl->player.weap_arr[0].weap_img, doom->game->hud_location);
	else if (doom->mdl->player.weap_id == SHOTGUN)
		draw_surface_with_alpha_cel_shaded(WIN_WIDTH - 240, 0,
			doom->mdl->player.weap_arr[1].weap_img, doom->game->hud_location);
	else if (doom->mdl->player.weap_id == ASSAULT_RIFLE)
		draw_surface_with_alpha_cel_shaded(WIN_WIDTH - 250, 0,
			doom->mdl->player.weap_arr[2].weap_img, doom->game->hud_location);
}

void	handle_weapon_bar(t_doom *doom)
{
	if (doom->game->cel_shade_hud)
		return (handle_weapon_bar_shaded(doom));
	if (doom->mdl->player.weap_id == PISTOL && !doom->mdl->player.is_running)
		draw_ignore_alpha(WIN_WIDTH - 150, 0,
			doom->mdl->player.weap_arr[0].weap_img, doom->game->hud_location);
	else if (doom->mdl->player.weap_id == SHOTGUN
		&& !doom->mdl->player.is_running)
		draw_ignore_alpha(WIN_WIDTH - 260, 0,
			doom->mdl->player.weap_arr[1].weap_img, doom->game->hud_location);
	else if (doom->mdl->player.weap_id == ASSAULT_RIFLE
		&& !doom->mdl->player.is_running)
		draw_ignore_alpha(WIN_WIDTH - 260, 0,
			doom->mdl->player.weap_arr[2].weap_img, doom->game->hud_location);
}

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

static void	render_crosshair(t_model *mdl)
{
	if (mdl->player.weap_id == PISTOL)
		render_pistol_ch(mdl);
	else if (mdl->player.weap_id == SHOTGUN)
		render_shotgun_ch(mdl);
	else if (mdl->player.weap_id == ASSAULT_RIFLE)
		render_assault_rifle_ch(mdl);
}

void	render_game_hud(t_doom *doom)
{
	if (doom->game->hud_location == NULL)
		doom->game->hud_location = doom->game->buff;
	handle_player_health_bar(doom);
	if (doom->mdl->player.weap_arr[JETPACK].do_own)
		handle_player_fuel_bar(doom);
	handle_player_ammo_bar(doom);
	handle_clip_bar(doom);
	handle_weapon_bar(doom);
	if (doom->mdl->player.has_key)
		draw_ignore_alpha(WIN_WIDTH - 90, 300, doom->sprites->txt_key_hud,
			doom->game->hud_location);
	if (!doom->game->show_info)
		render_crosshair(doom->mdl);
}
