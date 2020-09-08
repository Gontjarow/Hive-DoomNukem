/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 11:39:32 by msuarez-          #+#    #+#             */
/*   Updated: 2019/10/17 11:39:33 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char *p1;
	unsigned char *p2;

	p1 = (unsigned char*)dst;
	p2 = (unsigned char*)src;
	if (!dst && !src)
		return (NULL);
	if (p1 < p2)
		return (ft_memcpy(dst, src, len));
	else
		while (len-- > 0)
			p1[len] = p2[len];
	return (dst);
}
