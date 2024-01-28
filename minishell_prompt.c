/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/28 02:16:22 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 *  if signo == SIGINT handles ctrl + C
 * 	else handles ctrl + \ by doing nothing
*/
static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
	else
	{
		rl_redisplay();
	}
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

int	prompt(t_pipe *data)
{
	const char		*prompt_message;
	char			*line;
	struct termios	new_attr;
	char			**cmd;

	prompt_message = "bvsh-1.1$ ";

	/* Reading history*/
	read_history_file();
	
	/* Disabling echoing of input characters */
	tcgetattr(STDIN_FILENO, &new_attr);
	new_attr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);

	/* Listening to ctrl + c (interuption of the process)*/
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	while (1)
	{
		line = readline(prompt_message);
		/* Exit on ctrl + d */
		if (line == NULL)
		{
			ft_printf("line == null\n");
			// only works the right way with forbidden function
/* 			rl_replace_line("exit3", 0);
			rl_forced_update_display(); */

			//creates a new prompt line:
 			rl_on_new_line();
            rl_replace_line("exit\n", 0);
            rl_redisplay();
			printf("\n");
			//free(line);
			exit(0);
		}
		if (line && *line)
		{
			add_history(line);
			write_history_file(line);
			replace_pipes(line);
			data->cmds = ft_split(line, 31);
			data->cmdc = get_string_array_size(data->cmds);
			cmd = split_shell_cmd(data->cmds[0]);
			if (cmd[0])
				if (!builtins(cmd, data))
					pipex(data);
			freeall(data->cmds);
			freeall(cmd);
			free(line);
		}
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