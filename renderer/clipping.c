#include "renderer.h"

// https://www.cs.helsinki.fi/group/goa/viewing/leikkaus/lineClip.html
// https://www.youtube.com/watch?v=VMD7fsCYO9o


// https://www.youtube.com/watch?v=VMD7fsCYO9o
// https://www.youtube.com/watch?v=DHvXvJowalM
// https://www.youtube.com/watch?v=ai9SwUQF74c

t_vert			lerp_vert(t_vert a, t_vert b, double amount)
{
	return vec4_lerp(a, b, amount);
}

int				get_outcode(t_vert v)
{
	int outcode;

	outcode = 0;
	outcode |= OUTCODE_LEFT  * (v.x <= -v.w);
	outcode |= OUTCODE_RIGHT * (v.x >=  v.w);
	outcode |= OUTCODE_UP    * (v.y <= -v.w);
	outcode |= OUTCODE_DOWN  * (v.y >=  v.w);
	outcode |= OUTCODE_NEAR  * (v.z <= -v.w); // note: ensure correctness
	outcode |= OUTCODE_FAR   * (v.z >=  v.w);
}

int				get_clip_type(t_actual_face *face, int outcode[3])
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
		outcode[0] = a;
		outcode[1] = b;
		outcode[2] = c;
		return (CLIP_REQUIRED);
	}
}

int				clip_face(t_actual_face *face)
{
	int clip_type;
	int outcode[3];

	clip_type = get_clip_type(face, outcode);
	if (clip_type == CLIP_TRIVIAL_ACCEPT)
		return (CLIP_TRIVIAL_ACCEPT);
	else if (clip_type == CLIP_TRIVIAL_REJECT)
	{
		face->prev->next = face->next;
		recursive_fvert_free(face->vert);
		free(face);
		return (CLIP_TRIVIAL_REJECT);
	}
	else
	{
		// At this point, face should begin with exactly 3 verts.
		// It should also return only faces with exactly 3 verts.
		faceclipper(face, outcode);
		return (CLIP_REQUIRED);
	}
}

t_obj			obj_clip(t_obj obj)
{
	t_obj out;
	t_actual_face *out_begin;
	int clip_type;

	assert(obj.f_count >= 1);

	out.face = clip_face(obj.face);
	out_begin = out.face;

	while (obj.face->next != NULL)
	{
		out.face->next = clip_face(obj.face->next);
		obj.face = obj.face->next;
		out.face = out.face->next;
	}

	out.face = out_begin;
	return (out);
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
