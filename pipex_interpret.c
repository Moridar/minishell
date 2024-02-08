/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_interpret.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 21:27:35 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/08 15:11:56 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*double_interpret_and_join(char *ret, char *str, t_pipe *data)
{
	char	*interpreted_str;

	if (*str == '$' || (*str == '~' && ft_strlen(str) == 1))
		interpreted_str = expand_env_args(str, data);
	else
		interpreted_str = ft_strdup(str);
	free(str);
	if (!interpreted_str)
		return (free_return_null(ret));
	str = ft_strjoin(ret, interpreted_str);
	free(interpreted_str);
	free(ret);
	return (str);
}

static char	*interpret_double_quote(char *str, t_pipe *data, int start)
{
	char	*ret;
	char	*tmp;
	int		len;

	ret = NULL;
	while (str[start])
	{
		if (str[start] == '$' && str[start + 1] == '?')
			len = 2;
		else if (str[start] == '$')
			len = len_next_meta_char(str + start + 1, "$?'/", 1) + 1;
		else
			len = len_next_meta_char(str + start, "$", 0);
		tmp = ft_substr(str, start, len);
		if (!tmp)
			return (free_return_null(ret));
		ret = double_interpret_and_join(ret, tmp, data);
		if (!ret)
			return (NULL);
		start += len;
	}
	return (ret);
}

static char	*interpret_quote(char *str, char quote, t_pipe *data)
{
	char	*word;
	char	*tmp;

	word = ft_substr(str, 1, ft_strlen(str) - 2);
	if (!word || quote == '\'' || ft_strlen(word) == 0)
		return (word);
	tmp = interpret_double_quote(word, data, 0);
	free(word);
	return (tmp);
}

char	*interpret_and_join(char *ret, char *str, t_pipe *data)
{
	char	*interpreted_str;

	if (!str)
		return (free_return_null(ret));
	if (*str == '"' || *str == '\'')
		interpreted_str = interpret_quote(str, *str, data);
	else if (*str == '$' || (*str == '~' && ft_strlen(str) == 1))
		interpreted_str = expand_env_args(str, data);
	else
		interpreted_str = ft_strdup(str);
	free(str);
	if (!interpreted_str)
		return (free_return_null(ret));
	str = ft_strjoin(ret, interpreted_str);
	free(interpreted_str);
	free(ret);
	return (str);
}

char	*interpret(char *str, t_pipe *data)
{
	char	*ret;
	int		start;
	int		len;

	ret = ft_strdup("");
	start = 0;
	while (ret && str[start] && !ft_isspace(str[start]))
	{
		len = 0;
		if (str[start] == '\'' || str[start] == '"')
			len = get_quote_length(str + start, str[start]);
		else if (str[start] == '$' && str[start + 1] == '?')
			len = 2;
		else if (str[start] == '$')
			len = len_next_meta_char(str + start + 1, "$?\"'/", 1) + 1;
		else if (str[start] == '~')
			len = len_next_meta_char(str + start, "/", 1);
		else
			len = len_next_meta_char(str + start, "$\"'", 1);
		ret = interpret_and_join(ret, ft_substr(str, start, len), data);
		start += len;
	}
	return (ret);
}
