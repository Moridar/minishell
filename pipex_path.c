/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_command_prepare.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 10:13:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/28 02:44:00 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**get_paths(t_pipe *data)
{
	int		i;
	char	**paths;

	i = 0;
	while (data->envp && data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PATH=", 5) == 0)
			paths = ft_split((data->envp[i]) + 5, ':');
		i++;
	}
	if (!paths)
		paths = ft_split("/usr/local/bin:/usr/bin:"
				"/bin:/usr/sbin:/sbin", ':');
	return (paths);
}

static void	cmdnfound_exit(char *cmd)
{
	char	*errmsg;
	char	*tmp;

	tmp = ft_strjoin("bvsh: ", cmd);
	errmsg = ft_strjoin(tmp, ": command not found\n");
	write(2, errmsg, ft_strlen(errmsg));
	free(tmp);
	free(errmsg);
	exit(127);
}

char	*ft_getpath(char *cmd, t_pipe *data)
{
	int		i;
	char	*cmdpath;
	char	**paths;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	paths = get_paths(data);
	cmdpath = cmd;
	cmd = ft_strjoin("/", cmd);
	free(cmdpath);
	i = -1;
	while (paths[++i])
	{
		cmdpath = ft_strjoin(paths[i], cmd);
		if (access(cmdpath, F_OK) == 0)
			break ;
		free(cmdpath);
		cmdpath = NULL;
	}
	freeall(paths);
	if (!cmdpath)
		cmdnfound_exit(cmd + 1);
	free(cmd);
	return (cmdpath);
}
