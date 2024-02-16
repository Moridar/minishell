/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 18:52:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/16 23:42:12 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_execute(t_pipe *data, char **cmd)
{
	char	*path;

	toggle_carret(1);
	dup_and_close_fds(data);
	closepipe(data);
	if (child_builtins(cmd, data) >= 0)
		clean_exit(data, cmd, EXIT_SUCCESS);
	path = check_cmdpath(cmd[0], data, cmd);
	execve(path, cmd, data->envp);
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
		free(data->pid);
		errormsg_exit("fork", -1, data);
	}
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		free(data->pid);
		free(data->history_path);
		free(data->cmds);
		data->history_path = NULL;
		data->cmds = NULL;
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
	{
		ft_putstr_fd("bvsh: malloc error\n", 2);
		clean_exit(data, NULL, EXIT_FAILURE);
	}
	if (!cmd[0])
		return (free_return_null(cmd));
	return (cmd);
}

static void	execute_pipe(int i, t_pipe *data, char ***cmd)
{
	if (pipe(data->pipe[i % 2]) == -1)
	{
		closepipe(data);
		freeall(data->cmds);
		free(data->pid);
		errormsg_exit("pipe", -1, data);
	}
	*cmd = prepare_command(data, i);
	if (*cmd)
		execute_fork(i, data, *cmd);
	if (i != 0)
		close(data->pipe[(i + 1) % 2][0]);
	close(data->pipe[i % 2][1]);
}

/**
 * Entrance to execution of the commands
 * @param index the command index
*/
void	execute(int i, t_pipe *data)
{
	char	**cmd;

	cmd = NULL;
	data->status = 0;
	if (data->cmdc == 1)
	{
		cmd = prepare_command(data, i);
		if (cmd && builtins(cmd, data) == -1)
			execute_fork(i, data, cmd);
	}
	else if (i == data->cmdc - 1)
	{
		cmd = prepare_command(data, i);
		if (cmd)
			execute_fork(i, data, cmd);
		close(data->pipe[(i + 1) % 2][0]);
	}
	else
		execute_pipe(i, data, &cmd);
	freeall(cmd);
	if (data->fd[0] > 2)
		close(data->fd[0]);
	if (data->fd[1] > 2)
		close(data->fd[1]);
}
