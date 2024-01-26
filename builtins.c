/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:50:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/26 17:44:43 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* 
cd()

pwd()

export()

unset() */

int	echo_n(char *str) {
	printf("%s", str);
	return (1);
}

/**
 * @brief getcwd() needs to know the whole buffer size in advance and
 * returns NULL if the path is shorter than size.\n
 * 
 * If any argument, pwd() does not process it and does not error.
 * 
 * @return 1 for success, 0 for failure.
*/
int pwd()
{
	char *buff;
	int	size;

	size = 5;
	buff = (char *)malloc(size);
	if (!buff)
		return (0); // this should be somehow handled in the outer functions up to where the builtins is called
	while (!getcwd(buff, size))
	{
		size++;
		free(buff);
		buff = (char *)malloc(size);
		if (!buff)
			return (0); // this should be somehow handled in the outer functions up to where the builtins is called
	}
	free(buff);
	printf("%s\n", buff);
	return (1);
}

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
int	print_env_variables(t_pipe *data, int is_exported)
{
	int	i;

	i = 0;
	while(data->envp[++i])
	{
		// printf("ENV\n");
		if (is_exported)
			printf("declare -x ");
		printf("%s\n", data->envp[i]);
	}
	return (1);
}
int builtins(char **cmd, t_pipe *data, int	is_parent)
{
	int count;

	count = get_string_array_size(cmd);
	printf("count: %d\n", count);
	if (!is_parent)
	{
		if (ft_strncmp(cmd[0], "export", 7) == 0 && count == 1)
			return print_env_variables(data, 1);
		if (ft_strncmp(cmd[0], "env", 4) == 0 && count == 1)
			return print_env_variables(data, 0);
		if (ft_strncmp(cmd[0], "history", 8) == 0 && count == 1)
			return history();
		// if (ft_strncmp(cmd[0], "export", 7) == 0 && count == 2)
		// 	return export_var(&data, cmd[1]);
		if (ft_strncmp(cmd[0], "echo", 5) == 0 && count >= 2 && ft_strncmp(cmd[1], "-n", 3) == 0)
			return (echo_n(cmd[2]));
		if (ft_strncmp(cmd[0], "pwd", 4) == 0 && pwd())
			return (1);
	}
	else if (is_parent)
	{
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
	}

	return (0);
}
