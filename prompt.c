/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/23 12:10:52 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "prompt.h"
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
/* char	*replace_pipes(char *cmd)
{
	int		i;
	char	*new_cmd;

	new_cmd = ft_strdup(cmd);
	if (!new_cmd)
		return (NULL);
	i = 0;
	while (new_cmd[i])
	{
		if (new_cmd[i] == '|')
			new_cmd[i] = 31;
		pass_quotes(new_cmd, &i, '\"');
		pass_quotes(new_cmd, &i, '\'');
		i++;
	}
	return (new_cmd);
} */

int	prompt(char **commands)
{
	// Interactive mode
	const char		*prompt;
	char			*line;
	struct termios	new_attr;

	prompt = "bvsh-1.1$ ";

	/* Disabling echoing of input characters */
	tcgetattr(STDIN_FILENO, &new_attr);
	new_attr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);

	/* Listening to ctrl + c (interuption of the process)*/
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	while (1)
	{
		line = readline(prompt);
		/* Exit on ctrl + d */
		if (line == NULL)
		{
/* 			line = ft_strdup("exit1");
			free(line);
			printf("exit\n");
			rl_insert_text("exit2"); */

/* 			rl_replace_line("exit3", 0);
			rl_redisplay(); */

/* 			rl_on_new_line();
			rl_replace_line("\r", 0);
			printf("\r");
			ft_putstr_fd("exit", 2); */
			printf("exit\n"); 
			return (0);
		}
		if (line)
		{
			commands = ft_split(line, 31);
			add_history(line);
			free(line);
		}
	}
	printf("%s", commands[0]);
	return (0);
}

// s
