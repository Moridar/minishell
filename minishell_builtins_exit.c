/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 22:04:35 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/07 13:36:18 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_memory(t_pipe *data, char **cmd, char *line)
{
	freeall(data->cmds);
	freeall(cmd);
	freeall(data->envp);
	free(line);
}

static int	handle_wrong_arg(char *status)
{
	char	*error_msg;
	char	*temp;

	error_msg = ft_strjoin("bvsh: exit: ", status);
	if (!error_msg)
		return (2);
	temp = error_msg;
	error_msg = ft_strjoin(temp, ": numeric argument required\n");
	if (!error_msg)
		return (2);
	ft_putstr_fd("exit\n", 1);
	ft_putstr_fd(error_msg, 2);
	free(temp);
	free(error_msg);
	return (1);
}

/**
 * @param status
*/
static int	exit_status(char *status, t_pipe *data, char **cmd, char *line)
{
	char	*status_conv;
	int		status_n;

	status_n = ft_atoi(status);
	status_conv = ft_itoa(status_n);
	if (!status_conv)
		return (2);
	if (status[0] == '+')
		status = status + 1;
	if (ft_strncmp(status, status_conv, ft_strlen(status_conv) + 1) == 0)
	{
		free(status_conv);
		clean_memory(data, cmd, line);
		ft_putstr_fd("exit\n", 1);
		exit((char)status_n);
	}
	else
	{
		free(status_conv);
		handle_wrong_arg(status);
		clean_memory(data, cmd, line);
		exit(255);
	}
	return (1);
}

int	exit_builtin(char **cmd, t_pipe *data, char *line, int argc)
{
	if (argc == 1)
	{
		ft_putstr_fd("exit\n", 1);
		clean_memory(data, cmd, line);
		exit(0);
	}
	if (argc == 2)
		return (exit_status(cmd[1], data, cmd, line));
	if (argc > 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("bvsh: exit: too many arguments\n", 2);
		g_exit_status = 1;
		return (1);
	}
	return (1);
}
