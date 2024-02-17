/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/17 23:35:34 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_pipe *data)
{
	data->pid = ft_calloc(sizeof(pid_t), data->cmdc);
	if (!data->pid)
		msg_freeall_exit("malloc", NULL, 1, data);
	data->pipe[0][0] = -1;
	data->pipe[0][1] = -1;
	data->pipe[1][0] = -1;
	data->pipe[1][1] = -1;
}

int	pipex(t_pipe	*data)
{
	int		i;
	int		exit_status;

	initialise(data);
	exit_status = 0;
	i = -1;
	while (++i < data->cmdc)
	{
		execute(i, data);
		exit_status = data->status;
	}
	i = -1;
	while (++i < data->cmdc)
		waitpid(data->pid[i], &data->status, 0);
	free(data->pid);
	if (exit_status)
		return (exit_status);
	return (WEXITSTATUS(data->status));
}
