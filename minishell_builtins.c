/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:50:23 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/28 03:16:47 by bsyvasal         ###   ########.fr       */
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

static int	unset(t_pipe *data, char **cmd, int count)
{
	char	*tmp;
	int		i;
	int		size;

	if (count < 2)
		return (1);
	size = get_string_array_size(data->envp);
	tmp = ft_strjoin(cmd[1], "=");
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], tmp, ft_strlen(tmp)) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = NULL;
			data->envp = reallocate_arraylist(data->envp, size);
		}
	}
	free(tmp);
	return (1);
}

/**
 * @return 1 for success, 0 for failure
*/
static int	export(t_pipe *data, char *var)
{
	int		i;
	char	*key;
	int		keylen;
	char	**newenvp;

	keylen = len_next_meta_char(var, "=", 0);
	if (keylen == (int) ft_strlen(var))
		return (1);
	key = ft_substr(var, 0, keylen);
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], key, ft_strlen(key)) != 0)
			continue ;
		free(data->envp[i]);
		data->envp[i] = ft_strdup(var);
		free(key);
		return (1);
	}
	free(key);
	newenvp = copy_double_array(data->envp, 1);
	newenvp[i] = ft_strdup(var);
	freeall(data->envp);
	data->envp = newenvp;
	return (1);
}

static int	cd(t_pipe *data, char **cmd, int count)
{
	char	*ptr;

	if (count < 2)
		ft_putstr_fd("bvsh: cd: too few arguments\n", 2);
	else if (count > 2)
		ft_putstr_fd("bvsh: cd: too many arguments\n", 2);
	else if (chdir(cmd[1]) == 0)
	{
		ptr = ft_strjoin("PWD=", getcwd(NULL, 0));
		export(data, ptr);
		free(ptr);
	}
	return (1);
}

int	builtins(char **cmd, t_pipe *data)
{
	int	count;

	if (!data)
		printf("no data");
	if (!cmd)
		return (0);
	count = get_string_array_size(cmd);
	if (ft_strncmp(cmd[0], "export", 7) == 0 && count > 1)
		return (export(data, cmd[1]));
	if (ft_strncmp(cmd[0], "cd", 3) == 0)
		return (cd(data, cmd, count));
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (unset(data, cmd, count));
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
