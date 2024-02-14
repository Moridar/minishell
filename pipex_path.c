/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 10:13:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/14 16:47:02 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (paths);
}

static void	cmdnfound_exit(char *cmd, char **cmds, t_pipe *data)
{
	ft_putstr_fd("bvsh: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	freeall(cmds);
	freeall(data->envp);
	exit(127);
}

/**
 * returns 1 if directory
 * returns 0 if existing file
 * exits if non-existing 
*/
static char	*is_directory(char *path, char **cmds, t_pipe *data)
{
	DIR		*dir;

	if (!path)
		return (NULL);
	dir = opendir(path);
	if (!dir)
		return (path);
	closedir(dir);
	if (ft_strchr(path, '/') != NULL)
	{
		ft_putstr_fd("bvsh: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		freeall(data->envp);
		freeall_exit(cmds, 126);
	}
	cmdnfound_exit(path, cmds, data);
	return (NULL);
}

static char	*get_path(char *cmd, char *cmdline, t_pipe *data)
{
	int		i;
	char	*cmdpath;
	char	**paths;
	char	*tmp;

	paths = get_paths(data);
	if (!paths)
		cmdnfound_exit(cmd, cmdline, data);
	tmp = cmd;
	cmd = ft_strjoin("/", cmd);
	i = -1;
	while (paths[++i])
	{
		cmdpath = ft_strjoin(paths[i], cmd);
		if (access(cmdpath, X_OK) == 0)
			break ;
		free(cmdpath);
		cmdpath = NULL;
	}
	freeall(paths);
	free(cmd);
	if (!cmdpath)
		cmdnfound_exit(cmd, cmdline, data);
	return (cmdpath);
}

char	*check_cmdpath(char *cmd, t_pipe *data, char **cmdline)
{
	char	*cmdpath;

	cmdpath = NULL;
	if (ft_strchr(cmd, '/') == NULL)
		return (get_path(cmd, cmdline, data));
	else if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == -1)
		{
			freeall(data->envp);
			msg_freeall_exit("bvsh: Permission denied\n", cmdline, 126);
		}
		cmdpath = ft_strdup(cmd);
	}
	else
	{
		freeall(data->envp);
		msg_freeall_exit("bvsh: No such file or directory\n", cmdline, 127);
	}
	if (!cmdpath)
		cmdnfound_exit(cmd, cmdline, data);
	return (is_directory(cmdpath, cmdline, data));
}
