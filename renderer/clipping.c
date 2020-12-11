#include "renderer.h"

// https://www.cs.helsinki.fi/group/goa/viewing/leikkaus/lineClip.html
// https://www.youtube.com/watch?v=VMD7fsCYO9o

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
