/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:13:28 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/12 15:33:54 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print error message and exits
 * @brief Msg will be formatted: 'bvsh: @msg: @stderrmsg. 
 * @brief Exits if not set 0.
 * @param msg and exits
 * @param exit_status If set to -1 will get the errno.
*/
void	errormsg_exit(char *msg, int exit_status, t_pipe *data)
{
	freeall(data->envp);
	if (exit_status == -1)
		exit_status = errno;
	ft_putstr_fd("bvsh: ", 2);
	perror(msg);
	exit(exit_status);
}

void	msg_freeall_exit(char *msg, char **strarray, int exitno)
{
	freeall(strarray);
	ft_putstr_fd(msg, 2);
	exit(exitno);
}
