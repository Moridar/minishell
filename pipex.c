/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/14 19:45:51 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_pipe *data)
{
	data->status = 0;
	data->pid = ft_calloc(sizeof(pid_t), data->cmdc);
	if (!data->pid)
	{
		free(data->history_path);
		freeall(data->envp);
		freeall_exit(data->cmds, EXIT_FAILURE);
	}
	data->fd[0][0] = -1;
	data->fd[0][1] = -1;
	data->fd[1][0] = -1;
	data->fd[1][1] = -1;
}

int	pipex(t_pipe	*data)
{
	int		i;

	initialise(data);
	i = -1;
	while (++i < data->cmdc)
		execute(i, data);
	i = -1;
	while (++i < data->cmdc)
		waitpid(data->pid[i], &data->status, 0);
	free(data->pid);
	return (WEXITSTATUS(data->status));
}
