/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:33:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/25 11:45:25 by bsyvasal         ###   ########.fr       */
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
*/
int	len_next_meta_char(char *str, char *metachars)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (ft_strchr(metachars, str[i]))
			break ;
		if (ft_isspace(str[i]))
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
/**
 * @brief Print error message and exits
 * @brief Msg will be formatted: 'pipex: @msg: @stderrmsg. 
 * @brief Exits if not set 0.
 * @param msg and exits
*/
void	errormsg(char *msg, int exits)
{
	msg = ft_strjoin("pipex: ", msg);
	perror(msg);
	free(msg);
	if (exits)
		exit(errno);
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
