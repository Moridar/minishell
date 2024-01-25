/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 15:27:11 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/25 13:35:02 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @return size of the double dimension string array, for example, created by ft_split().
*/
int	get_string_array_size(char **str)
{
	int	j;

	j = 0;
	while (str[j])
	{
		j++;
	}
	return (j);
}

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

int	prompt(char *envp[])
{
	const char		*prompt;
	char			*line;
	struct termios	new_attr;
	char 			**cmds;

	(void)envp;
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
			replace_pipes(line);
			cmds = ft_split(line, 31);
			pipex(get_string_array_size(cmds), cmds, envp);
			add_history(line);
			free(line);
		}
	}
	printf("%s", cmds[0]);
	return (0);
}

int	main(int argc, char const *argv[], char *envp[])
{
	int 	i;
	int 	fd;
	char 	**cmds;
	char	*line;

	(void)envp;
	// Interactive mode
	if (argc < 2)
	{
		prompt(envp);
	}
	// Non-interactive mode
	else if (ft_strnstr(argv[1], "-c", 2))
	{
		cmds = ft_split(argv[2], '\0');
		pipex(1, cmds, envp);
	}
	else // handles scrip1.sh script2.sh. // works for several files but still not working for several lines in one .sh file
	{
		i = 1;
		while (i < argc)
		{
			fd = open(argv[i], O_RDONLY);
			// Add handling error here. If not fd return bash: filename: No such file or directory
			line = get_next_line(fd);
			// read line by line
			while(line)
			{
				if (line[ft_strlen(line) - 1] == '\n')
					line[ft_strlen(line) - 1] = 0;
				replace_pipes(line);
				cmds = ft_split(line, 31);
				free(line);
				pipex(get_string_array_size(cmds), cmds, envp);
				freeall(cmds);
				line = get_next_line(fd);
			}
			close(fd);
			i++;
		}
	}
	return (0);
}
