/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 12:07:17 by msuarez-          #+#    #+#             */
/*   Updated: 2019/10/18 12:07:19 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	ans;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		ans = (unsigned int)(n * -1);
	}
	else
		ans = (unsigned int)n;
	if (ans >= 10)
		ft_putnbr_fd(ans / 10, fd);
	ft_putchar_fd((char)(ans % 10 + 48), fd);
}
