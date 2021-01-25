#include "renderer.h"

void	linep(const char *name, t_xy_line line)
{
	printf("%s{(%8.3f, %8.3f), (%8.3f, %8.3f)}\n",
		name, line.start.x, line.start.y, line.stop.x, line.stop.y);
}

t_xy_line		line(double ax, double ay, double bx, double by)
{
	return ((t_xy_line){(t_xy){ax, ay}, (t_xy){bx, by}, 0xffffff});
}

t_xy_line		line_xy(t_xy start, t_xy stop, int color)
{
	return ((t_xy_line){start, stop, color});
}

signed			line_is_zero(t_xy_line line)
{
	return (
		line.start.x == 0 &&
		line.start.y == 0 &&
		line.stop.x == 0 &&
		line.stop.y == 0);
}

t_xy_line		line_add_offset(t_xy_line line, t_xy offset)
{
	t_xy_line out;

	out.start = vec2_add(line.start, offset);
	out.stop = vec2_add(line.stop, offset);
	out.color = line.color;
	return (out);
}

t_xy_line		line_sub_offset(t_xy_line line, t_xy offset)
{
	t_xy_line out;

	out.start = vec2_sub(out.start, offset);
	out.stop = vec2_sub(out.stop, offset);
	out.color = line.color;
	return (out);
}

t_xy_line		line_rot(t_xy_line line, double angle)
{
	t_xy_line out;

	out.start = vec2_rot(line.start, angle);
	out.stop = vec2_rot(line.stop, angle);
	out.color = line.color;
	return (out);
}

// 0 when perpendicular,
// 1 when parallel and both vectors are normal,
// otherwise the distance along the normal.

double			vec2_projected_length(t_xy v, t_xy normal)
{
	return (vec2_dot(v, normal));
}

// Absolute distance from line.
// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Vector_formulation

double			vec2_point_line_distance(t_xy point, t_xy line, t_xy dir)
{
	t_xy	point_to_line;
	double	projected_length;
	t_xy	projected_offset;
	t_xy	perpendicular;

	point_to_line = vec2_sub(line, point);                              //             (q - p)
	projected_length = vec2_projected_length(point_to_line, dir);       //             (q - p) ⋅ n
	projected_offset = vec2_mul(dir, projected_length);                 //           (((q - p) ⋅ n) * n)
	perpendicular = vec2_sub(point_to_line, projected_offset);          // (q - p) - (((q - p) ⋅ n) * n)
	return (vec2_mag(perpendicular));
}

// Closest position on the line.
t_xy	vec2_point_to_line(t_xy point, t_xy line_pos, t_xy line_dir)
{
	t_xy	point_to_line;
	double	projected_length;
	t_xy	projected_offset;

	point_to_line = vec2_sub(line_pos, point);                          //            (q - p)
	projected_length = -vec2_projected_length(point_to_line, line_dir); //            (q - p) ⋅ n
	projected_offset = vec2_mul(line_dir, projected_length);            //           ((q - p) ⋅ n) * n
	return (vec2_add(line_pos, projected_offset));                      //       q + ((q - p) ⋅ n) * n
}

// 2D Cross Product; Determinant of a 2D matrix.
// Does not result in useful values, only the sign matters.
// [a   b]   [x2-x1   x3-x1]
// [  X  ] = [      X      ] =             a*d - b*c
// [c   d]   [y2-y1   y3-y1]   (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1)

signed			vec2_point_side(t_xy point, t_xy start, t_xy end)
{
	double ad_bc;

	ad_bc = (end.x - start.x) * (point.y - start.y) -
			(point.x - start.x) * (end.y - start.y);
	if (ad_bc > 0)
		return (LINE_SIDE_RIGHT);
	else if (ad_bc < 0)
		return (LINE_SIDE_LEFT);
	else
		return (LINE_SIDE_NONE);
}

// Length that adjescent projects onto the hypotenuse
//         |\    <- clipped line
// line -> | \
// to    ------- <- clipping plane
// clipping    \
// plane        \

double			vec2_project_to_hypotenuse(t_xy adjescent, t_xy hypotenuse)
{
	t_xy	v_normal;
	double	v_length;
	double	projected;

	v_normal = vec2_norm(adjescent);
	v_length = vec2_mag(adjescent);
	projected = vec2_projected_length(hypotenuse, v_normal);
	return (vec2_mag(hypotenuse) * (v_length / projected));
}

static t_xy		clip(t_xy p1, t_xy p2, t_xy line_pos, t_xy line_dir)
{
	t_xy	line;
	t_xy	on_clip_plane;
	t_xy	p1_to_plane;
	double	length;
	t_xy	offset;

	line = vec2_sub(p2, p1);
	on_clip_plane = vec2_point_to_line(p1, line_pos, line_dir);
	p1_to_plane = vec2_sub(on_clip_plane, p1);
	length = vec2_project_to_hypotenuse(p1_to_plane, line);
	offset = vec2_mul(vec2_norm(line), length);
	return (vec2_add(p1, offset));
}

// The direction of the clipping line defines which side is "inside"
// Let's decide that "right" (1) is "inside"
// The left side will be moved to the clipping plane.

void			vec2_clip_line(t_xy_line in, t_xy_line *out, t_xy_line plane)
{
	t_xy	line_dir;
	signed	p1_side;
	signed	p2_side;

	out->start = in.start;
	out->stop = in.stop;
	line_dir = vec2_norm(vec2_sub(plane.stop, plane.start));
	p1_side = vec2_point_side(in.start, plane.start, plane.stop);
	p2_side = vec2_point_side(in.stop, plane.start, plane.stop);
	if (p1_side * p2_side == LINE_SIDES_CROSS)
	{
		if (p1_side == LINE_SIDE_OUT)
			out->start = clip(in.start, in.stop, plane.start, line_dir);
		else
			out->stop = clip(in.stop, in.start, plane.start, line_dir);
	}
	else if (p1_side == LINE_SIDE_OUT && p2_side == LINE_SIDE_OUT)
	{
		out->start = vec2(0, 0);
		out->stop = vec2(0, 0);
	}
	else if (p1_side == LINE_SIDE_NONE && p2_side == LINE_SIDE_OUT)
	{
		out->start = vec2(0, 0);
		out->stop = vec2(0, 0);
	}
	else if (p2_side == LINE_SIDE_NONE && p1_side == LINE_SIDE_OUT)
	{
		out->start = vec2(0, 0);
		out->stop = vec2(0, 0);
	}
}

t_xy_line		*set_clip_bounds(t_xy topl, t_xy topr, t_xy botr, t_xy botl)
{
	static t_xy_line bound[4] = {0};

	bound[0] = line_xy(topl, topr, 0xffffff);
	bound[1] = line_xy(topr, botr, 0xffffff);
	bound[2] = line_xy(botr, botl, 0xffffff);
	bound[3] = line_xy(botl, topl, 0xffffff);
	return (bound);
}

void			clip_to_bounds(t_xy_line in, t_xy_line *out, t_xy_line edge[4])
{
	vec2_clip_line(in, out, edge[0]);
	vec2_clip_line(*out, out, edge[1]);
	vec2_clip_line(*out, out, edge[2]);
	vec2_clip_line(*out, out, edge[3]);
}

t_xy_line		line_clamp(t_xy_line in, t_xy min, t_xy max)
{
	in.start.x = (in.start.x < min.x) ? min.x : in.start.x;
	in.stop.x  = (in.stop.x  < min.x) ? min.x : in.stop.x;

	in.start.y = (in.start.y < min.y) ? min.y : in.start.y;
	in.stop.y  = (in.stop.y  < min.y) ? min.y : in.stop.y;

	in.start.x = (in.start.x > max.x) ? max.x : in.start.x;
	in.stop.x  = (in.stop.x  > max.x) ? max.x : in.stop.x;

	in.start.y = (in.start.y > max.y) ? max.y : in.start.y;
	in.stop.y  = (in.stop.y  > max.y) ? max.y : in.stop.y;

	return (in);
}
