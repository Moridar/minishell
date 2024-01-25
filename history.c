/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:32:14 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/25 19:14:27 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * Creates if it does not exits and writes line history to a .bvsh_history file.
 * Will not write if line is empty.
*/
int	write_history_file(char *line)
{
	int fd;

	if (!line || line[0] == '\0')
		return (0);
	fd = open(".bvsh_history", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		errormsg(".bvsh_history", 1);
		return (1);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	close(fd);
	return (0);
}

/**
 * Reads history into readline prompt making it possible to use up and down
 * arrows to go through previous commands.
*/
int	read_history_file(void)
{
	int		fd;
	char	*line;

	fd = open(".bvsh_history", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		errormsg(".bvsh_history", 1);
		return (1);
	}
	line = get_next_line(fd);
	while(line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		add_history(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

/**
 * Returns all the history commands with lines numbered
*/
int	history(void)
{
	int		fd;
	char	*line;
	char	*res;
	int		i;

	fd = open(".bvsh_history", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		errormsg(".bvsh_history", 1);
		return (1);
	}
	res = ft_strdup("");
	line = get_next_line(fd);
	i = 0;
	while(line)
	{
		i++;
		printf("%5d  %s", i, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}