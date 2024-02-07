/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 23:28:16 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/07 16:07:29 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	here_doc(char *delimiter)
{
	int		heredoc_fd[2];
	char	*buffer;

	if (!delimiter || ft_strlen(delimiter) == 0)
	{
		ft_putstr_fd("bvsh: syntax error near unexpected token 'newline'\n", 2);
		exit(2);
	}
	if (pipe(heredoc_fd) < 0)
		errormsg("pipe", 1, -1);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer)
			break ;
		if (ft_strncmp(buffer, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		write(heredoc_fd[1], buffer, ft_strlen(buffer));
		write(heredoc_fd[1], "\n", 1);
		free(buffer);
	}
	free(buffer);
	close(heredoc_fd[1]);
	return (heredoc_fd[0]);
}

/**
 * Checks permission of the file and stop redirect if there is no permission
 * Creates file for each filename in redirect
 * 
 * @return -1 if file exist but has no read permission, 0 if file exist and
 * and can be read or was created if it did not exist before.
*/
int	openfile(char *filename, char symbol, int type)
{
	int	fd;

	fd = 0;
	//if (access(filename, F_OK) != -1 && access(filename, R_OK) == -1)
	//	return (-1);
	if (symbol == '<' && type == 1)
		fd = open(filename, O_RDONLY);
	if (symbol == '<' && type == 2)
		fd = here_doc(filename);
	if (symbol == '>' && type == 1)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (symbol == '>' && type == 2)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		errormsg(filename, 1, -1);
	return (fd);
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
