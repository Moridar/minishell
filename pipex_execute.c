/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 18:52:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/12 20:55:59 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (!cmd)
	{
		ft_putstr_fd("bvsh: malloc error\n", 2);
		freeall_exit(data->envp, EXIT_FAILURE);
	}
	if (!*cmd[0] || child_builtins(cmd, data))
	{
		freeall(data->envp);
		freeall_exit(cmd, EXIT_SUCCESS);
	}
	path = check_cmdpath(cmd[0], data, cmd);
	execve(path, cmd, data->envp);
	freeall(cmd);
	free(path);
}

static void	execute_fork(int i, t_pipe *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		closepipe(data);
		freeall(data->cmds);
		free(data->pid);
		errormsg_exit("fork", -1, data);
	}
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
	{
		closepipe(data);
		freeall(data->cmds);
		free(data->pid);
		errormsg_exit("pipe", -1, data);
	}
	execute_fork(i, data);
	if (i != 0)
		close(data->fd[(i + 1) % 2][0]);
	close(data->fd[i % 2][1]);
}

/**
 * Entrance to execution of the commands
 * @param index the command index
*/
void	execute(int i, t_pipe *data)
{
	if (data->cmdc == 1)
		execute_fork(i, data);
	else if (i == data->cmdc - 1)
	{
		execute_fork(i, data);
		close(data->fd[(i + 1) % 2][0]);
	}
	else
		execute_pipe(i, data);
}
