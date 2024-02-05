/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 10:13:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/05 14:26:22 by vshchuki         ###   ########.fr       */
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
	if (!paths)
	{
		paths = ft_split("/usr/local/bin:/usr/bin:"
				"/bin:/usr/sbin:/sbin", ':');
	}
	return (paths);
}

static void	cmdnfound_exit(char *cmd, char **cmds)
{
	char	*errmsg;
	char	*tmp;

	tmp = ft_strjoin("bvsh: ", cmd);
	errmsg = ft_strjoin(tmp, ": command not found\n");
	write(2, errmsg, ft_strlen(errmsg));
	freeall(cmds);
	free(tmp);
	free(errmsg);
	exit(127);
}

/**
 * returns 1 if directory
 * returns 0 if existing file
 * exits if non-existing 
*/
static char	*is_directory(char *path, char **cmds)
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
		freeall_exit(cmds, 126);
	}
	cmdnfound_exit(path, cmds);
	return (NULL);
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

	cmdpath = NULL;
	if (ft_strchr(cmd, '/') == NULL)
		cmdpath = get_path(cmd, data);
	else if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == -1)
			msg_freeall_exit("bvsh: Permission denied\n", cmds, 126);
		cmdpath = ft_strdup(cmd);
	}
	else
		msg_freeall_exit("bvsh: No such file or directory\n", cmds, 127);
	if (!cmdpath)
		cmdnfound_exit(cmd, cmds);
	return (is_directory(cmdpath, cmds));
}
