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

//Helper functions
void	freeall(char **strarray);
void	errormsg(char *msg, int exits);
void	closepipe(t_pipe *data);
int		count_lead_chars(char *str, char c);
int		get_quote_length(char *str, char quote);
char	*interpret_quote(char *str, char quote);
char	*interpret(char *str);
int		len_next_meta_char(char *str, char *metachars, int space);

// Init
void	set_paths(t_pipe *data);
void	initialise(t_pipe *data);

void	replace_pipes(char *cmd);

//Main functions
int		prompt(t_pipe *data);
void	execute(int i, t_pipe *data);
void	set_direction(t_pipe *data, int i, int *fd);
char	**make_args(char *arg);
char	*ft_getpath(char *cmd, char **paths);

int		pipex(t_pipe	*data);

char	*expand_env_args(char *str);
void	pass_quotes(char *new_cmd, int *i, char quote);
char	**split_shell_cmd(char	*cmd);
void	free_cmd_mem(char **command);

// Array utils
int		get_string_array_size(char **str);
void	copy_double_array(char **arr1, char ***arr2);

// History
int		write_history_file(char *line);
int		read_history_file(void);
int		history(void);

// Builtins
// void	exit_builtin(int status);
void	env(t_pipe *data);
int 	builtins(char **cmd, t_pipe *data);
int		child_builtins(char **cmd, t_pipe *data);
int		export_var(t_pipe *data, char *var);

#endif
