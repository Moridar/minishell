/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/10 01:18:49 by vshchuki         ###   ########.fr       */
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

/* Exit on ctrl + d */
static void	handle_ctrl_d(void)
{
	rl_on_new_line();
	rl_replace_line("exit\n", 0);
	rl_redisplay();
	printf("\n");
	exit(g_exit_status);
}

/**
 * Splits the line by pipes and stores the commands in the data struct.
 * Gives prompt input if the line has a loose pipe (no command after it).
 * The last command will be written from prompt commands double array.
*/
void	data_cmds_split_by_pipes(t_pipe *data, char *line)
{
	int		pipes_count;
	char	*tmp;

	pipes_count = replace_pipes(line, data);
	data->cmds = ft_split(line, 31);
	free(line);
	if (!data->cmds)
		return ;
	tmp = data->cmds[pipes_count - 1];
	data->cmds[pipes_count - 1] = ft_strtrim(tmp, " \t\n\v\f\r");
	free(tmp);
	if (data->cmds[pipes_count - 1] && data->cmds[pipes_count - 1][0] == '\0')
	{
		free(data->cmds[pipes_count - 1]);
		data->cmds[pipes_count - 1] = NULL;
	}
	data->cmdc = get_string_array_size(data->cmds);
	if (pipes_count == data->cmdc)
		return ;
	data->cmds = reallocate_arraylist(data->cmds, pipes_count);
	data->cmds[pipes_count - 1] = readline(">");
	data->cmdc = pipes_count;
}

static int	process_prompt_line(char *line, t_pipe *data)
{
	char	**cmd;
	int		builtins_res;

	builtins_res = 0;
	data_cmds_split_by_pipes(data, line);
	if (!data->cmds)
		return (-2);
	cmd = split_shell_cmd(data->cmds[0], data);
	if (!cmd)
		freeall_return(data->cmds, -2);
	if (cmd[0] && data->cmdc == 1)
		builtins_res = builtins(cmd, data);
	freeall(cmd);
	if (builtins_res == 0)
		g_exit_status = pipex(data);
	freeall(data->cmds);
	if (builtins_res == 2)
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
			handle_ctrl_d();
		if (line && *line)
		{
			add_history(line);
			write_history_file(line, data);
			return_value = process_prompt_line(line, data);
		}
	}
	return (return_value);
}
