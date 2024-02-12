/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/12 15:00:04 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	g_exit_status;

/**
 *  if signo == SIGINT handles ctrl + C
 * 	else handles ctrl + \ by doing nothing
*/
static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_exit_status = 1;
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
	if (!arraylist_tmp)
		return (-2);
	data->cmds = arraylist_tmp;
	data->cmds[pipes_count] = readline(">");
	return (0);
}

static int	split_pipeline(t_pipe *data, char *line)
{
	int		pipes_count;

	data->cmds = NULL;
	pipes_count = replace_pipes(line);
	if (pipes_count < 0)
		return (free_return (line, -1));
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
	char	**cmd;
	int		return_value;

	return_value = split_pipeline(data, line);
	if (return_value == -1)
		return (0);
	if (return_value != 0)
		return (return_value);
	if (data->cmdc == 1)
	{
		cmd = split_shell_cmd(data->cmds[0], data);
		if (!cmd)
			freeall_return(data->cmds, -2);
		if (cmd[0])
			return_value = builtins(cmd, data);
		freeall(cmd);
	}
	if (return_value == 0)
		g_exit_status = pipex(data);
	freeall(data->cmds);
	if (return_value == 2)
		return (1);
	return (0);
}

int	minishell_prompt(t_pipe *data)
{
	char	*line;
	int		return_value;

	g_exit_status = 0;
	read_history_file(data);
	signal(SIGQUIT, signal_handler);
	return_value = 0;
	while (return_value == 0)
	{
		signal(SIGINT, signal_handler);
		toggle_carret(0);
		line = readline("bvsh-1.1$ ");
		if (line == NULL)
		{
			printf("bvsh-1.1$ exit\n");
			freeall_exit(data->envp, g_exit_status);
		}
		if (line && *line)
		{
			add_history(line);
			write_history_file(line, data);
			return_value = process_prompt_line(line, data);
		}
	}
	return (return_value);
}
