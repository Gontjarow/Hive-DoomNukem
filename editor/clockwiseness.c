/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clockwiseness.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krusthol <krusthol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 16:21:54 by krusthol          #+#    #+#             */
/*   Updated: 2021/02/05 16:25:18 by krusthol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

static int		do_checks(double angle, int *i, int *non_convex, int *com_dir)
{
	int			dir;

	if (angle != (dir = 0))
		dir = angle > 0 ? 1 : -1;
	if (*com_dir != 0 && *com_dir != dir)
		*non_convex = 1;
	*com_dir = dir;
}

static int		warn_with_code(int code, char *str)
{
	ft_putendl(str);
	return (code);
}

static int		clockwiseness_result(t_wall *wall, t_room *room,
				int report_non_convex, const int plus_minus[2])
{
	if (wall->end.x != room->first_wall->start.x ||
		wall->end.y != room->first_wall->start.y)
	{
		if (report_non_convex)
			ft_putendl(WARNING_NON_CONVEX);
		if (plus_minus[0] > plus_minus[1])
			ft_putendl(WARNING_CCW);
		return (warn_with_code(0, WARNING_NON_CLOSED));
	}
	if (report_non_convex && plus_minus[0] < plus_minus[1])
		return (warn_with_code(0, WARNING_NON_CONVEX_CW));
	if (report_non_convex && plus_minus[1] < plus_minus[0])
		return (warn_with_code(0, WARNING_NON_CONVEX_CCW));
	if (report_non_convex)
		return (warn_with_code(0, WARNING_NON_CONVEX_Q));
	if (plus_minus[0])
		return (warn_with_code(2, WARNING_CCW));
	return (1);
}

static void		zero_ints(int *compare_dir, int *rnc, int *pm, int *i)
{
	*i = 0;
	*compare_dir = 0;
	*rnc = 0;
	pm[0] = 0;
	pm[1] = 0;
}

int				is_clockwise_convex_polygon(t_room *room, t_wall *wall, int wc)
{
	double		deflect;
	int			compare_dir;
	int			report_non_convex;
	int			plus_minus[2];
	int			i;

	zero_ints(&compare_dir, &report_non_convex, &plus_minus, &i);
	if (wc < 3)
		return (0);
	while (wc--)
	{
		if (wc != 0)
		{
			deflect = angle_abc(wall->start, wall->end, wall->next->end);
			do_checks(deflect, &i, &report_non_convex, &compare_dir);
			(compare_dir == 1) ? plus_minus[0]++ : plus_minus[1]++;
		}
		if (wc == 0)
			break ;
		wall = wall->next;
	}
	deflect = angle_abc(wall->start, wall->end, room->first_wall->end);
	do_checks(deflect, &i, &report_non_convex, &compare_dir);
	(compare_dir == 1) ? plus_minus[0]++ : plus_minus[1]++;
	return (clockwiseness_result(wall, room, report_non_convex, plus_minus));
}
