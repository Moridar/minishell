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

int	main(int argc, char *argv[], char *envp[])
{
	int		cmdc;
	char	*cmds[4];
	char	*files[2];

	argc = 0;
	argv = NULL;
	cmdc = 4;
	cmds[0] = "ls -l";
	cmds[1] = "cat";
	cmds[2] = "echo world hello";
	cmds[3] = "cat";
	files[0] = NULL;
	files[1] = NULL;
	pipex(cmdc, cmds, files, envp);
}
