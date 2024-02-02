/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:13:28 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/02 12:49:58 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Print error message and exits
 * @brief Msg will be formatted: 'pipex: @msg: @stderrmsg. 
 * @brief Exits if not set 0.
 * @param msg and exits
 * @param exit_status allows to pass custom exit status. If set to -1 errormsg() will
 * be equal to errno.
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

/**
 * @brief Free the double string array
 * @param **char strarray
*/
void	freeall(char **strarray)
{
	int	i;

	if (strarray)
	{
		i = -1;
		while (strarray[++i])
			free(strarray[i]);
		free(strarray);
	}
}

void	freeall_exit(char **strarray, int exitno)
{
	freeall(strarray);
	exit(exitno);
}

void	msg_freeall_exit(char *msg, char **strarray, int exitno)
{
	freeall(strarray);
	ft_putstr_fd(msg, 2);
	exit(exitno);
}
