/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_camera.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 08:16:00 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/21 18:25:28 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CAMERA_H
# define FT_CAMERA_H

#include "doom_nukem.h"

typedef struct	s_camera
{
	t_xyz		dir;
	t_matrix	rot;
	t_matrix	world_m;
	t_matrix	view_m;
	t_matrix	screen_m;
	t_matrix	full_m;
}				t_camera;

t_camera init_camera();

#endif
