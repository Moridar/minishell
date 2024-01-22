/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 13:59:48 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/01/16 13:31:16 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>

typedef struct s_pipe
{
	int		status;
	int		fd[2][2];
	pid_t	*pid;
	int		cmdc;
	int		append;
	char	**envp;
	char	**paths;
	char	**cmds;
}	t_pipe;

void	freeall(char **strarray);
char	**make_args(char *arg);
char	*ft_getpath(char *cmd, char **paths);
void	execute(int i, t_pipe *data);

void	errormsg(char *msg, int exits);
int		get_quote_length(char *str, char quote);
void	set_direction(t_pipe *data, int i, int *fd);
char	*interpret_quote(char *str, char quote);
char	*parse_quotes(char *str, char symbol, int set_space);

int		prompt(char **commands);

char	*expand_env_args(char *str);
char	**split_shell_cmd(char	*cmd);
void	free_cmd_mem(char **command);

#endif
