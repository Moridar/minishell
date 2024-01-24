/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:03:01 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/24 13:32:48 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	here_doc(char *delimiter)
{
	int		heredoc_fd[2];
	char	*buffer;

	if (pipe(heredoc_fd) < 0)
		errormsg("pipe", 1);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer)
			errormsg("heredoc", 1);
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

static char	*cut_filename(char *str, char symbol)
{
	int		i;
	char	*filename;
	int		start;

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
	filename = interpret(ft_substr(str, start, i - start));
	ft_memset(str, ' ', i);
	return (filename);
}

static int	get_filename(char *cmd, char symbol, char **filename)
{
	int		i;
	int		type;
	int		lasttype;

	i = -1;
	lasttype = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i += get_quote_length(cmd + i, cmd[i]);
		type = count_lead_chars(cmd + i, symbol);
		if (type == 1 || type == 2)
		{
			lasttype = type;
			if (*filename)
				free(*filename);
			*filename = cut_filename(cmd + i, symbol);
		}
		if (type >= 3)
			errormsg("syntax error near unexpected token `<'", 1);
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

	type = get_filename(data->cmds[i], symbol, filename);
	if (symbol == '<' && type == 0 && i == 0)
		return (STDIN_FILENO);
	if (symbol == '<' && type == 0)
		return (data->fd[(i + 1) % 2][0]);
	if (symbol == '<' && type == 1)
		return (open(*filename, O_RDONLY));
	if (symbol == '<' && type == 2)
		return (here_doc(*filename));
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
	char	*infilename;
	char	*outfilename;

	infilename = NULL;
	outfilename = NULL;
	fd[0] = get_fd('<', i, data, &infilename);
	if (fd[0] < 0)
		errormsg(infilename, 0);
	fd[1] = get_fd('>', i, data, &outfilename);
	if (fd[1] < 0)
		errormsg("output file", 1);
	if (infilename && outfilename
		&& ft_strncmp(infilename, outfilename, ft_strlen(infilename) + 1) == 0)
	{
		write(2, "cat: -: input file is output file\n", 34);
		close(fd[1]);
		close(fd[0]);
		closepipe(data);
		exit(1);
	}
}

int	main(void)
{
	// char	*str = "he\"ll\"o_$SHELL";
	// char	*str = "$SHELL_hello";
	// char	*str = "$SHELL\"_hello\"";
	// char	*str = "$SHELL'_hello'";
	// printf("|%s|\n", interpret(str));
	char *str;
	
	str = ft_strdup("<Mak'e'file>out echo helo world > output");
	printf("|%s|\n", str);
	printf("|%s|\n", cut_filename(str, '<'));


	return (0);
}