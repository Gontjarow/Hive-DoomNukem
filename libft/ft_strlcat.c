/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:29:03 by msuarez-          #+#    #+#             */
/*   Updated: 2019/10/16 15:29:04 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t i;
	size_t p1;
	size_t p2;

	i = 0;
	p1 = ft_strlen(dst);
	p2 = ft_strlen(src);
	if (dstsize <= p1)
		return (p2 + dstsize);
	while (src[i] && (p1 + i) < dstsize - 1)
	{
		dst[p1 + i] = src[i];
		i++;
	}
	dst[p1 + i] = '\0';
	return (p1 + p2);
}
