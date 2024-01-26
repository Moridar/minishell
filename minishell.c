/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:40:25 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/26 11:56:57 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	non_int_with_command(char *argv[], t_pipe *data)
{
	char	*line;

	line = ft_strdup(argv[2]);
	replace_pipes(line);
	data->cmds = ft_split(line, 31);
	data->cmdc = get_string_array_size(data->cmds);
	initialise(data);
	pipex(data);
}

void	non_int_with_files(int argc, char *argv[], t_pipe *data)
{
	int 	i;
	int 	fd;
	char	*line;

	i = 1;
	while (i < argc)
	{
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
			errormsg(argv[i], 1);
		line = get_next_line(fd);
		while(line)
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = 0;
			replace_pipes(line);
			data->cmds = ft_split(line, 31);
			data->cmdc = get_string_array_size(data->cmds);
			initialise(data);
			free(line);
			pipex(data);
			freeall(data->cmds);
			line = get_next_line(fd);
		}
		close(fd);
		i++;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe 	data;

	data.envp = NULL;
	copy_double_array(envp, &data.envp);
	// Interactive mode
	if (argc < 2)
		prompt(&data);
	// Non-interactive mode
	else if (ft_strnstr(argv[1], "-c", 2))
	{
		non_int_with_command(argv, &data);
	}
	else // handles scrip1.sh script2.sh. // works for several files but still not working for several lines in one .sh file
		non_int_with_files(argc, argv, &data);
	return (0);
}