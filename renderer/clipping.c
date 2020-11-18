#include "renderer.h"

#define CLIP_TRIVIAL_ACCEPT 1
#define CLIP_TRIVIAL_REJECT -1
#define CLIP_REQUIRED 0

// https://www.cs.helsinki.fi/group/goa/viewing/leikkaus/lineClip.html
// https://www.youtube.com/watch?v=VMD7fsCYO9o

render_frame_mocup()
{
	t_mesh clip_space;
	t_mesh clipped = clip_whole_object(clip_space);
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

	int clip_type;
	t_mesh out = mesh_duplicate(mesh); // probably can't use soon
	for (int f = 0; f < mesh.faces; ++f)
	{
		clip_type = get_clip_type(mesh.face[f]);
		if (clip_type == CLIP_TRIVIAL_ACCEPT)
			; // accept as-is
		else if (clip_type == CLIP_TRIVIAL_REJECT)
			; // delete entire face (how?)
		else
			modify_faces(&out, mesh.face[f]);
	}

}

int		get_clip_type(t_face face)
{
	int a;
	int b;
	int c;

	a = get_outcode(face.vert[0]);
	b = get_outcode(face.vert[1]);
	c = get_outcode(face.vert[2]);

	// all outcodes are within view (region 0)
	if ((a | b | c) == 0)
		return (CLIP_TRIVIAL_ACCEPT);
	// all outcodes (A&B, B&C, C&A) share a region
	if (!!(a & b) + !!(b & c) + !!(c & a) == 3)
		return (CLIP_TRIVIAL_REJECT);
	else
		return (CLIP_REQUIRED);
}

int get_outcode(t_vert v)
{
	int outcode;

	outcode = 0;
	outcode |= 0b000001 * (v.x <= -v.w);
	outcode |= 0b000010 * (v.x >= v.w);
	outcode |= 0b000100 * (v.y <= -v.w);
	outcode |= 0b001000 * (v.y >= v.w);
	outcode |= 0b010000 * (v.z <= -v.w);
	outcode |= 0b100000 * (v.z >= v.w);
}

void	modify_faces(t_mesh *out, t_face target)
{
	// start looping against near, left, right, top, bottom planes
	// ... profit?
	// Now the big question, do we allocate new data for vertices
	// or add new verts to the old ones (how to clean those up between frames?)
	// or do something else?
}

// Probably not needed anymore, keep for now:
int		vert_within_view(t_vert vert)
{
	// -w \       / w
	//     \     /
	//      \   /
	//     w0 .
	return ((-vert.w <= vert.x && vert.x <= vert.w)
		&&  (-vert.w <= vert.y && vert.y <= vert.w)
		&&  (-vert.w <= vert.z && vert.z <= vert.w));
}
