/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 22:04:35 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/02 00:49:26 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	// ft_putstr_fd("exit\n", 2);
	ft_putstr_fd(error_msg, 2);
	free(temp);
	free(error_msg);
	return (1);
}

/**
 * @param status
*/
int	exit_builtin(char *status, t_pipe *data, char **cmd, char *line)
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
		// ft_putstr_fd("exit\n", 1);
		clean_memory(data, cmd, line);
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
