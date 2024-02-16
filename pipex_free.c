/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 09:39:03 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/17 01:04:57 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free the double string array
 * @param **char strarray
*/
void	freeall(char **arraylist)
{
	int	i;

	if (arraylist)
	{
		i = -1;
		while (arraylist[++i])
			free(arraylist[i]);
		free(arraylist);
	}
}

int	free_return(void *ptr, int returnvalue)
{
	free(ptr);
	return (returnvalue);
}

int	freeall_return(char **arraylist, int return_value)
{
	freeall(arraylist);
	return (return_value);
}

void	*free_return_null(void *ptr)
{
	free(ptr);
	return (NULL);
}

void	freeall_exit(char **arraylist, int exitno)
{
	freeall(arraylist);
	exit(exitno);
}
