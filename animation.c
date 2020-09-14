/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:44:00 by krusthol          #+#    #+#             */
/*   Updated: 2020/09/14 18:07:23 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	render_menu(t_doom *doom)
{
	int selection_offset;

	selection_offset = 0;
	doom->alphabet_scale = 2;
	print_alphabet("hive doom nukem", doom, 512 - 420, 29);
	doom->alphabet_scale = 1;
	print_alphabet("ngontjar krusthol msuarez", doom, 512 - 364, 29 + 29 + 29);
	doom->alphabet_scale = doom->selected_le ? 2 : 1;
	selection_offset = doom->selected_le ? -14 : 0;
	print_alphabet("level editor", doom, 512 - (168 * doom->alphabet_scale), 309 + 29 + 29 + selection_offset);
	doom->alphabet_scale = doom->selected_le ? 1 : 2;
	selection_offset = doom->selected_le ? 0 : -14;
	print_alphabet("quit", doom, 512 - (56 * doom->alphabet_scale), 309 + 29 + 29 + 29 + 29 + selection_offset);

}

void		render_animation(t_doom *doom)
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
	render_menu(doom);
}

void		load_animation(t_doom *doom)
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
	}
}