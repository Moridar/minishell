/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/12 12:07:51 by bsyvasal         ###   ########.fr       */
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
void	check_loose_pipe(t_pipe *data, int pipes_count)
{
	char	*tmp;

	tmp = data->cmds[pipes_count];
	data->cmds[pipes_count] = ft_strtrim(tmp, " \t\n\v\f\r");
	free(tmp);
	if (data->cmds[pipes_count] && data->cmds[pipes_count][0] == '\0')
	{
		free(data->cmds[pipes_count]);
		data->cmds[pipes_count] = NULL;
	}
	data->cmdc = get_string_array_size(data->cmds);
	if (pipes_count + 1 == data->cmdc)
		return ;
	data->cmds = reallocate_arraylist(data->cmds, pipes_count + 1);
	data->cmds[pipes_count] = readline(">");
	data->cmdc = pipes_count + 1;
}

void	split_pipeline(t_pipe *data, char *line)
{
	int		pipes_count;

	pipes_count = replace_pipes(line, data);
	data->cmds = ft_split(line, 31);
	free(line);
	if (!data->cmds)
		return ;
	check_loose_pipe(data, pipes_count);
}

static int	process_prompt_line(char *line, t_pipe *data)
{
	char	**cmd;
	int		builtin_return;

	builtin_return = 0;
	split_pipeline(data, line);
	if (!data->cmds)
		return (-2);
	if (data->cmdc == 1)
	{
		cmd = split_shell_cmd(data->cmds[0], data);
		if (!cmd)
			freeall_return(data->cmds, -2);
		if (cmd[0])
			builtin_return = builtins(cmd, data);
		freeall(cmd);
	}
	if (builtin_return == 0)
		g_exit_status = pipex(data);
	freeall(data->cmds);
	if (builtin_return == 2)
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
			msg_freeall_exit("bvsh-1.1$ exit\n", data->envp, g_exit_status);
		if (line && *line)
		{
			add_history(line);
			write_history_file(line, data);
			return_value = process_prompt_line(line, data);
		}
	}
	return (return_value);
}
