/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:03:01 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/30 15:20:40 by vshchuki         ###   ########.fr       */
/*																			  */
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

char	*cut_filename(char *str, char symbol, t_pipe *data)
{
	int		i;
	int		start;
	char	*filename;
	char	*tmp;

	filename = NULL;
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

static int	get_filename(char *cmd, char symbol, char **filename, t_pipe *data)
{
	int		i;
	int		type;
	int		lasttype;

	i = -1;
	lasttype = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i += get_quote_length(cmd + i, cmd[i]) - 1;
		type = count_lead_chars(cmd + i, symbol);
		if (type == 1 || type == 2)
		{
			if (lasttype > 2)
				close(lasttype);
			lasttype = type;
			if (handle_file(cmd + i, symbol, filename, data) == -1)
				break ;
			if (symbol == '<' && type == 2)
				lasttype = here_doc(*filename);
		}
		if (type >= 3)
			errormsg("syntax error near unexpected token `<'", 1, -1);
	}
	return (lasttype);
}

//infile
//type 0 = pipe / std-in
//type 1 = < = file
//type 2 = << = here_doc
//outfile
//type 0 = pipe / std-out
//type 1 = > = file  
//type 2 = >> = append
static int	get_fd(char symbol, int i, t_pipe *data, char **filename)
{
	int		type;

	type = get_filename(data->cmds[i], symbol, filename, data);
	if (symbol == '<' && type == 0 && i == 0)
		return (STDIN_FILENO);
	if (symbol == '<' && type == 0)
		return (data->fd[(i + 1) % 2][0]);
	if (symbol == '<' && type == 1)
		return (open(*filename, O_RDONLY));
	if (symbol == '<' && type > 2)
		return (type);
	if (symbol == '>' && type == 0 && i == data->cmdc - 1)
		return (STDOUT_FILENO);
	if (symbol == '>' && type == 0)
		return (data->fd[i % 2][1]);
	if (symbol == '>' && type == 1)
		return (open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (symbol == '>' && type == 2)
		return (open(*filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

//fd[0] = input (generally pipe, but can be file/heredoc)
//fd[1] = output (generally pipe, but can be file/stdout)
void	set_direction(t_pipe *data, int i, int *fd)
{
	char	*filename[2];
	char	*errorexit;

	filename[0] = NULL;
	filename[1] = NULL;
	errorexit = NULL;
	fd[0] = get_fd('<', i, data, &(filename[0]));
	if (fd[0] < 0)
		errorexit = ft_strdup(filename[0]);
	if (errorexit == NULL)
		fd[1] = get_fd('>', i, data, &filename[1]);
	if (fd[1] < 0)
		errorexit = ft_strdup(filename[1]);
	if (!errorexit && filename[0] && filename[1] && ft_strncmp(filename[0],
			filename[1], ft_strlen(filename[0]) + 1) == 0)
		errorexit = ft_strdup("cat: -: input file is output file");
	free_filenames(filename[0], filename[1]);
	redirect_check_error(errorexit, fd, data);
}
