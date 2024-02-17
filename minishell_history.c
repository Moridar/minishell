/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_history.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:32:14 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/17 23:36:25 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Creates if it does not exits and writes line history to a .bvsh_history file.
 * Will not write if line is empty.
 */
int	write_history_file(char *line, t_pipe *data)
{
	int fd;

	if (data->history_path)
	{
		if (!line || line[0] == '\0')
			return (0);
		fd = open(data->history_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			free(line);
			errormsg_exit(".bvsh_history", -1, data);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		close(fd);
		return (0);
	}
	return (1);
}

/**
 * Reads history into readline prompt making it possible to use up and down
 * arrows to go through previous commands.
 */
int	read_history_file(t_pipe *data)
{
	int fd;
	char *line;

	if (data->history_path)
	{
		fd = open(data->history_path, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			errormsg_exit(".bvsh_history", -1, data);
		line = get_next_line(fd);
		while (line)
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
	return (1);
}
