#include "renderer.h"

// https://www.cs.helsinki.fi/group/goa/viewing/leikkaus/lineClip.html
// https://www.youtube.com/watch?v=VMD7fsCYO9o


// https://www.youtube.com/watch?v=VMD7fsCYO9o
// https://www.youtube.com/watch?v=DHvXvJowalM
// https://www.youtube.com/watch?v=ai9SwUQF74c

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

t_vert			lerp_vert(t_vert a, t_vert b)
{
	t_xyzw wa;
	t_xyzw wb;
	t_xyz t;

	// Todo: Does this fail because vert_within_view() has two conditions
	// and we don't know which one failed?
	wa = vec4((a.w - a.x), (a.w - a.y), (a.w - a.z), a.w);
	wb = vec4((b.w - b.x), (b.w - b.y), (b.w - b.z), b.w);

	t = vec3( // [0.0 - 1.0] range
		wa.x / (wa.x - wb.x),
		wa.y / (wa.y - wb.y),
		wa.z / (wa.z - wb.z));

	return vec4(
		a.x + (t.x * (b.x - a.x)),
		a.y + (t.y * (b.y - a.y)),
		a.z + (t.z * (b.z - a.z)),
		a.w + (      (b.w - a.w)) // note: ensure correctness
	);
}

t_actual_face	*faceclipper(t_actual_face *face, t_global_vert **vlist)
{
	t_face_vert *result         = NULL;

	t_face_vert	*prev           = list2fvert(face->vert, LAST);
	int			prev_inside     = vert_within_view(prev->data->pos);

	t_face_vert *curr           = face->vert;
	int			curr_inside;

	while (curr != NULL)
	{
		curr_inside = vert_within_view(curr->data->pos);

		if (curr_inside ^ prev_inside) // One inside, one outside. Lerp prev->curr.
		{
			t_vert lerped = lerp_vert(prev->data->pos, curr->data->pos);
			result = face_vert_add(result, new_vert(vlist, lerped));
		}

		if (curr_inside)
		{
			result = face_vert_add(result, curr);
		}

		prev           = curr;
		prev_inside    = curr_inside;
		curr           = curr->next;
	}

	// Return NULL or vertices in CCW order.
	// Todo: If lerp_vert doesn't work, -w and w needs to be checked explicitly.
	return (result);
}

int				get_outcode(t_vert v)
{
	int outcode;

	outcode = 0;
	outcode |= OUTCODE_LEFT  * (v.x <= -v.w);
	outcode |= OUTCODE_RIGHT * (v.x >=  v.w);
	outcode |= OUTCODE_UP    * (v.y <= -v.w);
	outcode |= OUTCODE_DOWN  * (v.y >=  v.w);
	outcode |= OUTCODE_NEAR  * (v.z <=  v.w); // note: ensure correctness
	outcode |= OUTCODE_FAR   * (v.z >= -v.w);
}

int				get_clip_type(t_actual_face *face)
{
	int a;
	int b;
	int c;

	a = get_outcode(face->vert->            data->pos);
	b = get_outcode(face->vert->next->      data->pos);
	c = get_outcode(face->vert->next->next->data->pos);

	// all outcodes are within view (region 0)
	if ((a | b | c) == 0)
	{
		return (CLIP_TRIVIAL_ACCEPT);
	}
	// all outcodes (A&B, B&C, C&A) share a region
	if (!!(a & b) + !!(b & c) + !!(c & a) == 3)
	{
		return (CLIP_TRIVIAL_REJECT);
	}
	else
	{
		return (CLIP_REQUIRED);
	}
}

t_actual_face	*clip_face(t_actual_face *face, t_global_vert **vlist, int clip_type)
{
	t_actual_face *clipped = NULL;

	if (clip_type == CLIP_TRIVIAL_ACCEPT)
	{
		return (new_face(vlist, // This doesn't feel right...
			face->vert->data->pos,
			face->vert->next->data->pos,
			face->vert->next->next->data->pos
		));
	}
	else if (clip_type == CLIP_REQUIRED)
	{
		// At this point, face should begin with exactly 3 verts.
		// Should it also return only faces with exactly 3 verts?
		// (Not if they're in order like a triagle-fan.)
		clipped = faceclipper(face, vlist);
		// Link prev and next to this one...
		clipped->prev = face->prev;
		clipped->next = face->next;
		face->prev->next = clipped;
		face->next->prev = clipped;
		return (clipped);
	}
	else
	{
		return (NULL);
	}
}

// Go through obj and create a clipped copy of it,
// skip faces that are rejected entirely.
// Do NOT modify obj's contents in any way.
t_obj			obj_clip(t_obj obj)
{
	t_obj out;
	int clip_type;

	out = (t_obj){0, 0, NULL, NULL};

	assert(obj.f_count >= 1);

	while (obj.face != NULL)
	{
		if ((clip_type = get_clip_type(obj.face)) != CLIP_TRIVIAL_REJECT)
		{
			face_list_add(out.face, clip_face(obj.face, &out.vert, clip_type));
			out.f_count += 1;
			out.v_count += 3;
		}
		obj.face = obj.face->next;
	}

	return (out);
}
