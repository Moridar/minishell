/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/23 13:45:33 by bsyvasal         ###   ########.fr       */
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
		exit_status = execute(i, data);
	i = -1;
	while (++i < data->cmdc)
		waitpid(data->pid[i], &data->status, 0);
	free(data->pid);
	if (exit_status == 4)
		exit_status = 2;
	if (exit_status)
		return (exit_status);
	if (WEXITSTATUS(data->status) != 0)
		return (WEXITSTATUS(data->status));
	if (data->status == 2)
		return (printf("^C\n") + 127);
	if (data->status == 3)
		return (printf("^\\Quit: 3\n") + 121);
	return (0);
}
