/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 14:44:33 by msuarez-          #+#    #+#             */
/*   Updated: 2019/10/16 14:44:34 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *needle, size_t len)
{
	unsigned int pos;
	unsigned int i;

	if (!*needle)
		return ((char*)str);
	pos = 0;
	while (str[pos] != '\0' && (size_t)pos < len)
	{
		if (str[pos] == needle[0])
		{
			i = 1;
			while (needle[i] != '\0' && str[pos + i] == needle[i] &&
					(size_t)(pos + i) < len)
				i++;
			if (needle[i] == '\0')
				return ((char*)&str[pos]);
		}
		pos++;
	}
	return (0);
}
