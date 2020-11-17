#include "renderer.h"

static double g_frustrum[6][4] = {
	{0, 0,  1, 1}, // Near
	{0, 0, -1, 1}, // Far
	{0, -1, 0, 1}, // Top
	{0,  1, 0, 1}, // Bottom
	{ 1, 0, 0, 1}, // Left
	{-1, 0, 0, 1}, // Right
};

int inside_plane(double coef[4], t_xyzw vert)
{
return (0
	+ coef[0] * vert.x
	+ coef[1] * vert.y
	+ coef[2] * vert.z
	+ coef[3] * vert.w >= 0);
}

// https://stackoverflow.com/questions/7604322/clip-matrix-for-3d-perspective-projection
t_vert clip(t_vert start, t_vert end, double side)
{
	double s;

	s = (start.w - side) / (start.w - end.w);
	return ((t_vert){
		(s * start.x) + ((1 - s) * end.x),
		(s * start.y) + ((1 - s) * end.y),
		(s * start.z) + ((1 - s) * end.z),
		side
	});
}

// If (w > 0), it's in front of the camera.
void clip_triangle(t_face tri)
{
	// if (!inside_plane(g_frustrum[0], tri.vert[v]))
		// tri.vert[v] = clip(tri.vert[v])
}

// -----------------

// https://gamedevelopment.tutsplus.com/tutorials/how-to-dynamically-slice-a-convex-shape--gamedev-14479
t_face	slice_triangle(t_vert a, t_vert b, t_vert c, double distance[3]);

t_mesh	slice_all_triangles(t_mesh mesh, t_xyz normal)
{
	t_mesh out;
	out.faces = mesh.faces * 2;

	for (int i = 0, of = 0; i < mesh.faces; ++i, ++of)
	{
		t_face f = mesh.face[i];
		double d[3];
		d[0] = vec3_dot(vec43(f.vert[0]), normal);
		d[1] = vec3_dot(vec43(f.vert[1]), normal);
		d[2] = vec3_dot(vec43(f.vert[2]), normal);

		if (d[0] * d[1] < 0)
			out.face[of++] = slice_triangle(f.vert[0], f.vert[1], f.vert[2], d);
		else if (d[1] * d[2] < 0)
			out.face[of++] = slice_triangle(f.vert[2], f.vert[0], f.vert[1], d);
		else if (d[1] * d[2] < 0)
			out.face[of++] = slice_triangle(f.vert[1], f.vert[2], f.vert[0], d);
		else // no clipping
			out.face[of++] = f;
	}
	return (out);
}

t_face	slice_triangle(t_vert a, t_vert b, t_vert c, double distance[3])
{
	// lord help me
}

// ----------------

// https://www.cubic.org/docs/3dclip.htm#ma2

clip_line_plane(t_vert a, t_vert b, t_xyz normal)
{
	double dist_a;
	double dist_b;
	float s;
	t_xyz intersection;

	dist_a = vec3_dot(vec43(a), normal);
	dist_b = vec3_dot(vec43(b), normal);
	if ((dist_a < 0) && (dist_b < 0))
	{
		// remove line
	}
	else if ((dist_a >= 0) && (dist_b >= 0))
	{
		// no clipping
	}
	else
	{
		s = dist_a / (dist_a - dist_b);
		intersection.x = a.x + (b.x - a.x) * s;
		intersection.y = a.y + (b.y - a.y) * s;
		intersection.z = a.z + (b.z - a.z) * s;
	}
}

clip_polygon_plane(t_face f)
{
	// clip triangle, must handle possible quads
	// -> call clip_line_plane for each edge
}

clip_polygon_viewport(t_face f)
{
	// -> call clip_polygon_plane for each side
	// note: may create lots of new faces
}

clip_mesh_viewport(t_mesh mesh)
{
	// -> call clip_polygon_viewport for each face
	// note: may create lots of new faces
}

// -------

// my own brainstorming

t_mesh	clip(t_mesh mesh);

render_frame_mocup()
{
	t_mesh clip_space;
	t_mesh clipped = clip(clip_space);
}

t_mesh	clip_whole_object(t_mesh mesh)
{
	// Let's pretend this mesh is a single triangle,
	// which has one vertex out of bounds:
	//
	//    v1 /^\
	//    --*---*---
	//     /     \
	// v2 /_______\ v3
	//
	// v1 needs to be discarded, 2 new verts are created,
	// the new verts (v4, v5) will form a quad with v2 and v3,
	// the new quad will need to be triangulated as well.

	// data is separated into a list of individual (unsorted) vertices
	// and a list of individual (unsorted) faces (with CCW sorted vertices)

	t_mesh out = mesh_duplicate(mesh);
	for (int f = 0; f < mesh.faces; ++f)
	{
		if (needs_clipping(mesh.face[f]))
			modify_faces(&out, mesh.face[f]);
	}

}

int		needs_clipping(t_face face)
{
	// if all point are within view, return TRUE
	// else return FALSE
}

void	modify_faces(t_mesh *out, t_face target)
{
	// if no part of the polygon crosses the viewport,
	// delete the entire face and return.

	// start looping against near, left, right, top, bottom planes
	// ...
}