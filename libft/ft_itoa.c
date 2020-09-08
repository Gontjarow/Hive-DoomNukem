/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 17:27:32 by msuarez-          #+#    #+#             */
/*   Updated: 2019/10/28 17:27:32 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int		nb_size(int nb)
{
	int		count;
	long	n;

	n = nb;
	count = 1;
	if (n < 0)
	{
		n *= -1;
		count++;
	}
	while (n > 9)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char			*ft_itoa(int n)
{
	char	*new;
	int		i;
	long	nb;

	nb = n;
	i = nb_size(nb);
	if (!(new = (char*)malloc(sizeof(char) * i + 1)))
		return (NULL);
	new[i--] = '\0';
	if (nb < 0)
	{
		new[0] = '-';
		nb *= -1;
	}
	if (nb == 0)
	{
		new[0] = '0';
		return (new);
	}
	while (nb > 0)
	{
		new[i--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (new);
}
