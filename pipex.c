/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/16 13:31:08 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	initalise(int cmdc, char *cmds[], char *files[], t_pipe *data)
{
	data->cmds = cmds;
	data->cmdc = cmdc;
	data->infile = files[0];
	data->outfile = files[1];
	data->status = 0;
	while (data->envp && *data->envp && ft_strncmp(*data->envp, "PATH=", 5))
		data->envp++;
	if (data->envp && *data->envp)
		data->paths = ft_split((*data->envp) + 5, ':');
	data->pid = ft_calloc(sizeof(pid_t), (cmdc - 1));
}

int	pipex(int cmdc, char *cmds[], char *files[], char *envp[])
{
	t_pipe	data;
	int		i;

	data.envp = envp;
	initalise(cmdc, cmds, files, &data);
	i = -1;
	while (++i < cmdc)
		execute(i, &data);
	free(data.pid);
	freeall(data.paths);
	return (WEXITSTATUS(data.status));
}

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	} else {
		rl_redisplay();
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int		cmdc;
	char	*cmds[4];
	char	*files[2];

	if (argc && argv)
		ft_printf("pipex: %d\n", argc);
	cmdc = 4;
	cmds[0] = "cat";
	cmds[1] = "cat";
	cmds[2] = "cat";
	cmds[3] = "cat";
	// files[0] = NULL;
	// files[1] = NULL;
	files[0] = "Makefile";
	files[1] = "out";

	(void)envp;

	// Interactive mode
	const char *prompt;
	char *line;
	struct termios new_attr;
	struct termios old_attr;

	prompt = "bvsh-1.1$ ";

	/* Disabling echoing of input characters */
	tcgetattr(STDIN_FILENO, &old_attr);
	new_attr = old_attr;
	new_attr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);

	/* Listening to control+c (interuption of the process)*/
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
		{
			// line = ft_strdup("exit1");
			// free(line);
			// printf("exit\n");
			// rl_insert_text("exit2");

			rl_replace_line("exit3", 0);
			rl_redisplay();

			// rl_on_new_line();

			// rl_replace_line("\r", 0);
			// printf("\r");
			// ft_putstr_fd("exit", 2);
			// printf("exit\n");
			return (0);
		}
		/* Handle CTRL + D */
		if (line)
		{
			// ft_printf("%s\n", line);
			add_history(line);
			free(line);
		}
	}

	// while (read(0, buf, 1)) {
	// 	write(1, &buf, 1);
	// }
	// if (!ft_strncmp(cmds[0], "history", ft_strlen(cmds[0])))
	// {
	// 	ft_printf("history print!\n");
	// }
	// else
	// {
	pipex(cmdc, cmds, files, envp);

	// }
	return (0);
}
