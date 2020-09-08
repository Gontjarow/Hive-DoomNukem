/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 11:49:21 by msuarez-          #+#    #+#             */
/*   Updated: 2019/11/11 12:28:02 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** We move forward on the string until we find a newline or a null-terminator.
** If we found a newline, we get a substring of the actual line and assign
** it to pointer line. Then we make a duplicate of it to tmp and free our
** string to avoid leaks and assign what we duplicated back to our string.
** Else, if we found a null-terminator and our ret value is the same as our
** BUFF_SIZE, we call our get_next_line function back. Otherwise we just make
** a duplicate of our string to line and free/set it to null.
*/

static int			ft_new_line(char **s, char **line, int fd, int ret)
{
	char	*tmp;
	int		len;

	len = 0;
	while (s[fd][len] != '\n' && s[fd][len] != '\0')
		len++;
	if (s[fd][len] == '\n')
	{
		*line = ft_strsub(s[fd], 0, len);
		tmp = ft_strdup(s[fd] + len + 1);
		free(s[fd]);
		s[fd] = tmp;
		if (s[fd][0] == '\0')
			ft_strdel(&s[fd]);
	}
	else if (s[fd][len] == '\0')
	{
		if (ret == BUFF_SIZE)
			return (get_next_line(fd, line));
		*line = ft_strdup(s[fd]);
		ft_strdel(&s[fd]);
	}
	return (1);
}

/*
** First we check for errors such as invalid file descriptor or empty/NULL
** line, if true we return (-1). Then we have a while loop that will read
** whatever content we have on our buff, with a valid given fd and our
** BUFF_SIZE set at our macro. This will happen while our ret value is not
** 0 (ended reading) or -1 (error).
** If our string is NULL we need to allocate memory for it in order to use
** it. Then we join whatever content we have on the buff to our s string
** and assign it to a temporary char pointer. We free s[fd] to avoid
** memory leakage and assign our join back to s[fd]. If we found a newline
** in our buff, we break the loop and call our cropping function
** ft_new_line to handle this first line that we were dealing with.
** We keep doing that until our whole file was read.
*/

int					get_next_line(const int fd, char **line)
{
	static char	*s[FD_MAX];
	char		buf[BUFF_SIZE + 1];
	char		*tmp;
	int			ret;

	if ((fd < 0 || fd >= FD_MAX) || line == NULL || BUFF_SIZE < 1)
		return (-1);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if (s[fd] == NULL)
			s[fd] = ft_strnew(1);
		tmp = ft_strjoin(s[fd], buf);
		free(s[fd]);
		s[fd] = tmp;
		if (ft_strchr(buf, '\n'))
			break ;
	}
	if (ret < 0)
		return (-1);
	else if (ret == 0 && (s[fd] == NULL || s[fd][0] == '\0'))
		return (0);
	return (ft_new_line(s, line, fd, ret));
}
