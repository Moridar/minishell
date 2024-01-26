/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:50:23 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/27 01:19:30 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @param return status
*/
void	exit_builtin(char *status)
{
	char	*status_converted;
	int		status_n;
	char	*error_msg;
	char	*temp;
	
	if (status)
	{
		status_n = ft_atoi(status);
		status_converted = ft_itoa(status_n);
/* 		if (!status_converted) // this should be somehow handled in the outer functions up to where the builtins is called
			return (0); */
		if (status[0] == '+')
			status = status + 1;
		if (ft_strncmp(status, status_converted, ft_strlen(status_converted) + 1) == 0)
		{
			free(status_converted);
			ft_putstr_fd("exit\n", 1);
			exit((char)status_n);
		}
		else
		{
			free(status_converted);
			error_msg = ft_strjoin("bvsh: exit: ", status);
			temp = error_msg;
			error_msg = ft_strjoin(temp, ": numeric argument required");
			ft_putstr_fd(error_msg, 2);
			free(temp);
			free(error_msg);
			exit(255);
		}
	}
}

//unset();
//export();
//cd();

/**
 * @return 1 for success, 0 for failure
*/
/* int	export_var(t_pipe *data, char *var)
{
	int i;
	int len;

	len = ft_strchr(var, '=') - var;
	if (!len)
		return (0);
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
			printf("data->envp[i]: %s\n", data->envp[i]);
			// free(data->envp[i]);
			// data->envp[i] = ft_strdup(var);
		}
		i++;
	}
	return (1);
} */
/**
 * is_exported = 0 for env
 * is_exported = 1 for export without options or arguments
 * 
 * @return 1 for success
*/
int	builtins(char **cmd, t_pipe *data)
{
	int	count;

	if (!data)
		printf("no data");
	count = get_string_array_size(cmd);
	// printf("is_parent in builtins!\n");
	// printf("cmd[0]: %s\n", cmd[0]);
	// printf("ft_strncmp(cmd[0], \"exit\", 5): %d\n", ft_strncmp(cmd[0], "exit", 5));
	if (ft_strncmp(cmd[0], "exit", 5) == 0 && count == 1)
		exit(0);
	if (ft_strncmp(cmd[0], "exit", 5) == 0 && count == 2)
		exit_builtin(cmd[1]);
	if (ft_strncmp(cmd[0], "exit", 5) == 0 && count > 2)
	{
		ft_putstr_fd("exit\nbvsh: exit: too many arguments\n", 2);
		return (1);
	}
	return (0);
}