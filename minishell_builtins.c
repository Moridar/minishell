/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:50:23 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/05 02:36:15 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	validate_key(int keylen, char *key)
{
	int	i;
	int	err;

	err = 0;
	i = 0;
	if (ft_isalpha(key[i]) == 0)
		err = 1;
	while (!err && i < keylen - 1)
		if (ft_isalnum(key[i++]) == 0)
			err = 1;
	if (!err && keylen > (int) ft_strlen(key))
		return (0);
	if (keylen == 1 || key[keylen - 1] != '=')
		err = 1;
	if (!err)
		return (1);
	ft_putstr_fd("bvsh: export: `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_exit_status = 1;
	return (0);
}

/**
 * @return 1 for success, 2 if malloc failed
*/
static int	export(t_pipe *data, char *var)
{
	int		i;
	int		keylen;
	char	**newenvp;

	if (!var)
		return (2);
	keylen = len_next_meta_char(var, "=", 0) + 1;
	if (validate_key(keylen, var) == 0)
		return (free_return(var, 1));
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], var, keylen) != 0)
			continue ;
		free(data->envp[i]);
		data->envp[i] = var;
		return (1);
	}
	newenvp = copy_double_array(data->envp, 1);
	if (!newenvp)
		return (free_return(var, 2));
	newenvp[i] = var;
	freeall(data->envp);
	data->envp = newenvp;
	return (1);
}

static int	cd(t_pipe *data, char **cmd, int count)
{
	char	*key;
	char	*path;

	g_exit_status = 1;
	if (count < 2)
		ft_putstr_fd("bvsh: cd: too few arguments\n", 2);
	else if (chdir(cmd[1]) == 0)
	{
		path = getcwd(NULL, 0);
		if (!path)
			return (2);
		key = ft_strjoin("PWD=", path);
		free(path);
		if (!key)
			return (2);
		g_exit_status = 0;
		return (export(data, key));
	}
	else
	{
		ft_putstr_fd("bvsh: cd: ", 2);
		perror(cmd[1]);
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
		return (export(data, ft_strdup(cmd[1])));
	if (ft_strncmp(cmd[0], "cd", 3) == 0)
		return (cd(data, cmd, count));
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (unset(data, cmd, count));
	if (ft_strncmp(cmd[0], "exit", 5) == 0 && count == 1)
		exit(1);
	if (ft_strncmp(cmd[0], "exit", 5) == 0 && count == 2)
		return (exit_builtin(cmd[1], data, cmd, line));
	if (ft_strncmp(cmd[0], "exit", 5) == 0 && count > 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("bvsh: exit: too many arguments\n", 2);
		g_exit_status = 1;
		return (1);
	}
	return (0);
}
