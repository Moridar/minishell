/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:50:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/26 00:09:04 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* echo_n()

cd()

pwd()

export()

unset() */

void	env(t_pipe *data)
{
	int i;

	i = -1;
	while(data->envp[++i])
	{
		// data->envp[i][0] = 'X';
		printf("%s\n", data->envp[i]);
	}
}
