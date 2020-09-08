/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 19:59:56 by msuarez-          #+#    #+#             */
/*   Updated: 2020/09/08 19:59:57 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		window_close(t_doom *p)
{
	(void)p;
	exit(0);
}

int		keyboard_press(int key, t_doom *p)
{
	(void)key;
	(void)p;
	return (1);
}
int		keyboard_release(int key, t_doom *p)
{
	(void)key;
	(void)p;
	return (1);
}

int		movement(t_doom *p)
{
	(void)p;
	return (1);
}

int		mouse_key(int key, int x, int y, t_doom *p)
{
	(void)key;
	(void)x;
	(void)y;
	(void)p;
	return (1);
}

int		mouse_move(int x, int y, t_doom *p)
{
	(void)x;
	(void)y;
	(void)p;
	return (1);
}
