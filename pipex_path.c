/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 10:13:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/01 22:04:06 by bsyvasal         ###   ########.fr       */
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
}

/**
 * returns 1 if directory
 * returns 0 if existing file
 * exits if non-existing 
*/
static int	is_directory(char *path)
{
	DIR	*dir;

	if (!path)
		return (0);
	dir = opendir(path);
	if (dir && ft_strchr(path, '/') != NULL)
	{
		closedir(dir);
		write(2, "bvsh: ", 6);
		write(2, path, ft_strlen(path));
		write(2, " Is a directory\n", 16);
		return (1);
	}
	if (dir)
	{
		closedir(dir);
		return (2);
	}
	return (0);
}

static char	*get_path(char *cmd, t_pipe *data)
{
	int		i;
	char	*cmdpath;
	char	**paths;

	paths = get_paths(data);
	cmd = ft_strjoin("/", cmd);
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
	free(cmd);
	return (cmdpath);
}

char	*check_cmdpath(char *cmd, t_pipe *data, char **cmds)
{
	char	*cmdpath;
	int		is_dir;

	if (access(cmd, X_OK) == 0)
		cmdpath = cmd;
	else if (ft_strchr(cmd, '/') == NULL)
		cmdpath = get_path(cmd, data);
	else
	{
		freeall(cmds);
		write(2, "bvsh: No such file or directory\n", 33);
		exit(127);
	}
	is_dir = 0;
	if (cmdpath)
		is_dir = is_directory(cmdpath);
	if (is_dir == 1)
		freeall_exit(cmds, 126);
	if (is_dir == 2 || !cmdpath)
	{
		cmdnfound_exit(cmd);
		freeall(cmds);
		exit(127);
	}
	return (cmdpath);
}
