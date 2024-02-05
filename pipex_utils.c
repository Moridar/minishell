/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:33:37 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/05 02:36:15 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_and_close_fds(int fd[2])
{
	if (fd[0] != STDERR_FILENO)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	if (fd[1] != STDOUT_FILENO)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

void	closepipe(t_pipe *data)
{
	if (data->fd[0][0] >= 0)
		close(data->fd[0][0]);
	if (data->fd[0][1] >= 0)
		close(data->fd[0][1]);
	if (data->fd[1][0] >= 0)
		close(data->fd[1][0]);
	if (data->fd[1][1] >= 0)
		close(data->fd[1][1]);
}

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
