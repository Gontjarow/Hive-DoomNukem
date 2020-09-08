/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 19:13:53 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/08 19:59:07 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_NUKEM_H
# define DOOM_NUKEM_H

# include "libft/libft.h"
# include "mlx/mlx.h"
# include <stdlib.h>

# define WIN_WIDTH 800
# define WIN_HEIGHT 450

# define EVT_KEY_DN 2
# define EVT_KEY_UP 3
# define EVT_MOUSE_DN 4
# define EVT_MOUSE_UP 5
# define EVT_MOUSE_MV 6
# define EVT_CLOSE_WIN 17

/*
** *
** Basic structs
** *
*/

typedef struct	s_image
{
	void	*ptr;
	int		*data;
	int		width;
	int		height;
	int		bpp;
	int		line;
	int		endian;
}				t_image;

/*
** *
** Main struct
** *
*/

typedef struct	s_doom
{
	void		*mlx;
	void		*win;
	t_image		buffer;
}				t_doom;

/*
** *
** Keys
** *
*/

# ifndef __linux__

enum			e_mouse
{
	LMB = 1,
	RMB = 2,
	MMB = 3,
	WhDn = 4,
	WhUp = 5
};
# else

enum			e_mouse
{
	LMB = 1,
	RMB = 3,
	MMB = 2,
	WhDn = 5,
	WhUp = 4
};
# endif

# ifndef __linux__

enum			e_key
{
	esc = 53,
	num_plus = 69,
	num_minus = 78,
	left = 123,
	right = 124,
	down = 125,
	up = 126,
	q = 12,
	e = 14
};
# else

enum			e_key
{
	esc = 65307,
	num_plus = 65451,
	num_minus = 65453,
	left = 65361,
	right = 65363,
	down = 65364,
	up = 65362,
	q = 113,
	e = 101
};
# endif

/*
** *
** Functions
** *
*/

int				window_close(t_doom *p);
int				keyboard_press(int key, t_doom *p);
int				keyboard_release(int key, t_doom *p);
int				movement(t_doom *p);
int				mouse_key(int key, int x, int y, t_doom *p);
int				mouse_move(int x, int y, t_doom *p);

t_image			ft_new_image(int *mlx, int width, int height);
t_image			ft_new_xpm_image(int *mlx, char *filename);

#endif
