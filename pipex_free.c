/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 09:39:03 by bsyvasal          #+#    #+#             */
/*   Updated: 2024/02/09 13:15:03 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free the double string array
 * @param **char strarray
*/
void	freeall(char **strarray)
{
	int	i;

	if (strarray)
	{
		i = -1;
		while (strarray[++i])
			free(strarray[i]);
		free(strarray);
	}
}

int	free_return(void *ptr, int returnvalue)
{
	free(ptr);
	return (returnvalue);
}

int	freeall_return(char **strarray, int return_value)
{
	freeall(strarray);
	return (return_value);
}

void	*free_return_null(void *ptr)
{
	free(ptr);
	return (NULL);
}

void	freeall_exit(char **strarray, int exitno)
{
	freeall(strarray);
	exit(exitno);
}
