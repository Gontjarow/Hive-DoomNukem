/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_object.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngontjar <ngontjar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:29:26 by ngontjar          #+#    #+#             */
/*   Updated: 2020/09/22 03:12:09 by ngontjar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

/*
** \brief Allocates(!) enough memory (one block) for all given verts.
** \return Struct containing a pointer to the allocated memory.
** (Including a count of the contents.)
*/
t_face		init_face(int n, ...)
{
	va_list		arg;
	t_face		face;
	int			i;

	face = (t_face){0};
	if (n > 0 && (face.vert = malloc(n * sizeof(t_vert))))
	{
		va_start(arg, n);
		face.verts = n;
		i = 0;
		while (i < n)
		{
			face.vert[i] = va_arg(arg, t_vert);
			++i;
		}
	}
	return (face);
}

/*
** \brief Allocates(!) enough memory (one block) for all given faces.
** \return Struct containing a pointer to the allocated memory.
** (Including a count of the contents.)
*/
t_mesh		init_mesh(int n, ...)
{
	va_list		arg;
	t_mesh		mesh;
	int			i;

	mesh = (t_mesh){0};
	if (n > 0 && (mesh.face = malloc(n * sizeof(t_face))))
	{
		va_start(arg, n);
		mesh.faces = n;
		i = 0;
		while (i < n)
		{
			mesh.face[i] = va_arg(arg, t_face);
			++i;
		}
	}
	return (mesh);
}

/*
** \brief Populates an object struct.
*/
t_object	init_object(t_mesh mesh)
{
	return ((t_object){
		.mesh = mesh
	});
}

/*
** \brief Allocates(!) a new mesh with the transformation applied.
*/
t_mesh		mesh_transform(const t_mesh *mesh, const t_matrix *m)
{
	int		i;
	int		p;
	t_mesh	new;

	// Malloc room for all faces.
	printf("Allocating %d faces\n", mesh->faces);

	if (NULL == (new.face = malloc(sizeof(t_face) * mesh->faces)))
		ft_die("Not enough memory for faces at: " __FILE__);

	i = 0;
	while (i < mesh->faces)
	{
		// For each face, malloc room for all verts.
		printf("Allocating %d verts\n", mesh->face[i].verts);
		if (NULL == (new.face[i].vert = malloc(sizeof(t_vert) * mesh->face[i].verts)))
			ft_die("Not enough memory for verts at: " __FILE__);

		p = 0;
		while (p < mesh->face[i].verts)
		{
			new.face[i].vert[p] = vec3_transform(mesh->face[i].vert[p], *m);
			++p;
		}

		++i;
	}
	return (new);
}

void		free_verts(t_face *face)
{
	free(face->vert);
}

void		free_faces(t_mesh *mesh)
{
	int i;

	i = 0;
	while (i < mesh->faces)
	{
		free(mesh->face[i].vert);
		++i;
	}
	free(mesh->face);
}

void		mesh_draw(unsigned int *surface, const t_mesh *mesh)
{
	int i;
	int p;
	t_xyz start;
	t_xyz end;

	i = 0;
	while (i < mesh->faces)
	{
		p = 0;
		while (p < mesh->face[i].verts - 1)
		{
			start = mesh->face[i].vert[p++];
			end = mesh->face[i].vert[p];
			ft_draw(surface, start, end, 0xFFFFFF);
			++p;
		}
		start = mesh->face[i].vert[p];
		end = mesh->face[i].vert[0];
			ft_draw(surface, start, end, 0xFFFFFF);
		++i;
	}
}
