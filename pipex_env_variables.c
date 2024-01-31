/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_env_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 17:12:21 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/31 19:06:08 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * Expands first found env variable in an argument string
 * @param start a pointer to $ of env variable
 * @return
 */
char	*expand_double_quoted(char *str, char *start)
{
	char	*temp;
	char	*before_var;
	char	*after_var;
	char	*res;
	int		len;

	printf("double quote $$ ever used\n");
	start++;
	len = 0;
	while (start[len] && !ft_isspace(start[len])
		&& start[len] != '\n' && start[len] != '\"')
		len++;
	temp = ft_substr(str, start - str, len);
	before_var = ft_substr(str, 0, start - str - 1);
	res = ft_strjoin(before_var, getenv(temp));
	free(before_var);
	free(temp);
	after_var = start + len;
	temp = res;
	res = ft_strjoin(temp, after_var);
	free(temp);
	return (res);
}

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
char	*expand_simple_var(char *start, t_pipe *data)
{
	char	*res;
	char	*temp;
	int		i;

	res = NULL;
	i = -1;
	while (!res && data->envp[++i])
	{
		if (ft_strncmp(data->envp[i], start + 1, ft_strlen(start + 1)) == 0)
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
	char	*ret;

	if (str[1] == 0)
		return (str);
	if (str[1] == '?')
		return (ft_itoa(g_exit_status));
	ret = expand_simple_var(str, data);
	return (ret);
}

// char	*expand_env_args(char *str, t_pipe *data)
// {
// 	char	*start;
// 	char	*res;
// 	char	*temp;

// 	res = ft_strdup(str);
// 	start = ft_strchr(res, '$');
// 	while (res && res[0] == '"' && res[ft_strlen(res) - 1] == '"' && start)
// 	{
// 		temp = res;
// 		res = expand_double_quoted(temp, start);
// 		free(temp);
// 		start = ft_strchr(res, '$');
// 	}
// 	if (res == start)
// 	{
// 		temp = res;
// 		res = expand_simple_var(start, data);
// 		free(temp);
// 	}
// 	return (res);
// }

// when the string is glued: "hello"$SHELL'hey'
// another test case: "$SHELL's'"$SHELL
// 's' will be not expanded, $SHELL will be expanded.
// 1. expand simple variables (in loop expand variable => check if it hits ' or "" or the end of string, then expand)
// 2. expand "quotes" (trim "" and expand env variables). Insided "quotes" expand env vars but not 'singles'
// 3. expand 'singles' that are not inside (trim and join)

/* int	main(void)
{
	char	*str1;
	// char	*str2;
	char*new_str;
	str1 = "\"$USER start $SHELL end $USER\"";
	// str1 = "\"\"";
	// str1 = "$SHELL";
	// str1 = "$HELLO";
	// str1 = "$SHELLs";
	// str1 = "'$SHELL'";

	// char *new_str;
	// expand_env_arg(str);
	new_str = expand_env_args(str1);
	printf("%s\n", new_str);
	free(new_str);

	return (0);
} */
