/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/23 12:04:55 by vshchuki         ###   ########.fr       */
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
			commands = ft_split(line, '|');
			add_history(line);
			free(line);
		}
	}
	printf("%s", commands[0]);
	return (0);
}

// s
