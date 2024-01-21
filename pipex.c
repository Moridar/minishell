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

void	set_paths(t_pipe *data)
{
	int	i;

	i = 0;
	while (data->envp && data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PATH=", 5) == 0)
			data->paths = ft_split((data->envp[i]) + 5, ':');
		i++;
	}
	if (!data->paths)
		data->paths = ft_split("/usr/local/bin:/usr/bin:"
				"/bin:/usr/sbin:/sbin", ':');
}

void	initalise(int cmdc, char *cmds[], t_pipe *data, char *envp[])
{
	data->cmds = cmds;
	data->cmdc = cmdc;
	data->envp = envp;
	data->status = 0;
	set_paths(data);
	data->pid = ft_calloc(sizeof(pid_t), cmdc);
	data->fd[0][0] = -1;
	data->fd[0][1] = -1;
	data->fd[1][0] = -1;
	data->fd[1][1] = -1;
}

int	pipex(int cmdc, char *cmds[], char *envp[])
{
	t_pipe	data;
	int		i;

	initalise(cmdc, cmds, &data, envp);
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
	char	*cmds[8];
	char	**commands;

	if (argc > 10 && argv)
		ft_printf("pipex: %d\n", argc);
	cmdc = 7;
	cmds[0] = ft_strdup("echo \"'a'\" >out1");
	cmds[1] = ft_strdup("<out cat >>out1");
	cmds[2] = ft_strdup("<out1 cat");
	cmds[3] = ft_strdup("wc");
	cmds[4] = ft_strdup("cat >>out1");
	cmds[5] = ft_strdup("echo hello world >> out1");
	cmds[6] = ft_strdup("<out1 cat");
	pipex(cmdc, cmds, envp);
	exit(0);
}
