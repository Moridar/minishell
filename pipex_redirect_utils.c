/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 23:28:16 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/18 00:33:53 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_filenames(char *infilename, char *outfilename)
{
	if (infilename)
		free(infilename);
	if (outfilename)
		free(outfilename);
}

void	redirect_error_exit(char *errmsg, int *fd, t_pipe *data)
{
	data->status = 1;
	ft_putstr_fd("bvsh: ", 2);
	if (fd[0] == -3 || fd[1] == -3)
		ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(errmsg, 2);
	free(errmsg);
	if (fd[1] == -2 || fd[2] == -2)
		ft_putstr_fd(": malloc error", 2);
	else if ((fd[1] != -3 && fd[0] != -3) && errno)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
	}
	ft_putstr_fd("\n", 2);
	if (fd[1] >= 2)
		close(fd[1]);
	if (fd[0] >= 2)
		close(fd[0]);
	if (fd[0] == -3 || fd[1] == -3)
		data->status = 2;
}

static int	buffer_interpret_pipe(char *buffer, int *heredoc_fd, t_pipe *data)
{
	char	*interpreted_str;

	while (*buffer)
	{
		interpreted_str = interpret(buffer, data);
		if (!interpreted_str)
		{
			close(heredoc_fd[1]);
			close(heredoc_fd[0]);
			return (-2);
		}
		write(heredoc_fd[1], interpreted_str, ft_strlen(interpreted_str));
		free(interpreted_str);
		buffer += len_next_meta_char(buffer, "", 1);
		while (ft_isspace(*buffer))
			write(heredoc_fd[1], buffer++, 1);
	}
	write(heredoc_fd[1], "\n", 1);
	return (0);
}

static int	here_doc(char *delimiter, t_pipe *data)
{
	int		heredoc_fd[2];
	char	*buffer;

	if (!delimiter || ft_strlen(delimiter) == 0)
		return (-3);
	if (pipe(heredoc_fd) < 0)
		return (-1);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer)
			break ;
		if (ft_strncmp(buffer, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		if (buffer_interpret_pipe(buffer, heredoc_fd, data) == -2)
			return (free_return (buffer, -2));
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
int	openfile(char *filename, char symbol, int type, t_pipe *data)
{
	int	fd;

	if (!filename)
		return (-2);
	fd = 0;
	if (symbol == '<' && type == 1)
	{
		if (access(filename, R_OK) == -1)
			return (-1);
		fd = open(filename, O_RDONLY);
	}
	else if (symbol == '<' && type == 2)
		fd = here_doc(filename, data);
	else if (symbol == '>')
	{
		if (access(filename, F_OK) == 0 && access(filename, W_OK) == -1)
			return (-1);
		if (type == 1)
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (type == 2)
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	return (fd);
}
