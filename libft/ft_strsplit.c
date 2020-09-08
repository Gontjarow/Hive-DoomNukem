/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 10:53:11 by msuarez-          #+#    #+#             */
/*   Updated: 2019/10/21 10:53:11 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int		ft_count_words(char const *str, char c)
{
	int		i;
	int		words;

	i = 0;
	words = 0;
	while (str[i] != '\0')
	{
		while (!(str[i] == c))
			if (str[i] != '\0')
				i++;
			else
				break ;
		while (str[i] == c)
			i++;
		words++;
	}
	return (words);
}

static char		*ft_find_words(char const *str, char c, int *index)
{
	int		i;
	int		start;
	int		end;
	char	*dest;

	start = *index;
	i = start;
	while (!(str[i] == c))
		if (str[i] != '\0')
			i++;
		else
			break ;
	end = i;
	dest = (char*)malloc(end - start + 1);
	while (str[i] == c)
		i++;
	*index = i;
	i = 0;
	while (i < end - start)
	{
		dest[i] = str[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char			**ft_strsplit(char const *s, char c)
{
	int		i;
	int		index;
	char	**dest;

	i = 0;
	index = 0;
	if (!s || !c)
		return (NULL);
	if (!(dest = (char**)malloc(sizeof(char*) * (ft_count_words(s, c) + 1))))
		return (NULL);
	while (*s == c)
		s++;
	while (s[i] != '\0')
	{
		dest[index] = ft_find_words(s, c, &i);
		index++;
	}
	dest[index] = 0;
	return (dest);
}
