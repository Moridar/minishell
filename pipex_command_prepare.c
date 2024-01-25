/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_command_prepare.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 10:13:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/25 13:25:31 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	cmdnfound_exit(char *cmd)
{
	char	*errmsg;

	errmsg = ft_strjoin("pipex: ", cmd);
	errmsg = ft_strjoin(errmsg, ": command not found\n");
	write(2, errmsg, ft_strlen(errmsg));
	exit(127);
}

char	*ft_getpath(char *cmd, char **paths)
{
	int		i;
	char	*cmdpath;

	if (access(cmd, F_OK) == 0)
		return (cmd);
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
	free(paths);
	if (!cmdpath)
		cmdnfound_exit(cmd + 1);
	free(cmd);
	return (cmdpath);
}

char	*remove_quote(char *arg, char c)
{
	char	*str;
	char	*strend;
	int		i;

	str = ft_strchr(arg, c) + 1;
	strend = str;
	i = ft_strlen(str);
	while (i >= 0)
	{
		if (str[i] == '\\')
			str[i] = 32;
		if (str[i - 1] == '\\')
			i--;
		i--;
	}
	while (1)
	{
		strend = ft_strchr(strend + 1, c);
		if (!strend)
			return (NULL);
		if (*(strend - 1) != '\\')
			break ;
	}
	*strend = 0;
	return (str);
}

char	**make_args(char *arg)
{
	char	**args;
	int		i;
	char	c;

	if (access(arg, F_OK) == 0)
		return (ft_split(arg, 0));
	args = ft_split(arg, ' ');
	if (!args)
		exit(1);
	c = 0;
	if (args[1] && (*args[1] == '\'' || *args[1] == '"'))
		c = *args[1];
	if (c)
	{
		free(args[1]);
		args[1] = remove_quote(arg, c);
		i = 2;
		while (args[i])
			free(args[i++]);
		args[2] = 0;
	}
	return (args);
}
