/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/16 12:00:57 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_pipe *data)
{
	data->pid = ft_calloc(sizeof(pid_t), data->cmdc);
	if (!data->pid)
	{
		free(data->history_path);
		freeall(data->envp);
		freeall_exit(data->cmds, EXIT_FAILURE);
	}
	data->pipe[0][0] = -1;
	data->pipe[0][1] = -1;
	data->pipe[1][0] = -1;
	data->pipe[1][1] = -1;
}

int	pipex(t_pipe	*data)
{
	int		i;

	initialise(data);
	i = -1;
	while (++i < data->cmdc)
		execute(i, data);
	if (data->pid[0] == 0)
	{
		free(data->pid);
		return (data->status);
	}
	i = -1;
	while (++i < data->cmdc)
		waitpid(data->pid[i], &data->status, 0);
	free(data->pid);
	return (WEXITSTATUS(data->status));
}
