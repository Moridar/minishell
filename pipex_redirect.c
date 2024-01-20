/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:03:01 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/20 03:01:52 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*cut_filename(char *str)
{
	int		start;
	int		end;
	char	*filename;

	filename = NULL;
	start = 0;
	while (str[start] == '<')
		start++;
	while (ft_isspace(str[start]))
		start++;
	while (str[start] && str[start] != '<' && !ft_isspace(str[start]))
	{
		if (str[start] == '\'' || str[start] == '"')
		{
			end = start + get_quote_length(str + start, str[start]) - 1;
			filename = ft_strjoin(filename, interpret_quote(str + start, str[start]));
		}
		else
		{
			end = start;
			while (str[end] && !ft_isspace(str[end]))
				end++;
			filename = ft_strjoin(filename, ft_substr(str, start, end - start));
		}
		start = end + 1;
		printf("filename: %s\n", filename);
	}
	ft_memset(str, ' ', end + 1);
	return (filename);
}

//type 0 = pipe
//type 1 = file
//type 2 = here_doc
static int	get_fd_in(char *cmd)
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
		if (cmd[i] == '<')
		{
			type = 1;
			if (cmd[i + 1] == '<')
				type = 2;
			if (filename)
				free(filename);
			filename = cut_filename(cmd + i);
			ft_printf("cmd: %s\n", cmd);
		}
	}
	if (type == 1)
		return (open(filename, O_RDONLY));
	else if (type == 2)
		return (2);
	return (0);
}

//type 0 = pipe
//type 1 = file
//type 2 = append
static int	get_fd_out(char *cmd)
{
	int		type;
	char	*filename;

	type = 0;
	filename = ft_strdup("outfile");
	if (cmd)
		type = 0;
	if (type == 1)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (type == 2)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (1);
}

//fd[0] = input (generally pipe, but can be file/heredoc)
//fd[1] = output (generally pipe, but can be file/stdout)
void	set_direction(t_pipe *data, int i, int *fd)
{
	fd[0] = get_fd_in(data->cmds[i]);
	if (fd[0] < 0)
		errormsg("input file", 1);
	fd[1] = get_fd_out(data->cmds[i]);
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
// 	printf("cmd: %s\n", cmds[0]);
// 	printf("fd: %d\n", get_fd_in(cmds[0]));
// }
