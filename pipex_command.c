/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:11:39 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/08 19:30:00 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Replaces all the whitespaces inside the command to special char.
 * It should not affect whitespces inside quotes.
 * 
 * @return pointer to a new command string.
*/
char	*replace_spaces(char *cmd)
{
	int		i;
	char	*new_cmd;

	new_cmd = ft_strdup(cmd);
	if (!new_cmd)
		return (NULL);
	i = 0;
	while (new_cmd[i])
	{
		if (ft_isspace(new_cmd[i]))
			new_cmd[i] = 31;
		if ((new_cmd[i] == '\'' || new_cmd[i] == '"')
			&& get_quote_length(&new_cmd[i], new_cmd[i]) != 1)
			i += get_quote_length(&new_cmd[i], new_cmd[i]) - 1;
		i++;
	}
	return (new_cmd);
}

char	**interpret_commands(char **command, t_pipe *data)
{
	int		i;
	int		new_size;
	char	*interpreted_str;

	i = 0;
	new_size = 0;
	while (command[i])
	{
		interpreted_str = interpret(command[i], data);
		if (!interpreted_str)
			return (NULL);
		free(command[i]);
		if (!interpreted_str[0])
		{
			free(interpreted_str);
			interpreted_str = NULL;
		}
		command[i] = interpreted_str;
		if (command[i++])
			new_size++;
	}
	if (new_size != i)
		command = reallocate_arraylist(command, new_size);
	return (command);
}

/**
 * Splits the shell command. Preserves whitespaces inside quotes, expands env
 * variables and trims wrapping quotes.
 * If there were any redirections and filenames in the string, they should be 
 * removed and replaced with whitespaces before using the function.
 * 
 * Some kind of a smart make_args()
 * 
 * @return double dimension array of command and it's flags
*/
char	**split_shell_cmd(char	*cmd, t_pipe *data)
{
	char	*new_str;
	char	**command;
	char	**interpreted_command;
	int		size;

	new_str = replace_spaces(cmd);
	command = ft_split(new_str, 31);
	size = get_string_array_size(command);
	free(new_str);
	interpreted_command = interpret_commands(command, data);
	if (interpreted_command == NULL)
	{
		while (size >= 0)
			free(command[size--]);
		free(command);
	}
	return (interpreted_command);
}
