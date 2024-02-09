/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:40:25 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/09 11:29:01 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_command(char *line, t_pipe *data)
{
	replace_pipes(line);
	data->cmds = ft_split(line, 31);
	free(line);
	if (!data->cmds)
		return (-2);
	data->cmdc = get_string_array_size(data->cmds);
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
	if (run_command(line, data) == -2)
		return (-2);
	return (0);
}

static int	minishell_files(int argc, char *argv[], t_pipe *data)
{
	int		i;
	int		fd;
	char	*line;

	i = 0;
	while (++i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
			errormsg(argv[i], 1, -1);
		line = get_next_line(fd);
		while (line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			if (run_command(line, data) == -2)
			{
				close(fd);
				return (-2);
			}
			line = get_next_line(fd);
		}
		close(fd);
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	data;
	int		return_value;

	data.envp = copy_double_array(envp, 0);
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
