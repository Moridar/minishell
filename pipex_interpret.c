/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_interpret.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 21:27:35 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/31 19:13:08 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*interpret_double_quote(char *str, t_pipe *data)
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
		if (str[start] == '$' && str[start + 1] == '?')
			len = 2;
		else if (str[start] == '$')
			len = len_next_meta_char(str + start + 1, "$?'", 1) + 1;
		else
			len = len_next_meta_char(str + start, "$", 0);
		substr = expand_env_args(ft_substr(str, start, len), data);
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
		word = interpret_double_quote(word, data);
		free(tmp);
	}
	return (word);
}

char	*interpret_and_join(char *ret, char *str, t_pipe *data)
{
	char	*interpreted_str;
	char	*tmp;

	if (*str == '"' || *str == '\'')
		interpreted_str = interpret_quote(str, *str, data);
	else if (*str == '$')
		interpreted_str = expand_env_args(str, data);
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
			len = len_next_meta_char(str + start + 1, "$?\"'", 1) + 1;
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