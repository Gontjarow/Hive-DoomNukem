/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_object.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:10:46 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/22 03:26:18 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

#ifndef FT_OBJECT_H
# define FT_OBJECT_H

typedef t_xyz	t_vert;

/*
** A vert has X Y Z components. ("vertex")
** A face is composed of at LEAST three points. ("ngons" are possible)
** A face cannot have MORE THAN sixteen points.
** A mesh is composed of at LEAST one face.
*/

typedef struct	s_face
{
	int		verts;
	t_vert	*vert;
}				t_face;

typedef struct	s_mesh
{
	int		faces;
	t_face	*face;
}				t_mesh;

/*
** An object consists of just an optional mesh, for now.
*/

typedef struct	s_object
{
	t_mesh	mesh;
}				t_object;

t_face		init_face(int n, ...);
t_mesh		init_mesh(int n, ...);
t_object	init_object(t_mesh mesh);
t_mesh		mesh_transform(const t_mesh *mesh, const t_matrix *m);
void		free_verts(t_face *face);
void		free_faces(t_mesh *mesh);
void		mesh_draw(unsigned int *surface, const t_mesh *mesh);

#endif
