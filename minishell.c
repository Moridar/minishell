/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:40:25 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/02 01:32:27 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	minishell_command(char *argv[], t_pipe *data)
{
	char	*line;

	line = ft_strdup(argv[2]);
	replace_pipes(line);
	data->cmds = ft_split(line, 31);
	data->cmdc = get_string_array_size(data->cmds);
	g_exit_status = pipex(data);
}

static void	minishell_files(int argc, char *argv[], t_pipe *data)
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
			replace_pipes(line);
			data->cmds = ft_split(line, 31);
			data->cmdc = get_string_array_size(data->cmds);
			free(line);
			g_exit_status = pipex(data);
			freeall(data->cmds);
			line = get_next_line(fd);
		}
		close(fd);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	data;

	data.envp = copy_double_array(envp, 0);
	if (argc < 2)
		minishell_prompt(&data);
	else if (ft_strnstr(argv[1], "-c", 2))
		minishell_command(argv, &data);
	else
		minishell_files(argc, argv, &data);
	return (0);
}
