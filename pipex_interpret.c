/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_interpret.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 21:27:35 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/06 01:38:17 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*interpret_double_quote(char *str, t_pipe *data, int start)
{
	char	*ret;
	char	*tmp;
	char	*substr;
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
		substr = expand_env_args(tmp, data);
		free(tmp);
		tmp = ft_strjoin(ret, substr);
		free(ret);
		free(substr);
		ret = tmp;
		start += len;
	}
	return (ret);
}

char	*interpret_quote(char *str, char quote, t_pipe *data)
{
	char	*word;
	char	*tmp;

	word = ft_substr(str, 1, ft_strlen(str) - 2);
	if (quote == '"')
	{
		tmp = word;
		word = interpret_double_quote(word, data, 0);
		if (!word)
			return (tmp);
		free(tmp);
	}
	return (word);
}

char	*interpret_and_join(char *ret, char *str, t_pipe *data)
{
	char	*interpreted_str;

	if (*str == '"' || *str == '\'')
		interpreted_str = interpret_quote(str, *str, data);
	else if (*str == '$' || (*str == '~' && ft_strlen(str) == 1))
		interpreted_str = expand_env_args(str, data);
	else
		interpreted_str = ft_strdup(str);
	free(str);
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

	ret = NULL;
	start = 0;
	while (str[start] && !ft_isspace(str[start]))
	{
		len = 0;
		if (str[start] == '\'' || str[start] == '"')
			len = get_quote_length(str + start, str[start]);
		else if (str[start] == '$' && str[start + 1] == '?')
			len = 2;
		else if (str[start] == '$')
			len = len_next_meta_char(str + start + 1, "$?\"'/", 1) + 1;
		else if (str[start] == '~' && (str[start + 1] == '/'
				|| !str[start + 1] || ft_isspace(str[start + 1])))
			len = 1;
		else
			len = len_next_meta_char(str + start, "$\"'", 1);
		ret = interpret_and_join(ret, ft_substr(str, start, len), data);
		start += len;
	}
	return (ret);
}

/* int	main(void)
{
	char	*str = "he\"ll\"o_\"$SH'E'LL\"$SHELL";
//char	*str = "\'hey hey\'$SHELL\"some $SHELL some\"";
	// char	*str = "$SHELL_hello";
	//char	*str = "$SHELL\"_hello\"";
	//char	*str = "$SH\"E\"LL'_hello'";
	printf("|%s|\n|%s|\n", str, interpret(str));
	//printf("|%s|\n", cut_filename("< Makefile>out echo helo world > output"));

	return (0);
}  */