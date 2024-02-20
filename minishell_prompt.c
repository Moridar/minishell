/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/20 11:18:40 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	g_last_signal;

/**
 *  if signo == SIGINT handles ctrl + C
 * 	else handles ctrl + \ by doing nothing
*/
static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_last_signal = SIGINT;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		rl_redisplay();
}

/**
 * Gives prompt input if the line has a loose pipe (no command after it).
 * The last command will be written from prompt commands double array.
*/
static int	check_loose_pipe(t_pipe *data, int pipes_count)
{
	char	*tmp;
	char	**arraylist_tmp;

	tmp = data->cmds[pipes_count];
	data->cmds[pipes_count] = ft_strtrim(tmp, " \t\n\v\f\r");
	free(tmp);
	if (data->cmds[pipes_count] && data->cmds[pipes_count][0] == '\0')
	{
		free(data->cmds[pipes_count]);
		data->cmds[pipes_count] = NULL;
	}
	if (data->cmds[pipes_count])
		return (0);
	arraylist_tmp = copy_arraylist(data->cmds, 1);
	freeall(data->cmds);
	data->cmds = arraylist_tmp;
	if (!arraylist_tmp)
		return (-2);
	data->cmds[pipes_count] = readline(">");
	return (0);
}

static int	split_pipeline(t_pipe *data, char *line)
{
	int		pipes_count;

	pipes_count = replace_pipes(line);
	if (pipes_count < 0)
		return (free_return (line, 2));
	data->cmds = ft_split(line, 31);
	free(line);
	if (!data->cmds)
		return (-2);
	if (pipes_count > 0)
		if (check_loose_pipe(data, pipes_count) == -2)
			return (-2);
	data->cmdc = sizeof_arraylist(data->cmds);
	return (0);
}

static int	process_prompt_line(char *line, t_pipe *data)
{
	int		return_value;

	return_value = split_pipeline(data, line);
	if (return_value != 0)
		return (return_value);
	return_value = pipex(data);
	freeall(data->cmds);
	data->cmds = NULL;
	return (return_value);
}

void	minishell_prompt(t_pipe *data)
{
	char	*line;

	read_history_file(data);
	signal(SIGQUIT, signal_handler);
	while (data->exit_status >= 0)
	{
		signal(SIGINT, signal_handler);
		toggle_carret(0);
		line = readline("bvsh-1.1$ ");
		if (line == NULL)
		{
			ft_printf("bvsh-1.1$ exit\n");
			freeall(data->cmds);
			break ;
		}
		if (line && *line)
		{
			data->exit_status = g_last_signal != 0;
			g_last_signal = 0;
			add_history(line);
			write_history_file(line, data);
			data->exit_status = process_prompt_line(line, data);
		}
	}
}
