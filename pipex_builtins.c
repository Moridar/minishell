/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:50:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/18 21:34:53 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (0);
}

static int	pwd(t_pipe *data, char **cmd)
{
	char	*buff;

	buff = getcwd(NULL, 0);
	if (buff == NULL)
		msg_freeall_exit("malloc error", cmd, 1, data);
	ft_printf("%s\n", buff);
	free(buff);
	return (0);
}

static int	print_env_variables(t_pipe *data, int is_exported)
{
	int		i;
	char	*str;
	int		len;

	i = -1;
	while (data->envp[++i])
	{
		str = data->envp[i];
		if (is_exported)
		{
			len = len_next_meta_char(str, "=", 0);
			ft_putstr_fd("declare -x ", 1);
			write(1, str, len);
			if (str[len] != 0)
				ft_printf("=\"%s\"", str + len + 1);
			ft_printf("\n");
		}
		else if (ft_strchr(str, '='))
			ft_printf("%s\n", data->envp[i]);
	}
	return (0);
}

/**
 * @return -2 if memory allocation in one of the builtins failed.
 * 0 for successfully running builtin command.
 * -1 if the cmd is not built-in in parent
*/
int	child_builtins(char **cmd, t_pipe *data)
{
	int	count;

	count = sizeof_arraylist(cmd);
	if (ft_strncmp(cmd[0], "export", 7) == 0 && count == 1)
		return (print_env_variables(data, 1));
	if (ft_strncmp(cmd[0], "env", 4) == 0 && count == 1)
		return (print_env_variables(data, 0));
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		return (echo(cmd, count));
	if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		return (pwd(data, cmd));
	if (ft_strncmp(cmd[0], "exit", 5) == 0)
		return (exit_builtin(cmd, data, count));
	if (ft_strncmp(cmd[0], "export", 7) == 0)
		return (0);
	if (ft_strncmp(cmd[0], "cd", 3) == 0)
		return (0);
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (0);
	return (-1);
}
