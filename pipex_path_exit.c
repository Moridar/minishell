/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:27:38 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/14 18:16:49 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_no_permission_exit(char *path, char **cmdline, t_pipe *data)
{
	ft_putstr_fd("bvsh: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	if (path != cmdline[0])
		free(path);
	freeall(cmdline);
	freeall(data->envp);
	exit(126);
}

void	cmdnfound_exit(char **cmdline, t_pipe *data)
{
	ft_putstr_fd("bvsh: ", 2);
	ft_putstr_fd(cmdline[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	freeall(cmdline);
	freeall(data->envp);
	exit(127);
}
