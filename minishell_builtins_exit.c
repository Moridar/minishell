/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 22:04:35 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/12 15:25:08 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_memory(t_pipe *data, char **cmd)
{
	freeall(data->cmds);
	freeall(cmd);
	freeall(data->envp);
}

/**
 * @param status
*/
static int	exit_status(char *status, t_pipe *data, char **cmd)
{
	char	*status_conv;
	int		status_n;

	status_n = ft_atoi(status);
	status_conv = ft_itoa(status_n);
	if (!status_conv)
	{
		clean_memory(data, cmd);
		exit (1);
	}
	if (status[0] == '+')
		status = status + 1;
	ft_putstr_fd("exit\n", 1);
	if (ft_strncmp(status, status_conv, ft_strlen(status_conv) + 1) != 0)
	{
		free(status_conv);
		ft_putstr_fd("bvsh: exit: ", 2);
		ft_putstr_fd(status, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		clean_memory(data, cmd);
		exit (255);
	}
	free(status_conv);
	return (status_n);
}

int	exit_builtin(char **cmd, t_pipe *data, int argc)
{
	int	exitno;

	if (argc == 1)
	{
		ft_putstr_fd("exit\n", 1);
		clean_memory(data, cmd);
		exit(0);
	}
	exitno = exit_status(cmd[1], data, cmd);
	if (argc > 2)
	{
		ft_putstr_fd("bvsh: exit: too many arguments\n", 2);
		g_exit_status = 1;
		return (1);
	}
	clean_memory(data, cmd);
	exit(exitno);
	return (1);
}
