/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 20:00:00 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/08 20:00:01 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ft_die(const char *error_message)
{
	ft_putendl(error_message);
	exit(0);
}

void	setup(char *arg, t_doom *p)
{
	(void)arg;
	p->mlx = mlx_init();
	p->win = mlx_new_window(p->mlx, WIN_WIDTH, WIN_HEIGHT, "Doom Nukem");
	p->buffer = ft_new_image(p->mlx, WIN_WIDTH, WIN_HEIGHT);
}

int		main(int argc, char **argv)
{
	t_doom doom;

	if (argc != 2)
		ft_die("usage: ./doom_nukem [mapname]");
	setup(argv[1], &doom);
	mlx_hook(doom.win, EVT_KEY_DN, EVT_KEY_DN, keyboard_press, &doom);
	mlx_hook(doom.win, EVT_KEY_UP, EVT_KEY_UP, keyboard_release, &doom);
	mlx_mouse_hook(doom.win, mouse_key, &doom);
	mlx_hook(doom.win, EVT_MOUSE_MV, 0, mouse_move, &doom);
	mlx_hook(doom.win, EVT_CLOSE_WIN, 0, window_close, &doom);
	mlx_loop_hook(doom.mlx, movement, &doom);
	// render();
	mlx_loop(doom.mlx);
}
