/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 00:50:23 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/16 10:38:39 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Only checks trailing '=' for export builtin
 * @param builtin can be unset or export
*/
static int	validate_key(int keylen, char *key, char *builtin)
{
	int	i;
	int	err;

	err = 0;
	i = 0;
	if (ft_isalpha(key[i]) == 0 && key[i] != '_')
		err = 1;
	while (!err && ++i < keylen - 1)
		if (ft_isalnum(key[i]) == 0 && key[i] != '_')
		err = 1;
	if (!err)
		return (0);
	ft_putstr_fd("bvsh: ", 2);
	ft_putstr_fd(builtin, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	unset_var(t_pipe *data, char *env_var)
{
	int		size;
	int		i;
	int		keylen;

	keylen = len_next_meta_char(env_var, "=", 0);
	keylen += env_var[keylen] == '=';
	if (!env_var || validate_key(keylen, env_var, "unset"))
		return (1);
	size = sizeof_arraylist(data->envp);
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], env_var, keylen) == 0
			&& (!data->envp[i][keylen] || data->envp[i][keylen] == '='))
		{
			free(data->envp[i]);
			data->envp[i] = NULL;
			data->envp = reallocate_arraylist(data->envp, size - 1);
			break ;
		}
	}
	if (!data->envp)
		return (-2);
	return (0);
}

/**
 * @param count size of **cmd
*/
static int	unset_builtin(t_pipe *data, char **cmd, int count)
{
	int	i;
	int	result;

	if (count < 2)
		return (0);
	i = 1;
	while (cmd[i])
	{
		result = unset_var(data, cmd[i]);
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}

/**
 * @return 0 for success, -2 if malloc failed
*/
int	export(t_pipe *data, char *var)
{
	int		i;
	int		keylen;
	char	**newenvp;

	if (!var)
		return (-2);
	keylen = len_next_meta_char(var, "=", 0) + 1;
	if (validate_key(keylen, var, "export") == 1)
		return (free_return(var, 1));
	i = -1;
	while (data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], var, keylen) != 0)
			continue ;
		free(data->envp[i]);
		data->envp[i] = var;
		return (0);
	}
	newenvp = copy_arraylist(data->envp, 1);
	if (!newenvp)
		return (free_return(var, -2));
	newenvp[i] = var;
	freeall(data->envp);
	data->envp = newenvp;
	return (0);
}

/**
 * @return -2 if memory allocation in one of the builtins failed.
 * 0 for successfully running builtin command.
 * -1 if the cmd is not built-in in parent
*/
int	builtins(char **cmd, t_pipe *data)
{
	int	count;

	if (!data || !cmd)
		return (0);
	count = sizeof_arraylist(cmd);
	if (ft_strncmp(cmd[0], "export", 7) == 0 && count > 1)
		data->status = export(data, ft_strdup(cmd[1]));
	if (ft_strncmp(cmd[0], "cd", 3) == 0)
		data->status = cd(data, cmd, count);
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		data->status = (unset_builtin(data, cmd, count));
	if (ft_strncmp(cmd[0], "exit", 5) == 0)
		data->status = exit_builtin(cmd, data, count);
	else
		return (-1);
	return (0);
}
