/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 18:52:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/24 02:00:22 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_execute(t_pipe *data, char **cmd)
{
	char	*path;
	int		exit_status;

	dup_and_close_fds(data);
	closepipe(data);
	exit_status = child_builtins(cmd, data);
	if (exit_status >= 0)
		clean_exit(data, cmd, exit_status);
	path = check_cmdpath(cmd[0], data, cmd);
	execve(path, cmd, data->envp);
	printf("interrupted\n");
	if (path != cmd[0])
		free(path);
	clean_exit(data, cmd, EXIT_FAILURE);
}

static void	execute_fork(int i, t_pipe *data, char **cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		freeall(cmd);
		closepipe(data);
		errormsg_exit("fork", -1, data);
	}
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		free(data->pid);
		free(data->history_path);
		freeall(data->cmds);
		data->history_path = NULL;
		data->cmds = NULL;
		data->pid = NULL;
		signal(SIGINT, exit);
		child_execute(data, cmd);
	}
	data->pid[i] = pid;
}

static char	**prepare_command(t_pipe *data, int i)
{
	char	**cmd;

	cmd = NULL;
	set_direction(data, i, data->fd);
	if (data->status != 0)
		return (NULL);
	cmd = split_shell_cmd(data->cmds[i], data);
	if (!cmd)
		msg_freeall_exit("malloc error", NULL, 1, data);
	if (!cmd[0])
		return (free_return_null(cmd));
	if (!cmd[0][0])
	{
		ft_putstr_fd("bvsh: : command not found\n", 2);
		freeall(cmd);
		data->status = 127;
		return (NULL);
	}
	return (cmd);
}

/**
 * Entrance to execution of the commands
 * @param index the command index
*/
int	execute(int i, t_pipe *data)
{
	char	**cmd;

	data->status = 0;
	if (data->cmdc != 1 && i != data->cmdc - 1 && pipe(data->pipe[i % 2]) == -1)
		errormsg_exit("pipe", -1, data);
	cmd = prepare_command(data, i);
	if (data->cmdc == 1 && cmd && builtins(cmd, data) == 0)
		;
	else if (cmd)
		execute_fork(i, data, cmd);
	freeall(cmd);
	if (data->fd[0] > 2)
		close(data->fd[0]);
	if (data->fd[1] > 2)
		close(data->fd[1]);
	data->fd[0] = -1;
	data->fd[1] = -1;
	return (data->status);
}
