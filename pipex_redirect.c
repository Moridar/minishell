/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:03:01 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/14 16:00:51 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*cut_filename(char *str, char symbol, t_pipe *data)
{
	int		i;
	int		start;
	char	*filename;
	char	*tmp;

	i = 0;
	while (str[i] == symbol)
		i++;
	while (ft_isspace(str[i]))
		i++;
	start = i;
	while (str[i] && str[i] != '<' && str[i] != '>' && !ft_isspace(str[i]))
	{
		if (str[i] == '\'' || str[i] == '"')
			i += get_quote_length(str + i, str[i]);
		else
			i++;
	}
	tmp = ft_substr(str, start, i - start);
	filename = interpret(tmp, data);
	free(tmp);
	ft_memset(str, ' ', i);
	return (filename);
}

static int	get_fd(char *cmd, char **filename, t_pipe *data)
{
	int		type;
	int		fd;
	char	symbol;

	symbol = cmd[0];
	fd = 0;
	type = count_lead_chars(cmd, symbol);
	if (type == 1 || type == 2)
	{
		if (*filename)
			free(*filename);
		*filename = cut_filename(cmd, symbol, data);
		fd = openfile(*filename, symbol, type, data);
	}
	if (type >= 3)
		return (-3);
	return (fd);
}

static char	*find_redirections(t_pipe *data, char *cmd, int *fd, char **file)
{
	int		i;
	int		index;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i += get_quote_length(cmd + i, cmd[i]) - 1;
		if (cmd[i] == '<' || cmd[i] == '>')
		{
			index = cmd[i] == '>';
			if (fd[index] >= 2)
				close(fd[index]);
			fd[index] = get_fd(cmd + i, &file[index], data);
			if (fd[index] == -3 && (ft_isspace(cmd[i + 2]) || !cmd[i + 2]))
				return (ft_strjoin("newline", "'"));
			if (fd[index] == -3)
				return (ft_strjoin(cmd + i + 2, "'"));
			if (fd[index] < 0)
				return (ft_strdup(file[index]));
		}
	}
	return (NULL);
}

//infile
//type 0 = pipe / std-in
//outfile
//type 0 = pipe / std-out
static void	set_basic_fd(int i, t_pipe *data, int *fd)
{
	if (i == 0)
		fd[0] = STDIN_FILENO;
	else
		fd[0] = data->fd[(i + 1) % 2][0];
	if (i == data->cmdc - 1)
		fd[1] = STDOUT_FILENO;
	else
		fd[1] = data->fd[i % 2][1];
}

//fd[0] = input (generally pipe, but can be file/heredoc/stdin)
//fd[1] = output (generally pipe, but can be file/stdout)
void	set_direction(t_pipe *data, int i, int *fd)
{
	char	*filename[2];
	char	*errorexit;

	filename[0] = NULL;
	filename[1] = NULL;
	errorexit = NULL;
	set_basic_fd(i, data, fd);
	errorexit = find_redirections(data, data->cmds[i], fd, filename);
	free_filenames(filename[0], filename[1]);
	if (errorexit)
		redirect_error_exit(errorexit, fd, data);
}
