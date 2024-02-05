/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 03:16:28 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/05 04:15:56 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks commands first arguments cmd[1] to include a tilde and interpret it.
 */
static char	*interpret_tilde_path(t_pipe *data, char **cmd)
{
	char	*temp;
	char	*new_path;

	new_path = ft_strjoin("$HOME", cmd[1] + 1);
	if (!new_path)
		errormsg("cd", 1, -1);
	temp = new_path;
	new_path = interpret(temp, data);
	if (!new_path)
		errormsg("cd", 1, -1);
	free(temp);
	return (new_path);
}

/**
 * Interprets tilde in the path and changes directory and .
 * chdir returns 0 on success, -1 on error, -1 is returned, and errno is set to
 * indicate the error.
*/
int	change_directory(t_pipe *data, char **cmd)
{
	char	*path;
	int		result;

	path = cmd[1];
	if (cmd[1][0] == '~')
	{
		path = interpret_tilde_path(data, cmd);
		result = chdir(path);
		free(path);
	}
	result = chdir(path);
	return (result);
}
