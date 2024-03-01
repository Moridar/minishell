/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:38:30 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/03/01 11:39:56 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Toggles carret character (^C, ^D, ^\) which are shown by default when
 * ctrl+c, ctrl+d, ctrl+\ are pressed.
 * is_on = 0 for prompt
 * is_on = 1 during the command execution
 * @param is_on 1 for enable carret characters from displayin in the shell
 * 0 to disable carret characters from shell
 */
static void	toggle_carret(int is_on)
{
	struct termios	new_attr;

	tcgetattr(STDIN_FILENO, &new_attr);
	if (!is_on)
		new_attr.c_lflag &= ~ECHOCTL;
	else
		new_attr.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
}

static void	initialise(t_pipe *data)
{
	data->pid = ft_calloc(sizeof(pid_t), data->cmdc);
	if (!data->pid)
		msg_freeall_exit("malloc", NULL, 1, data);
	data->pipe[0][0] = -1;
	data->pipe[0][1] = -1;
	data->pipe[1][0] = -1;
	data->pipe[1][1] = -1;
}

int	pipex(t_pipe	*data)
{
	int		i;
	int		exit_status;

	initialise(data);
	exit_status = 0;
	i = -1;
	toggle_carret(1);
	while (++i < data->cmdc)
		exit_status = execute(i, data);
	i = -1;
	while (++i < data->cmdc)
		waitpid(data->pid[i], &data->status, 0);
	toggle_carret(0);
	free(data->pid);
	if (exit_status == 4)
		exit_status = 2;
	if (exit_status)
		return (exit_status);
	if (WEXITSTATUS(data->status) != 0)
		return (WEXITSTATUS(data->status));
	if (data->status == 2)
		return (printf("\n") + 127);
	if (data->status == 3)
		return (printf("Quit: 3\n") + 121);
	return (0);
}
