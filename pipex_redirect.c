/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:03:01 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/21 02:03:44 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	get_num_of_repeats(char *str, char symbol)
{
	int	i;

	i = 0;
	while (str[i] == symbol)
	{
		i++;
	}
	return (i);
}

static char	*cut_filename(char *str, char symbol)
{
	int		i;
	char	*filename;

	filename = NULL;
	i = 0;
	while (str[i] == symbol)
		i++;
	while (ft_isspace(str[i]))
		i++;
	filename = parse_quotes(str + i, symbol, 1);
	ft_memset(str, ' ', i);
	return (filename);
}

static int	get_filename(char *cmd, char symbol, char **filename)
{
	int		i;
	int		type;
	int		lasttype;

	*filename = NULL;
	i = -1;
	lasttype = 0;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i += get_quote_length(cmd + i, cmd[i]);
		type = get_num_of_repeats(cmd + i, symbol);
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
//type 1 = file
//type 2 = here_doc
//outfile
//type 0 = pipe / std-out
//type 1 = file
//type 2 = append
static int	get_fd(char *cmd, char symbol, int i, t_pipe *data)
{
	char	*filename;
	int		type;

	type = get_filename(cmd, symbol, &filename);
	if (symbol == '<' && type == 0 && i == 0)
		return (STDIN_FILENO);
	if (symbol == '<' && type == 0)
		return (data->fd[(i + 1) % 2][0]);
	if (symbol == '<' && type == 1)
		return (open(filename, O_RDONLY));
	if (symbol == '<' && type == 2)
	{
		errormsg("here_doc", 0);
		return (0);
	}
	if (symbol == '>' && type == 0 && i == data->cmdc - 1)
		return (STDOUT_FILENO);
	if (symbol == '>' && type == 0)
		return (data->fd[i % 2][1]);
	if (symbol == '>' && type == 1)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (symbol == '>' && type == 2)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

//fd[0] = input (generally pipe, but can be file/heredoc)
//fd[1] = output (generally pipe, but can be file/stdout)
void	set_direction(t_pipe *data, int i, int *fd)
{
	fd[0] = get_fd(data->cmds[i], '<', i, data);
	if (fd[0] < 0)
		errormsg("input file", 0);
	fd[1] = get_fd(data->cmds[i], '>', i, data);
	if (fd[1] < 0)
		errormsg("output file", 1);
}

// int	main(int argc, char *argv[], char *envp[])
// {
// 	int		cmdc;
// 	char	*cmds[4];

// 	if (argc > 10 && argv)
// 		ft_printf("pipex: %d\n", argc);
// 	cmdc = 2;
// 	cmds[0] = ft_strdup("<< \"Makefile '\''two\'\" < hello << 'here is another \"\" filename' echo \"Helo world!\" > \"another file name\"");
// 	cmds[1] = "cat";
// 	cmds[2] = "echo world hello";
// 	cmds[3] = "wc";
// 	printf("fd: %d\n", get_fd(cmds[0], '<'));
// 	printf("fd: %d\n", get_fd(cmds[0], '>'));
// }
