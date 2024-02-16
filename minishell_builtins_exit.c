/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 22:04:35 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/16 11:54:34 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ft_putstr_fd("bvsh: malloc error\n", 2);
		clean_exit(data, cmd, 1);
	}
	if (status[0] == '+')
		status = status + 1;
	ft_putstr_fd("exit\n", data->fd[1]);
	if (ft_strncmp(status, status_conv, ft_strlen(status_conv) + 1) != 0)
	{
		free(status_conv);
		ft_putstr_fd("bvsh: exit: ", 2);
		ft_putstr_fd(status, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		clean_exit(data, cmd, 255);
	}
	free(status_conv);
	return (status_n);
}

int	exit_builtin(char **cmd, t_pipe *data, int argc)
{
	int	exitno;

	if (argc == 1)
	{
		ft_putstr_fd("exit\n", data->fd[1]);
		clean_exit(data, cmd, data->exit_status);
	}
	exitno = exit_status(cmd[1], data, cmd);
	if (argc > 2)
	{
		ft_putstr_fd("bvsh: exit: too many arguments\n", 2);
		return (1);
	}
	clean_exit(data, cmd, exitno);
	return (0);
}

int	cd(t_pipe *data, char **cmd, int count)
{
	char	*key;
	char	*path;

	if (count < 2)
		ft_putstr_fd("bvsh: cd: too few arguments\n", 2);
	else if (chdir(cmd[1]) == 0)
	{
		path = getcwd(NULL, 0);
		if (!path)
			return (-2);
		key = ft_strjoin("PWD=", path);
		free(path);
		if (!key)
			return (-2);
		return (export(data, key));
	}
	else
	{
		ft_putstr_fd("bvsh: cd: ", 2);
		perror(cmd[1]);
	}
	return (1);
}
