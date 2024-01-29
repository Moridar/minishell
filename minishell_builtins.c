/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:50:23 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/29 15:22:18 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	unset(t_pipe *data, char **cmd, int count)
{
	char	*tmp;
	int		i;
	int		size;

	if (count < 2)
		return (1);
	size = get_string_array_size(data->envp);
	tmp = ft_strjoin(cmd[1], "=");
	if (!tmp)
		return (2);
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], tmp, ft_strlen(tmp)) == 0)
		{
			free(data->envp[i]);
			data->envp[i] = NULL;
			data->envp = reallocate_arraylist(data->envp, size - 1);
			break ;
		}
	}
	free(tmp);
	if (!data->envp)
		return (2);
	return (1);
}

/**
 * @return 1 for success, 2 if malloc failed
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
	if (!key)
		return (2);
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], key, ft_strlen(key)) != 0)
			continue ;
		free(key);
		key = ft_strdup(var);
		if (!key)
			return (2);
		free(data->envp[i]);
		data->envp[i] = key;
		return (1);
	}
	free(key);
	newenvp = copy_double_array(data->envp, 1);
	if (!newenvp)
		return (2);
	newenvp[i] = ft_strdup(var);
	if (!newenvp[i])
	{
		free_double_arr(data->envp, i);
		return (2);
	}
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
		if (!ptr)
			return (2);
		export(data, ptr);
		free(ptr);
	}
	return (1);
}

/**
 * @return 2 if memory allocation in one of the builtins failed.
 * 1 for successfully running builtin command.
 * 0 if the builtin has not run.
*/
int	builtins(char **cmd, t_pipe *data, char *line)
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
		return (exit_builtin(cmd[1], data, cmd, line));
	if (ft_strncmp(cmd[0], "exit", 5) == 0 && count > 2)
	{
		ft_putstr_fd("exit\nbvsh: exit: too many arguments\n", 2);
		return (1);
	}
	return (0);
}
