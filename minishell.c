/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:40:25 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/12 13:36:01 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_command(char *line, t_pipe *data)
{
	if (replace_pipes(line) == -1)
		return (-1);
	data->cmds = ft_split(line, 31);
	free(line);
	if (!data->cmds)
		return (-2);
	data->cmdc = sizeof_arraylist(data->cmds);
	g_exit_status = pipex(data);
	freeall(data->cmds);
	return (0);
}

/**
 * @return -2 if memory allocating function failed
*/
static int	minishell_command(char *argv[], t_pipe *data)
{
	char	*line;

	line = ft_strdup(argv[2]);
	if (!line)
		return (-2);
	return (run_command(line, data));
}

static int	minishell_files(int argc, char *argv[], t_pipe *data)
{
	int		i;
	int		fd;
	int		ret;
	char	*line;

	ret = 0;
	i = 0;
	while (ret == 0 && ++i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
			errormsg_exit(argv[i], -1, data);
		line = get_next_line(fd);
		while (ret == 0 && line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			ret = run_command(line, data);
			if (ret != -2)
				line = get_next_line(fd);
		}
		close(fd);
	}
	return (ret);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	data;
	int		return_value;

	data.envp = copy_arraylist(envp, 0);
	if (!data.envp)
		return (EXIT_FAILURE);
	if (argc < 2)
		return_value = minishell_prompt(&data);
	else if (ft_strnstr(argv[1], "-c", 2))
		return_value = minishell_command(argv, &data);
	else
		return_value = minishell_files(argc, argv, &data);
	freeall(data.envp);
	if (return_value == -2)
		ft_putstr_fd("bvsh: malloc error\n", 2);
	return (return_value != 0);
}
