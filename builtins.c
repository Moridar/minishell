/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:50:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/26 00:27:45 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* echo_n()

cd()

pwd()

export()

unset() */

/**
 * @return 0 for success, 1 for failure
*/
/* int	export_var(t_pipe *data, char *var)
{
	int i;
	int len;

	len = ft_strchr(var, '=') - var;
	if (!len)
		return (1);
	i = 0;
	printf("len: %d\n", len);
	while(data->envp[i])
	{
		// data->envp[i] = ft_strdup("hello=hello");
		if (!ft_strncmp(data->envp[i], var, len + 1))
		{
			printf("data->envp[i]: %s\n", data->envp[i]);
			data->envp[i][0] = 'X';
			data->envp[i][1] = 'X';
			data->envp[i][2] = 'X';
			data->envp[i][3] = 'X';
			data->envp[i][4] = 'X';
			// free(data->envp[i]);
			// data->envp[i] = ft_strdup(var);
		}
		i++;
	}
	return (0);
} */
/**
 * is_exported = 0 for env
 * is_exported = 1 for export without options or arguments
 * 
 * @return 1 for success
*/
int	print_env_variables(t_pipe *data, int is_exported)
{
	int	i;

	i = 0;
	while(data->envp[++i])
	{
		printf("ENV\n");
		if (is_exported)
			printf("declare -x ");
		printf("%s\n", data->envp[i]);
	}
	return (1);
}
int builtins(char **cmd, t_pipe *data)
{
	int count;

	count = get_string_array_size(cmd);
	printf("count: %d\n", count);
	if (ft_strncmp(cmd[0], "export", 7) == 0 && count == 1)
		return print_env_variables(data, 1);
	if (ft_strncmp(cmd[0], "env", 4) == 0 && count == 1)
		return print_env_variables(data, 0);
	// if (ft_strncmp(cmd[0], "export", 7) == 0 && count == 2)
	// 	return export_var(data, cmd[1]);
	return (0);
}
