/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:50:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/27 01:36:05 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	echo(char **cmd, int count)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (count >= 2 && ft_strncmp(cmd[1], "-n", 3) == 0)
		n_flag = i++;
	while (cmd[i])
	{
		ft_printf("%s", cmd[i]);
		if (cmd[i++ + 1])
			ft_printf(" ");
	}
	if (!n_flag)
		ft_printf("\n");
	return (1);
}

static int	pwd(void)
{
	getcwd(NULL, 0);
	return (1);
}

static int	print_env_variables(t_pipe *data, int is_exported)
{
	int	i;

	i = 0;
	while (data->envp[++i])
	{
		if (is_exported)
			printf("declare -x ");
		printf("%s\n", data->envp[i]);
	}
	return (1);
}

int	child_builtins(char **cmd, t_pipe *data)
{
	int	count;

	count = get_string_array_size(cmd);
	if (ft_strncmp(cmd[0], "export", 7) == 0 && count == 1)
		return (print_env_variables(data, 1));
	if (ft_strncmp(cmd[0], "env", 4) == 0 && count == 1)
		return (print_env_variables(data, 0));
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		return (echo(cmd, count));
	if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		return (pwd());
	if (ft_strncmp(cmd[0], "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "exit", 5) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (1);
	return (0);
}
