/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 12:03:01 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/19 15:43:28 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	close_quote(char *str, char *quote)
{
	int	i;

	i = 0;
	while (str[++i])
	{
		if (str[i] == quote)
			return (i);
	}
	errormsg("unclosed quote", 1);
}

static char	*cut_word(char *str)
{
	int		i;
	int		quote;
	int		start;
	int		end;
	char	*word;

	i = -1;
	quote = 0;
	while (str[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			quote = !quote;
		if (!quote && cmd[i] == '>')
		{
			start = i + 1;
			while (cmd[start] && ft_isspace(cmd[start]))
				start++;
			end = start;
			while (cmd[end] && !ft_isspace(cmd[end]))
				end++;
			if (word)
			{
				free(word);
				word = NULL;
			}
			word = ft_substr(cmd, start, end - start);
		}
	}
	return (word);
}

//type 0 = pipe
//type 1 = file
//type 2 = here_doc
static int	get_fd_in(char *cmd)
{
	int		i;
	char	*filename;
	int		type;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i += close_quote(cmd + i, cmd[i]);
		else if (cmd[i] == '<')
		{
			type = 1;
			if (cmd[i + 1] == '<')
				type = 2;
			if (filename)
				free(filename);
			filename = cut_word(cmd + i);
		}
	}
	if (type == 0)
		return (0);
	else if (type == 1)
		return (open(filename, O_RDONLY));
	else if (type == 2)
		return (here_doc(filename));
}

//type 0 = pipe
//type 1 = file
//type 2 = append
static int	get_fd_out(char *cmd)
{
	int		type;
	char	*filename;

	if (type == 0)
		return (1);
	else if (type == 1)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (type == 2)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

//fd[0] = input (generally pipe, but can be file/heredoc)
//fd[1] = output (generally pipe, but can be file/stdout)
void	set_direction(t_pipe *data, int i, int *fd)
{
	fd[0] = get_fd_in(data->cmds[i]);
	fd[1] = check_out(data, i);
}
