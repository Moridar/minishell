/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:13:28 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/15 12:29:16 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_pipe *data, char **cmd, int exitno)
{
	free(data->history_path);
	freeall(data->cmds);
	freeall(data->envp);
	freeall(cmd);
	exit(exitno);
}

/**
 * @brief Print error message and exits
 * @brief Msg will be formatted: 'bvsh: @msg: @stderrmsg. 
 * @brief Exits if not set 0.
 * @param msg and exits
 * @param exit_status If set to -1 will get the errno.
*/
void	errormsg_exit(char *msg, int exit_status, t_pipe *data)
{
	if (exit_status == -1)
		exit_status = errno;
	ft_putstr_fd("bvsh: ", 2);
	perror(msg);
	clean_exit(data, NULL, exit_status);
}

void	msg_freeall_exit(char *msg, char **strarray, int exitno, t_pipe *data)
{
	ft_putstr_fd(msg, 2);
	clean_exit(data, strarray, exitno);
}
