/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 11:00:35 by msuarez-          #+#    #+#             */
/*   Updated: 2019/10/17 11:00:37 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*p1;
	unsigned char		*p2;

	p1 = (unsigned char*)dst;
	p2 = (unsigned char*)src;
	if (dst == src)
		return (dst);
	while (n)
	{
		*(p1++) = *(p2++);
		n--;
	}
	return (dst);
}
