/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:19:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/25 23:08:17 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shlvl_increment(t_pipe *data)
{
	int		i;
	int		shlvl;
	char	*shlvl_str;

	i = -1;
	shlvl_str = getenv("SHLVL");
	while (shlvl_str && ft_isdigit(shlvl_str[++i]))
		;
	if (!shlvl_str || shlvl_str[i] != '\0')
		return (export(data, ft_strdup("SHLVL=1")));
	shlvl = ft_atoi(shlvl_str);
	if (shlvl < 0)
		return (export(data, ft_strdup("SHLVL=0")));
	shlvl_str = ft_itoa(shlvl + 1);
	if (!shlvl_str)
		return (1);
	i = export(data, ft_strjoin("SHLVL=", shlvl_str));
	free(shlvl_str);
	return (i);
}

/**
 * @return size of the double dimension string array, for example,
 * created by ft_split().
 */
int	sizeof_arraylist(char **arraylist)
{
	int	j;

	j = 0;
	while (arraylist[j])
	{
		j++;
	}
	return (j);
}

/**
 * Used to copy double dimension array arr1 to arr2.
 * For example, envp.
 */
char	**copy_arraylist(char **arr1, int increase_size)
{
	int		size;
	int		i;
	int		len;
	char	**arr2;

	i = 0;
	size = sizeof_arraylist(arr1);
	arr2 = (char **)ft_calloc((size + 1 + increase_size), sizeof(char *));
	if (!arr2)
		return (NULL);
	while (i < size)
	{
		len = (ft_strlen(arr1[i]) + 1);
		arr2[i] = (char *)ft_calloc(len, sizeof(char));
		if (!arr2[i])
		{
			freeall(arr2);
			return (NULL);
		}
		ft_memmove(arr2[i], arr1[i], len);
		i++;
	}
	return (arr2);
}

/**
 * Reallocates the arraylist when some elements are deleted
 * DO NOT use for increased size list.
 * @param newsize is the size of the new array
 */
char	**reallocate_arraylist(char **arr, int newsize)
{
	char	**new_arr;
	int		i;
	int		j;

	new_arr = (char **)ft_calloc((newsize + 1), sizeof(char *));
	if (!new_arr)
		return (NULL);
	i = 0;
	j = 0;
	while (j < newsize)
	{
		if (arr[i])
			new_arr[j++] = arr[i];
		i++;
	}
	free(arr);
	return (new_arr);
}

/**
 * Replaces all pipe char (|) inside prompt line to a special char (31) for
 * the split avoiding pipe chars (|) inside quotes stay unaffected.
 */
int	replace_pipes(char *cmd)
{
	int		i;
	int		count;
	int		valid;

	i = -1;
	count = 0;
	valid = 0;
	while (cmd[++i])
	{
		if (cmd[i] != '|' && ft_isprint(cmd[i]) && !ft_isspace(cmd[i]))
			valid = 1;
		if (cmd[i] == '|')
		{
			if (!valid || cmd[i + 1] == '|')
				return (write(2, "bvsh: syntax error "
						"near unexpected token `|'\n", 46) - 47);
			cmd[i] = 31;
			count++;
			valid = 0;
		}
		if ((cmd[i] == '\'' || cmd[i] == '"')
			&& get_quote_length(&cmd[i], cmd[i]) != 1)
			i += get_quote_length(&cmd[i], cmd[i]) - 1;
	}
	return (count);
}
