/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:13:28 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/02 01:40:10 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	dup_and_close_fds(int fd[2])
{
	if (fd[0] != STDERR_FILENO)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	if (fd[1] != STDOUT_FILENO)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

void	closepipe(t_pipe *data)
{
	if (data->fd[0][0] >= 0)
		close(data->fd[0][0]);
	if (data->fd[0][1] >= 0)
		close(data->fd[0][1]);
	if (data->fd[1][0] >= 0)
		close(data->fd[1][0]);
	if (data->fd[1][1] >= 0)
		close(data->fd[1][1]);
}

/**
 * @brief Print error message and exits
 * @brief Msg will be formatted: 'pipex: @msg: @stderrmsg. 
 * @brief Exits if not set 0.
 * @param msg and exits
 * @param exit_status allows to pass custom exit status. If set to -1 errormsg() will
 * be equal to errno.
*/
void	errormsg(char *msg, int exits, int exit_status)
{
	if (exit_status == -1)
		exit_status = errno;
	msg = ft_strjoin("bvsh: ", msg);
	perror(msg);
	free(msg);
	if (exits)
		exit(exit_status);
}
/**
 * @brief Free the double string array
 * @param **char strarray
*/
void	freeall(char **strarray)
{
	int	i;

	if (strarray)
	{
		i = -1;
		while (strarray[++i])
			free(strarray[i]);
		free(strarray);
	}
}

void	freeall_exit(char **strarray, int exitno)
{
	freeall(strarray);
	exit(exitno);
}
