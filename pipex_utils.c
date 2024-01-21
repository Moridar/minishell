/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:33:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/21 16:44:55 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	freeall(char **strarray)
{
	int	i;

	if (strarray)
	{
		i = -1;
		while (strarray[++i])
			free(strarray[i]);
		free(strarray);
	}
}

void	errormsg(char *msg, int exits)
{
	msg = ft_strjoin("pipex: ", msg);
	perror(msg);
	free(msg);
	if (exits)
		exit(errno);
}

int	get_quote_length(char *str, char quote)
{
	int	i;

	i = 0;
	while (str[++i])
	{
		if (str[i] == quote)
			return (i + 1);
	}
	errormsg("unclosed quote", 1);
	return (0);
}

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
