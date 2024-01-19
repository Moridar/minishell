/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 18:52:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/19 15:23:22 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	child_execute(int fd[2], char **args, char *path, char *envp[])
{
	dup2(fd[0], STDIN_FILENO);
	if (fd[0] != STDIN_FILENO)
		close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	if (fd[1] != STDOUT_FILENO)
		close(fd[1]);
	execve(path, args, envp);
	exit(EXIT_FAILURE);
}

static void	child_prepare(t_pipe *data, int i)
{
	char	**args;
	char	*path;
	int		fd[2];

	set_direction(data, i, fd);
	args = make_args(data->cmds[i]);
	path = ft_getpath(ft_strdup(args[0]), data->paths);
	child_execute(fd, args, path, data->envp);
}

static void	execute_fork(int i, t_pipe *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		errormsg("fork", 1);
	if (pid == 0)
	{
		child_prepare(data, i);
		exit(EXIT_FAILURE);
	}
	data->pid[i] = pid;
}

static void	execute_pipe(int i, t_pipe *data)
{
	if (pipe(data->fd[i % 2]) == -1)
		errormsg("pipe", 1);
	if (i == 0)
		execute_fork(i, data);
	else
	{
		data->fd[(i + 1) % 2][1] = data->fd[i % 2][1];
		execute_fork(i, data);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i - 1], &data->status, 0);
	}
	close(data->fd[(i + 1) % 2][1]);
}

//Single command and the last command doesnt need pipe
void	execute(int i, t_pipe *data)
{
	if (data->cmdc == 1)
	{
		execute_fork(i, data);
		waitpid(data->pid[i], &data->status, 0);
	}
	else if (i == data->cmdc - 1)
	{
		execute_fork(i, data);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i - 1], &data->status, 0);
		waitpid(data->pid[i], &data->status, 0);
	}
	else
		execute_pipe(i, data);
}
