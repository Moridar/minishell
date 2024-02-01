/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 19:13:28 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/01 19:14:07 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	closepipe(t_pipe *data)
{
	if (data->fd[0][0] >= 0)
		close(data->fd[0][0]);
	if (data->fd[0][1] >= 0)
		close(data->fd[0][1]);
	if (data->fd[1][0] >= 0)
		close(data->fd[1][0]);
	if (data->fd[1][1] >= 0)
		close(data->fd[1][1]);
}

/**
 * @brief Print error message and exits
 * @brief Msg will be formatted: 'pipex: @msg: @stderrmsg. 
 * @brief Exits if not set 0.
 * @param msg and exits
*/
void	errormsg(char *msg, int exits)
{
	msg = ft_strjoin("bvsh: ", msg);
	perror(msg);
	free(msg);
	if (exits)
		exit(errno);
}
