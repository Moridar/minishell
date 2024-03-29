/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:13:28 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/20 10:58:51 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_pipe *data, char **arraylist, int exitno)
{
	closepipe(data);
	if (data->fd[0] > 2)
		close(data->fd[0]);
	if (data->fd[1] > 2)
		close(data->fd[1]);
	free(data->pid);
	free(data->history_path);
	freeall(data->cmds);
	freeall(data->envp);
	freeall(arraylist);
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

void	msg_freeall_exit(char *msg, char **arraylist, int exitno, t_pipe *data)
{
	ft_putstr_fd("bvsh: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	clean_exit(data, arraylist, exitno);
}
