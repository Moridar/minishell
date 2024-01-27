/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:50:23 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/27 19:07:21 by bsyvasal         ###   ########.fr       */
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

int	unset(t_pipe *data, char *cmd)
{
	if (data && cmd)
		return (2);
	return (1);
}

/**
 * @return 1 for success, 0 for failure
*/
int	export(t_pipe *data, char *var)
{
	int		i;
	char	*key;
	int		keylen;
	char	**newenvp;

	keylen = len_next_meta_char(var, "=", 0);
	key = ft_substr(var, 0, keylen);
	if (!key || keylen == 0)
		return (1);
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], key, ft_strlen(key)) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = ft_strdup(var);
			free(key);
			return (1);
		}
	}
	copy_double_array(data->envp, &newenvp, 1);
	newenvp[i] = ft_strdup(var);
	return (1);
}

int	cd(t_pipe *data, char **cmd, int count)
{
	char	*buff;
	char	*ptr;

	if (count == 1)
	{
		buff = getcwd(NULL, 0);
		if (buff == NULL)
			errormsg("cd", 1);
		ptr = ft_strrchr(buff, '/');
		*ptr = 0;
		chdir(buff);
		ptr = ft_strjoin("PWD=", buff);
		export(data, ptr);
		free(buff);
		free(ptr);
		return (1);
	}
	chdir(cmd[1]);
	ptr = ft_strjoin("PWD=", cmd[1]);
	export(data, ptr);
	free(ptr);
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
	if (ft_strncmp(cmd[0], "cd", 3) == 0 && count > 2)
		return (printf("bsvh: cd: too many arguments\n"));
	if (ft_strncmp(cmd[0], "cd", 3) == 0)
		return (cd(data, cmd, count));
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (unset(data, cmd[0]));
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
