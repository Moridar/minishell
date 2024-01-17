/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 18:52:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/17 09:23:14 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	errormsg(char *msg, int exits)
{
	msg = ft_strjoin("pipex: ", msg);
	perror(msg);
	free(msg);
	if (exits)
		exit(errno);
}

static void	exec(int i, t_pipe *data, int fd[])
{
	char	**args;
	char	*path;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		errormsg("fork", 1);
	if (pid == 0)
	{
		args = make_args(data->cmds[i]);
		path = ft_getpath(ft_strdup(args[0]), data->paths);
		if (i == 0 || i == data->cmdc - 1)
			close(fd[2]);
		dup2(fd[0], STDIN_FILENO);
		if (fd[0] != STDIN_FILENO)
			close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (fd[1] != STDOUT_FILENO)
			close(fd[1]);
		execve(path, args, data->envp);
		exit(EXIT_FAILURE);
	}
	data->pid[i] = pid;
	if ((i == 0 && data->infile) || (data->cmdc - i == 1 && data->outfile))
		close(fd[3]);
}

static void	firstexec(t_pipe *data)
{
	int		file;
	int		newfd[4];

	if (data->infile)
	{
		file = open(data->infile, O_RDONLY);
		if (file < 0)
			return (errormsg(data->infile, 0));
		newfd[0] = file;
		newfd[3] = file;
	}
	else
	{
		newfd[0] = STDIN_FILENO;
		newfd[3] = STDIN_FILENO;
	}
	newfd[1] = data->fd[0][1];
	newfd[2] = data->fd[0][0];
	exec(0, data, newfd);
}

static void	lastexec(int i, t_pipe *data)
{
	int		file;
	int		newfd[4];

	if (data->outfile)
	{
		file = open(data->outfile, O_WRONLY | O_TRUNC | O_CREAT, 00644);
		if (file < 0)
			errormsg(data->outfile, 1);
		newfd[1] = file;
		newfd[3] = file;
	}
	else
	{
		newfd[1] = STDOUT_FILENO;
		newfd[3] = STDOUT_FILENO;
	}
	newfd[0] = data->fd[(i + 1) % 2][0];
	newfd[2] = data->fd[(i + 1) % 2][1];
	exec(i, data, newfd);
}

//i = 2 first
//i == data->argc -2 is last
//else between
void	execute(int i, t_pipe *data)
{
	if (i == 0)
	{
		if (pipe(data->fd[0]) == -1)
			errormsg("pipe", 1);
		firstexec(data);
	}
	else if (i == data->cmdc - 1)
	{
		lastexec(i, data);
		close(data->fd[(i + 1) % 2][1]);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i - 1], &data->status, 0);
		waitpid(data->pid[i], &data->status, 0);
	}
	else
	{
		if (pipe(data->fd[i % 2]) == -1)
			errormsg("pipe", 1);
		close(data->fd[(i + 1) % 2][1]);
		data->fd[(i + 1) % 2][1] = data->fd[i % 2][1];
		exec(i, data, data->fd[(i + 1) % 2]);
		close(data->fd[(i + 1) % 2][0]);
		waitpid(data->pid[i - 1], &data->status, 0);
	}
}
