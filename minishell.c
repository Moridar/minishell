/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:40:25 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/09 01:51:12 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @return -2 if memory allocating function failed
*/
static int	minishell_command(char *argv[], t_pipe *data)
{
	char	*line;

	line = ft_strdup(argv[2]);
	if (!line)
		return (-2);
	replace_pipes(line);
	data->cmds = ft_split(line, 31);
	if (!data->cmds)
		return (-2);
	free(line);
	data->cmdc = get_string_array_size(data->cmds);
	g_exit_status = pipex(data);
	return (0);
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
		line = get_next_line(fd); // Should we check get next line for memory fail? End of fail returns NULL and malloc fail returns NULL? How to distinguish these two cases?
		while (line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			replace_pipes(line);
			data->cmds = ft_split(line, 31);
			if (!data->cmds)
			{
				free(line);
				freeall_exit(data->envp, EXIT_FAILURE);
			}
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
	if (!data.envp)
		return (EXIT_FAILURE);
	if (argc < 2)
		minishell_prompt(&data);
	else if (ft_strnstr(argv[1], "-c", 2) && minishell_command(argv, &data) == -2)
		freeall_exit(data.envp, EXIT_FAILURE);
	else
		minishell_files(argc, argv, &data);
	return (EXIT_SUCCESS);
}
