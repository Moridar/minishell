/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_env_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 17:12:21 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/05 02:36:15 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Replaces all consecutive whitespaces to a space char ' '(32).
 * Also trims all spaces before and after including next line.
 *
 * @return new string
 */
char	*shift_replace_spaces(char *str)
{
	int		i;
	char	*res;
	int		len;

	res = ft_strtrim(str, (char []){9, 10, 11, 12, 13, 32});
	i = 0;
	while (res[i])
	{
		if (ft_isspace(res[i]))
		{
			res[i] = ' ';
			if (ft_isspace(res[i + 1]))
			{
				res[i + 1] = ' ';
				len = ft_strlen(&res[i]);
				ft_memmove(&res[i], &res[i + 1], len);
				i--;
			}
		}
		i++;
	}
	return (res);
}

/**
 * Expands simple env variable in argument string
 * @param start a pointer to $ of env variable
 * @return new allocated expanded env variable or empty string if env variable
 * not found.
 */
static char	*expand_simple_var(char *start, t_pipe *data)
{
	char	*res;
	char	*temp;
	int		i;

	res = NULL;
	i = -1;
	while (!res && data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], start + 1, ft_strlen(start + 1)) == 0
			&& (data->envp[i])[ft_strlen(start + 1)] == '=' )
		{
			res = ft_strdup(data->envp[i] + ft_strlen(start + 1) + 1);
			temp = res;
			res = shift_replace_spaces(temp);
			free(temp);
		}
	}
	if (!res)
		return (NULL);
	return (res);
}

/**
 * Expands environment variables found in the argument string.
 * Works with expanding double quoted argument, single quoted and simple.
 * If the argument string starts with env variable (simple), the whole string
 * will be treated as the env variable name.
 * If the argument string is single quoted nothing will happen.
 *
 * @param str argument string after the shell command is split.
 * @return a new allocated string even if there is nothing to expand.
 */
char	*expand_env_args(char *str, t_pipe *data)
{
	if (*str != '$')
		return (ft_strdup(str));
	if (str[1] == 0)
		return (ft_strdup(str));
	if (str[1] == '?')
		return (ft_itoa(g_exit_status));
	return (expand_simple_var(str, data));
}
