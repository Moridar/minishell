/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:40:25 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/14 19:31:58 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_command(char *line, t_pipe *data)
{
	if (replace_pipes(line) == -1)
	{
		data->exit_status = 2;
		free(line);
		return ;
	}
	data->cmds = ft_split(line, 31);
	free(line);
	if (!data->cmds)
	{
		data->exit_status = -2;
		return ;
	}
	data->cmdc = sizeof_arraylist(data->cmds);
	data->exit_status = pipex(data);
	freeall(data->cmds);
}

/**
 * @return -2 if memory allocating function failed
*/
static void	minishell_command(char *argv[], t_pipe *data)
{
	char	*line;

	line = ft_strdup(argv[2]);
	if (!line)
	{
		data->exit_status = -2;
		return ;
	}
	run_command(line, data);
}

static void	minishell_files(int argc, char *argv[], t_pipe *data)
{
	int		i;
	int		fd;
	char	*line;

	i = 0;
	while (data->exit_status != -2 && ++i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
			errormsg_exit(argv[i], -1, data);
		line = get_next_line(fd);
		while (data->exit_status != -2 && line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			run_command(line, data);
			if (data->exit_status != -2)
				line = get_next_line(fd);
		}
		close(fd);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	data;

	data.exit_status = 0;
	data.envp = copy_arraylist(envp, 0);
	data.history_path = interpret("~/.bvsh_history", &data);
	unset_var(&data, "OLDPWD");
	if (!data.envp)
		return (EXIT_FAILURE);
	if (argc < 2)
		minishell_prompt(&data);
	else if (ft_strnstr(argv[1], "-c", 2))
		minishell_command(argv, &data);
	else
		minishell_files(argc, argv, &data);
	free(data.history_path);
	freeall(data.envp);
	if (data.exit_status == -2)
		ft_putstr_fd("bvsh: malloc error\n", 2);
	if (data.exit_status < 0)
		return (EXIT_FAILURE);
	if (data.exit_status == 0)
		return (g_last_signal);
	return (data.exit_status);
}
