/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/27 01:47:31 by bsyvasal         ###   ########.fr       */
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

/* void	initalise(int cmdc, char *cmds[], t_pipe *data, char *envp[])
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
} */

int	pipex(t_pipe	*data)
{
	int		i;

	i = -1;
	while (++i < data->cmdc)
		execute(i, data);
	free(data->pid);
	freeall(data->paths);
	return (WEXITSTATUS(data->status));
}

/* int	main(int argc, char *argv[], char *envp[])
{
	int		cmdc;
	char	*cmds[8];
	char	**commands;

	if (argc > 10 && argv)
		ft_printf("pipex: %d\n", argc);
	cmdc = 2;
	cmds[0] = ft_strdup("<< END cat");
	cmds[1] = ft_strdup("cat");
	pipex(cmdc, cmds, envp);
	exit(0);
}*/
