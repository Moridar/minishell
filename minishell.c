/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:40:25 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/25 18:47:56 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char const *argv[], char *envp[])
{
	int 	i;
	int 	fd;
	char	*line;
	t_pipe 	data;

	(void)envp;
	copy_double_array(envp, &data.envp);
	// Interactive mode
	if (argc < 2)
	{
		prompt(&data);
	}
	// Non-interactive mode
	else if (ft_strnstr(argv[1], "-c", 2))
	{
		line = ft_strdup(argv[2]);
		replace_pipes(line);
		data.cmds = ft_split(line, 31);
		data.cmdc = get_string_array_size(data.cmds);
		initialise(&data);
		pipex(&data);
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
				data.cmds = ft_split(line, 31);
				data.cmdc = get_string_array_size(data.cmds);
				initialise(&data);
				free(line);
				pipex(&data);
				freeall(data.cmds);
				line = get_next_line(fd);
			}
			close(fd);
			i++;
		}
	}
	return (0);
}