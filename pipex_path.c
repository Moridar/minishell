/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 10:13:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/01 19:20:45 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**get_paths(t_pipe *data)
{
	int		i;
	char	**paths;

	i = 0;
	paths = NULL;
	while (data->envp && data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PATH=", 5) == 0)
			paths = ft_split((data->envp[i]) + 5, ':');
		i++;
	}
	if (!paths)
	{
		paths = ft_split("/usr/local/bin:/usr/bin:"
				"/bin:/usr/sbin:/sbin", ':');
	}
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

static char	*is_not_directory(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		write(2, "bvsh: ", 6);
		write(2, path, ft_strlen(path));
		write(2, " is a directory\n", 16);
		free(path);
		return (NULL);
	}
	return (path);
}

char	*get_path(char *cmd, t_pipe *data)
{
	int		i;
	char	*cmdpath;
	char	**paths;

	if (access(cmd, F_OK) == 0)
		return (is_not_directory(cmd));
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
	return (is_not_directory(cmdpath));
}
