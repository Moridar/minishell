/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:33:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/01 19:14:49 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Returns the length of a quote, including the quote itself.
*/
int	get_quote_length(char *str, char quote)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i++] == quote)
			return (i);
	}
	return (1);
}

/**
 * @brief Return the length until next meta character
 * @param space sets if space is a stopper
*/
int	len_next_meta_char(char *str, char *metachars, int space)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (ft_strchr(metachars, str[i]))
			break ;
		if (space && ft_isspace(str[i]))
			break ;
		i++;
	}
	return (i);
}

/**
 * @brief Free the double string array
 * @param **char strarray
*/
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

void	freeall_exit(char **strarray, int exitno)
{
	freeall(strarray);
	exit(exitno);
}

/**
 * Count the number of char c leading the string.
*/
int	count_lead_chars(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] == c)
	{
		i++;
	}
	return (i);
}
