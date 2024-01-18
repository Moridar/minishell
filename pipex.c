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

void sigint_handler(int signo)
{
	(void)signo;

    // Reprint the prompt
    // rl_forced_update_display();
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char *argv[], char *envp[])
{
	int		cmdc;
	// char	*cmds[4];
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

	prompt = "bvsh-1.0$ ";
	// char	buf[1];
	char *line;
	
	signal(SIGINT, sigint_handler);
	while (1)
	{
		rl_variable_bind("echo-control-characters", "off");
		line = readline(prompt);
		if (line == NULL)
		{
			free(line);
			// rl_replace_line("exit", 0);
			printf("exit");
			return (0);
		}
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
