#include "doom_nukem.h"

/*
** load_mesh_obj()
** Parses through a Wavefront OBJ file.
** Note: indexes start from 1
** Vert lines: "v -1.000000 1.000000 0.000000" ("x y z")
** Face lines: "f 1/1/1 2/2/1 4/3/1 3/4/1" ("v/vt/vn")
** v: vertex index
** vt: texture UV coordinate
** vn: vertex normal
*/

static void	free_array(char **array)
{
	ssize_t i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

static void	fill_faces(t_face *face, int n, t_vert *buffer)
{
	int		i;

	// printf("fill face with %d verts\n", n);
	assert(face->verts == n);
	i = 0;
	while (i < n)
	{
		face->vert[i] = buffer[i];
		++i;
	}
}

static void	fill_mesh(t_mesh *mesh, int n, t_face *buffer)
{
	int		i;

	// printf("fill mesh with %d faces\n", n);
	assert(mesh->faces == n);
	i = 0;
	while (i < n)
	{
		mesh->face[i] = buffer[i];
		++i;
	}
}

t_mesh	load_mesh_obj(const char *file)
{
	char	*line;
	int		fd = open(file, O_RDONLY);
	int		vcount = 0;
	int		fcount = 0;
	t_vert	file_vertex[1024] = {0}; // Verts per file
	t_vert	face_vertex[16] = {0}; // Verts per face
	t_face	face_buffer[1024] = {0}; // Faces per mesh
	t_mesh mesh;

	// printf("file_vertex %zu face_vertex %zu face_buffer %zu\n",
	// 	sizeof(file_vertex), sizeof(face_vertex), sizeof(face_buffer));

	while (vcount < 1024 && get_next_line(fd, &line) > 0)
	{
		char **split;
		// vertex
		if (ft_strncmp(line, "v ", 2) == 0)
		{
			// printf("OBJ read: \"%s\"\n", line);
			split = ft_strsplit(line, ' ');
			file_vertex[vcount++] = vec4(
				atof(split[1]),
				atof(split[2]),
				atof(split[3]),
				1);
			free_array(split);
		}
		// face
		else if (ft_strncmp(line, "f ", 2) == 0)
		{
			// printf("OBJ read: \"%s\"\n", line);
			split = ft_strsplit(line, ' ');
			ssize_t i = 1;
			while (split[i])
			{
				int vindex = atoi(split[i]);
				face_vertex[i - 1] = file_vertex[vindex - 1];
				++i;
			}
			face_buffer[fcount] = init_face(i - 1); //! Technically very bad.
			fill_faces(face_buffer + fcount, i - 1, face_vertex);
			free_array(split);
			++fcount;
		}
		free(line);
	}
	mesh = init_mesh(fcount); //! Technically very bad.
	fill_mesh(&mesh, fcount, face_buffer);
	return (mesh);
}

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

	// printf("init face with %d verts\n", n);
	assert(3 <= n && n <= 16);
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

void		free_verts(t_face *face)
{
	free(face->vert);
	face->verts = 0;
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
	mesh->faces = 0;
}

void		mesh_draw(unsigned int *surface, const t_mesh *mesh)
{
	int i;
	int p;
	t_vert start;
	t_vert end;

	// printf("enter mesh_draw (%i faces)\n", mesh->faces);
	i = 0;
	while (i < mesh->faces)
	{
		p = 0;
		while (p < mesh->face[i].verts - 1)
		{
			start = mesh->face[i].vert[p++];
			end = mesh->face[i].vert[p];
			draw(surface, vec42(start), vec42(end), 0xFFFFFF);
			// printf("loop draw {%.1f %.1f} - {%.1f %.1f}\n", start.x, start.y, end.x, end.y);
		}
		start = mesh->face[i].vert[p];
		end = mesh->face[i].vert[0];
			draw(surface, vec42(start), vec42(end), 0xFFFFFF);
		// printf("last draw {%.1f %.1f} - {%.1f %.1f}\n", start.x, start.y, end.x, end.y);
		++i;
	}
}

t_mesh		mesh_duplicate(t_mesh mesh)
{
	t_mesh out;
	int i;

	out = init_mesh(mesh.faces);
	i = 0;
	while (i < mesh.faces)
	{
		out.face[i] = init_face(mesh.face[i].verts);
		ft_memcpy(
			out.face[i].vert,
			mesh.face[i].vert,
			sizeof(t_vert) * mesh.face[i].verts);
	}
	return (out);
}

/*
** \brief Applies the given matrix to all verts in the mesh.
** \return Newly allocated data, containing the transformed verts.
*/
// t_mesh		mesh_transform(t_matrix matrix, t_mesh mesh)
// {
// 	int i;
// 	int v;
// 	t_mesh out;

// 	assert(mesh.faces >= 1);
// 	out = mesh_duplicate(mesh);
// 	i = 0;
// 	while (i < out.faces)
// 	{
// 		v = 0;
// 		while (v < out.face[i].verts)
// 		{
// 			out.face[i].vert[v] = apply_m(matrix, out.face[i].vert[v]);
// 			++v;
// 		}
// 		++i;
// 	}
// 	return (out);
// }
