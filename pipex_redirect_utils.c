/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 23:28:16 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/06 16:20:01 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks permission of the file and stop redirect if there is no permission
 * Creates file for each filename in redirect
 * 
 * @return -1 if file exist but has no read permission, 0 if file exist and
 * and can be read or was created if it did not exist before.
*/
static int	check_file_perm_exist(char *filename)
{
	int		fd;

	if (access(filename, F_OK) != -1 && access(filename, R_OK) == -1)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		errormsg(filename, 1, -1);
	close(fd);
	return (0);
}

int	handle_file(char *cmd, char symbol, char **filename, t_pipe *data)
{
	if (*filename)
		free(*filename);
	*filename = cut_filename(cmd, symbol, data);
	if (symbol == '>' && check_file_perm_exist(*filename) == -1)
		return (-1);
	return (0);
}

void	free_filenames(char *infilename, char *outfilename)
{
	if (infilename)
		free(infilename);
	if (outfilename)
		free(outfilename);
}

void	redirect_check_error(char *errmsg, int *fd, t_pipe *data)
{
	if (errmsg)
	{
		ft_putstr_fd("bvsh: ", 2);
		ft_putstr_fd(errmsg, 2);
		free(errmsg);
		if (errno)
		{
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(strerror(errno), 2);
		}
		ft_putstr_fd("\n", 2);
		if (fd[1] >= 2)
			close(fd[1]);
		if (fd[0] >= 2)
			close(fd[0]);
		closepipe(data);
		freeall(data->envp);
		freeall(data->cmds);
		exit(1);
	}
}
