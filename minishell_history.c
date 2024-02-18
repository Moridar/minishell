/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_history.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:32:14 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/19 01:47:13 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	disable_history(t_pipe *data)
{
	ft_putstr_fd("bvsh: ", 2);
	perror(data->history_path);
	if (data->history_path)
	{
		free(data->history_path);
		data->history_path = NULL;
	}
	return (1);
}

/**
 * Creates if it does not exits and writes line history to a .bvsh_history file.
 * Will not write if line is empty.
 */
int	write_history_file(char *line, t_pipe *data)
{
	int	fd;

	if (data->history_path)
	{
		if (!line || line[0] == '\0')
			return (0);
		fd = open(data->history_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (disable_history(data));
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
	int		fd;
	char	*line;

	if (data->history_path)
	{
		fd = open(data->history_path, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (disable_history(data));
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
