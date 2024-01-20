/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:03:01 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/20 17:37:39 by bsyvasal         ###   ########.fr       */
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

//infile
//type 0 = pipe / std-in
//type 1 = file
//type 2 = here_doc
//outfile
//type 0 = pipe / std-out
//type 1 = file
//type 2 = append
static int	get_fd(char *cmd, char symbol)
{
	int		i;
	char	*filename;
	int		type;

	filename = NULL;
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i += get_quote_length(cmd + i, cmd[i]);
		type = get_num_of_repeats(cmd + i, symbol);
		if (type == 1 || type == 2)
		{
			if (filename)
				free(filename);
			filename = cut_filename(cmd + i, symbol);
		}
		if (type >= 3)
			errormsg("syntax error near unexpected token `<'", 1);
	}
	if (type == 1)
		return (open(filename, O_RDONLY));
	else if (type == 2)
		return (2);
	return (0);
}

//fd[0] = input (generally pipe, but can be file/heredoc)
//fd[1] = output (generally pipe, but can be file/stdout)
void	set_direction(t_pipe *data, int i, int *fd)
{
	fd[0] = get_fd(data->cmds[i], '<');
	if (fd[0] < 0)
		errormsg("input file", 1);
	fd[1] = get_fd(data->cmds[i], '>');
	if (fd[1] < 1)
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
