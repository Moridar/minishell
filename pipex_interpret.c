/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_interpret.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 21:27:35 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/28 01:15:51 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*interpret_and_join(char *ret, char *str);

// static char	*interpret_simple(char *str)
// {
// 	char	*word;

// 	word = expand_env_args(str);
// 	return (word);
// }

static char	*interpret_double_quote(char *str)
{
	char	*ret;
	char	*tmp;
	char	*substr;
	int		start;
	int		len;

	ret = NULL;
	start = 0;
	while (str[start])
	{
		len = 0;
		if (str[start] == '$')
			len = len_next_meta_char(str + start + 1, "$'", 1) + 1;
		else
			len = len_next_meta_char(str + start, "$", 0);
		substr = expand_env_args(ft_substr(str, start, len));
		tmp = ft_strjoin(ret, substr);
		free(ret);
		free(substr);
		ret = tmp;
		start += len;
	}
	return (ret);
}

char	*interpret_quote(char *str, char quote)
{
	char	*word;
	char	*tmp;

	word = ft_substr(str, 1, ft_strlen(str) - 2);
	if (quote == '"')
	{
		tmp = word;
		word = interpret_double_quote(word);
		free(tmp);
	}
	return (word);
}

char	*interpret_and_join(char *ret, char *str)
{
	char	*interpreted_str;
	char	*tmp;

	if (*str == '"' || *str == '\'')
		interpreted_str = interpret_quote(str, *str);
	else if (*str == '$')
		interpreted_str = expand_env_args(str);
	else
	{
		tmp = ft_strjoin(ret, str);
		free(str);
		free(ret);
		return (tmp);
	}
	tmp = ft_strjoin(ret, interpreted_str);
	free(str);
	free(ret);
	free(interpreted_str);
	return (tmp);
}

char	*interpret(char *str)
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
		else if (str[start] == '$')
			len = len_next_meta_char(str + start + 1, "$\"'", 1) + 1;
		else
			len = len_next_meta_char(str + start, "$\"'", 1);
		ret = interpret_and_join(ret, ft_substr(str, start, len));
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