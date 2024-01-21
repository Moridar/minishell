/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 21:27:35 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/21 22:49:08 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*interpret_quote(char *str, char quote)
{
	char	*word;

	word = ft_substr(str + 1, 0, get_quote_length(str + 1, quote) - 1);
	return (word);
}

char	*parse_quotes(char *str, char symbol)
{
	char	*ret;
	int		start;
	int		end;

	ret = NULL;
	start = 0;
	while (str[start] && str[start] != symbol && !ft_isspace(str[start]))
	{
		if (str[start] == '\'' || str[start] == '"')
		{
			end = start + get_quote_length(str + start, str[start]) - 1;
			ret = ft_strjoin(ret, interpret_quote(str + start, str[start]));
		}
		else
		{
			end = start;
			while (str[end + 1] && !ft_isspace(str[end + 1]))
				end++;
			ret = ft_strjoin(ret, ft_substr(str, start, end - start + 1));
		}
		start = end + 1;
	}
	return (ret);
}
