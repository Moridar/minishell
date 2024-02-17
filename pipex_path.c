/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 10:13:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/17 17:58:32 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_paths(t_pipe *data, char **cmdline)
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
		msg_freeall_exit("No such file or directory\n", cmdline, 127, data);
	return (paths);
}

/**
 * returns 1 if directory
 * returns 0 if existing file
 * exits if non-existing 
*/
static char	*is_directory(char *path, char **cmdline, t_pipe *data)
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
		free(data->history_path);
		freeall(data->envp);
		freeall_exit(cmdline, 126);
	}
	cmdnfound_exit(cmdline, data);
	return (NULL);
}

static char	*find_path(char *cmd, char **cmdline,
		t_pipe *data, char **no_permission_path)
{
	char	*cmdpath;
	int		i;
	char	**paths;

	paths = get_paths(data, cmdline);
	i = -1;
	while (cmd && paths[++i])
	{
		cmdpath = ft_strjoin(paths[i], cmd);
		if (cmdpath && access(cmdpath, F_OK) == 0)
		{
			free(*no_permission_path);
			if (access(cmdpath, X_OK) == 0)
				break ;
			*no_permission_path = ft_strdup(cmdpath);
		}
		free(cmdpath);
		cmdpath = NULL;
	}
	freeall(paths);
	return (cmdpath);
}

static char	*get_path(char *cmd, char **cmdline, t_pipe *data)
{
	char	*cmdpath;
	char	*no_permission_path;

	no_permission_path = NULL;
	cmd = ft_strjoin("/", cmd);
	if (!cmd)
		msg_freeall_exit("malloc error\n",
			cmdline, 2, data);
	cmdpath = find_path(cmd, cmdline, data, &no_permission_path);
	free(cmd);
	if (cmdpath)
		return (cmdpath);
	if (no_permission_path)
		cmd_no_permission_exit(no_permission_path, cmdline, data);
	cmdnfound_exit(cmdline, data);
	return (NULL);
}

char	*check_cmdpath(char *cmd, t_pipe *data, char **cmdline)
{
	if (ft_strchr(cmd, '/') == NULL)
		return (get_path(cmd, cmdline, data));
	else if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == -1)
			cmd_no_permission_exit(cmd, cmdline, data);
		return (is_directory(cmd, cmdline, data));
	}
	msg_freeall_exit("No such file or directory\n", cmdline, 127, data);
	return (NULL);
}
