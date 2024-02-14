/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 02:36:51 by vshchuki          #+#    #+#             */
/*   Updated: 2024/02/14 17:43:43 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
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
# include <string.h>

extern unsigned char	g_last_signal;

typedef struct s_pipe
{
	int		exit_status;
	int		status;
	int		fd[2][2];
	pid_t	*pid;
	int		cmdc;
	char	**envp;
	char	**cmds;
}	t_pipe;

//Helper functions
int		count_lead_chars(char *str, char c);
int		get_quote_length(char *str, char quote);
int		len_next_meta_char(char *str, char *metachars, int space);
int		replace_pipes(char *cmd);

//Redirection helpers
void	redirect_error_exit(char *errmsg, int *fd, t_pipe *data);
void	free_filenames(char *infilename, char *outfilename);
int		openfile(char *filename, char symbol, int type, t_pipe *data);

//Error and exit handling
void	freeall(char **strarray);
void	freeall_exit(char **strarray, int exitno);
int		freeall_return(char **strarray, int return_value);
int		free_return(void *ptr, int returnvalue);
void	*free_return_null(void *ptr);
void	msg_freeall_exit(char *msg, char **strarray, int exitno);
void	errormsg_exit(char *msg, int exit_status, t_pipe *data);
void	closepipe(t_pipe *data);
void	dup_and_close_fds(int fd[2]);
void	cmd_no_permission_exit(char *path, char **cmdline, t_pipe *data);
void	cmdnfound_exit(char **cmdline, t_pipe *data);

//Main functions
void	minishell_prompt(t_pipe *data);
int		pipex(t_pipe *data);

void	toggle_carret(int is_on);
void	execute(int i, t_pipe *data);
void	set_direction(t_pipe *data, int i, int *fd);
char	**split_shell_cmd(char	*cmd, t_pipe *data);
char	*check_cmdpath(char *cmd, t_pipe *data, char **cmds);
char	*interpret(char *str, t_pipe *data);
char	*expand_env_args(char *str, t_pipe *data);

// Array utils
int		sizeof_arraylist(char **str);
char	**copy_arraylist(char **arr1, int increase_size);
char	**reallocate_arraylist(char **arr, int size);

// History
int		write_history_file(char *line, t_pipe *data);
int		read_history_file(t_pipe *data);

// Builtins
int		builtins(char **cmd, t_pipe *data);
int		child_builtins(char **cmd, t_pipe *data);
int		exit_builtin(char **cmd, t_pipe *data, int argc);

#endif
