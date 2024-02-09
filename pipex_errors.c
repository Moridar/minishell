/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:13:28 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/09 09:42:27 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print error message and exits
 * @brief Msg will be formatted: 'pipex: @msg: @stderrmsg. 
 * @brief Exits if not set 0.
 * @param msg and exits
 * @param exit_status If set to -1 will get the errno.
*/
void	errormsg(char *msg, int exits, int exit_status)
{
	if (exit_status == -1)
		exit_status = errno;
	msg = ft_strjoin("bvsh: ", msg);
	perror(msg);
	free(msg);
	if (exits)
		exit(exit_status);
}

void	msg_freeall_exit(char *msg, char **strarray, int exitno)
{
	freeall(strarray);
	ft_putstr_fd(msg, 2);
	exit(exitno);
}

void	*free_return_null(void *ptr)
{
	free(ptr);
	return (NULL);
}
