/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/30 22:23:40 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

unsigned char g_exit_status;

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
 * Replaces all pipe char (|) inside prompt line to a special char (31) for
 * the split avoiding pipe chars (|) inside quotes stay unaffected.
*/
void	replace_pipes(char *cmd)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '|')
			cmd[i] = 31;
		if ((cmd[i] == '\'' || cmd[i] == '"')
			&& get_quote_length(&cmd[i], cmd[i]) != 1)
			i += get_quote_length(&cmd[i], cmd[i]) - 1;
		i++;
	}
}

/* Exit on ctrl + d */
void	handle_ctrl_d(void)
{
	rl_on_new_line();
	rl_replace_line("exit\n", 0);
	rl_redisplay();
	printf("\n");
	exit(0);
}

void	process_prompt_line(char *line, t_pipe *data)
{
	char	**cmd;
	int		builtins_res;

	builtins_res = 0;
	add_history(line);
	write_history_file(line);
	replace_pipes(line);
	data->cmds = ft_split(line, 31);
	data->cmdc = get_string_array_size(data->cmds);
	cmd = split_shell_cmd(data->cmds[0], data);
	if (cmd[0] && data->cmdc == 1)
		builtins_res = builtins(cmd, data, line);
	if (builtins_res == 0)
		g_exit_status = pipex(data);
	freeall(data->cmds);
	freeall(cmd);
	free(line);
	if (builtins_res == 2)
		free_env_exit(data, 1);
}

/**
 * Toggles carret character (^C, ^D, ^\) which are shown by default when
 * ctrl+c, ctrl+d, ctrl+\ are pressed.
 * is_on = 0 for prompt
 * is_on = 1 during the command execution
 * @param is_on 1 for removing carret characters from displayin in the shell
 * 0 to hide carret characters from shell
*/
void	toggle_carret(int is_on)
{
	struct termios	new_attr;

	tcgetattr(STDIN_FILENO, &new_attr);
	if (!is_on)
		new_attr.c_lflag &= ~ECHOCTL;
	else
		new_attr.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
}

int	minishell_prompt(t_pipe *data)
{
	char			*line;

	g_exit_status = 0;
	read_history_file();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	while (1)
	{
		toggle_carret(0);
		line = readline("bvsh-1.1$ ");
		if (line == NULL)
			handle_ctrl_d();
		if (line && *line)
			process_prompt_line(line, data);
	}
	return (0);
}

/* int	main(int argc, char const *argv[], char *envp[])
{
	t_pipe 	data;

	(void)argc;
	(void)argv;
	printf("hello!");
	copy_double_array(envp, &data.envp, 0);
	printf("%s\n", data.envp[0]);
	return (0);
} */

//bash -c "echo $SHELL\"helloworld\""
//Output: |/bin/zshhelloworld|
//./pipex -c "echo $SHELL\"helloworld\""
//Output: /bin/zsh"helloworld"