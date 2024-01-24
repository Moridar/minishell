/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_interpret.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 21:27:35 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/24 15:55:02 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*interpret_simple(char *str)
{
	char	*word;

	word = expand_env_args(str);
	return (word);
}

char	*interpret_double_quote(char *str)
{
	char	*word;

	word = expand_env_args(str);
	return (word);
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
	char	quote;
	char	*interpreted_str;

	quote = *str;
	if (quote == '"' || quote == '\'')
		interpreted_str = interpret_quote(str, quote);
	else
		interpreted_str = interpret_simple(str);
	free(str);
	str = ft_strjoin(ret, interpreted_str);
	free(ret);
	free(interpreted_str);
	return (str);
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
			len = start + get_quote_length(str + start, str[start]);
		else
			while (str[start + len] && str[start + len] != '\''
				&& str[start + len] != '"' && !ft_isspace(str[start + len]))
				len++;
		ret = interpret_and_join(ret, ft_substr(str, start, len));
		start += len + 1;
	}
	return (ret);
}

// char	*interpret(char *str)
// {
// 	char	*ret;
// 	int		start;
// 	int		end;

// 	ret = NULL;
// 	start = 0;
// 	while (str[start] && !ft_isspace(str[start]))
// 	{
// 		if (str[start] == '\'' || str[start] == '"')
// 		{
// 			end = start + get_quote_length(str + start, str[start]);
// 			ret = ft_strjoin(ret, interpret_quote(str + start, str[start]));
// 		}
// 		else
// 		{
// 			end = start;
// 			while (str[end + 1] && str[end + 1] != '\''
// 				&& str[end + 1] != '"' && !ft_isspace(str[end + 1]))
// 				end++;
// 			ret = ft_strjoin(ret,
// 					interpret_simple(ft_substr(str, start, end - start + 1)));
// 		}
// 		start = end + 1;
// 	}
// 	return (ret);
// }

/* int	main(void)
{
	// char	*str = "he\"ll\"o_$SHELL";
	// char	*str = "$SHELL_hello";
	// char	*str = "$SHELL\"_hello\"";
	// char	*str = "$SHELL'_hello'";
	// printf("|%s|\n", interpret(str));
	printf("|%s|\n", cut_filename("< Makefile>out echo helo world > output"));

	return (0);
} */