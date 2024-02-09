/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/09 02:15:48 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialise(t_pipe *data)
{
	data->status = 0;
	data->pid = ft_calloc(sizeof(pid_t), data->cmdc);
	if (!data->pid)
	{
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
	free(data->pid);
	return (WEXITSTATUS(data->status));
}

/* int	main(int argc, char *argv[], char *envp[])
{
	int		cmdc;
	char	*cmds[8];
	char	**commands;

	if (argc > 10 && argv)
		ft_printf("pipex: %d\n", argc);
	cmdc = 2;
	cmds[0] = ft_strdup("<< END cat");
	cmds[1] = ft_strdup("cat");
	pipex(cmdc, cmds, envp);
	exit(0);
}*/
