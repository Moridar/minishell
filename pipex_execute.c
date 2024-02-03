/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 18:52:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/03 21:48:26 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	interrupt(int sig)
{
	g_exit_status = 130;
	if (sig == SIGINT)
		exit(130);
}

static void	child_execute(t_pipe *data, int i)
{
	char	**cmd;
	char	*path;
	int		fd[2];

	toggle_carret(1);
	set_direction(data, i, fd);
	cmd = split_shell_cmd(data->cmds[i], data);
	freeall(data->cmds);
	dup_and_close_fds(fd);
	closepipe(data);
	if (child_builtins(cmd, data))
		freeall_exit(cmd, 0);
	path = check_cmdpath(cmd[0], data, cmd);
	execve(path, cmd, data->envp);
}

static void	execute_fork(int i, t_pipe *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		errormsg("fork", 1, -1);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		free(data->pid);
		signal(SIGINT, interrupt);
		child_execute(data, i);
		freeall(data->envp);
		exit(EXIT_FAILURE);
	}
	data->pid[i] = pid;
}

static void	execute_pipe(int i, t_pipe *data)
{
	if (pipe(data->fd[i % 2]) == -1)
		errormsg("pipe", 1, -1);
	if (i == 0)
		execute_fork(i, data);
	else
	{
		execute_fork(i, data);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i - 1], &data->status, 0);
	}
	close(data->fd[i % 2][1]);
}

/**
 * Entrance to execution of the commands
 * @param index the command index
*/
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
		waitpid(data->pid[i - 1], &data->status, 0);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i], &data->status, 0);
	}
	else
		execute_pipe(i, data);
}
