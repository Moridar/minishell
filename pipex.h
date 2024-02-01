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
# include <signal.h>
# include <dirent.h>

extern unsigned char	g_exit_status;

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
char	*interpret_quote(char *str, char quote, t_pipe *data);
int		len_next_meta_char(char *str, char *metachars, int space);
void	replace_pipes(char *cmd);
void	free_env_exit(t_pipe *data, int exitno);
int		free_return(void *ptr, int returnvalue);
void	freeall_exit(char **strarray, int exitno);
void	closepipe(t_pipe *data);
void	dup_and_close_fds(int fd[2]);

//Main functions
int		minishell_prompt(t_pipe *data);
int		pipex(t_pipe	*data);

void	toggle_carret(int is_on);
void	execute(int i, t_pipe *data);
void	set_direction(t_pipe *data, int i, int *fd);
char	**split_shell_cmd(char	*cmd, t_pipe *data);
char	*check_cmdpath(char *cmd, t_pipe *data, char **cmds);
char	*interpret(char *str, t_pipe *data);
char	*expand_env_args(char *str, t_pipe *data);

// Array utils
int		get_string_array_size(char **str);
char	**free_double_arr(char **arr, int i);
char	**copy_double_array(char **arr1, int increase_size);
char	**reallocate_arraylist(char **arr, int size);

// History
int		write_history_file(char *line);
int		read_history_file(void);
int		history(void);

// Builtins
void	env(t_pipe *data);
int		builtins(char **cmd, t_pipe *data, char *line);
int		child_builtins(char **cmd, t_pipe *data);
int		export_var(t_pipe *data, char *var);
int		exit_builtin(char *status, t_pipe *data, char **cmd, char *line);

#endif
