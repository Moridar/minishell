/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:50:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/25 19:27:00 by vshchuki         ###   ########.fr       */
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
		printf("%s\n", data->envp[i]);
}
