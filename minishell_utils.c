/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:19:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/30 22:21:31 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	free_return(void *ptr, int returnvalue)
{
	free(ptr);
	return (returnvalue);
}

void	free_env_exit(t_pipe *data, int exitno)
{
	freeall(data->envp);
	exit(exitno);
}

/**
 * @return size of the double dimension string array, for example,
 * created by ft_split().
*/
int	get_string_array_size(char **str)
{
	int	j;

	j = 0;
	while (str[j])
	{
		j++;
	}
	return (j);
}

/**
 * Used to copy double dimension array arr1 to arr2.
 * For example, envp.
*/
char	**copy_double_array(char **arr1, int increase_size)
{
	int		size;
	int		i;
	int		len;
	char	**arr2;

	i = 0;
	size = get_string_array_size(arr1);
	arr2 = (char **)ft_calloc((size + 1 + increase_size), sizeof(char *));
	if (!arr2)
		return (NULL);
	while (i < size)
	{
		len = (ft_strlen(arr1[i]) + 1);
		arr2[i] = (char *)ft_calloc(len, sizeof(char));
		if (!arr2[i])
		{
			freeall(arr2);
			return (NULL);
		}
		ft_memmove(arr2[i], arr1[i], len);
		i++;
	}
	return (arr2);
}

/**
 * Reallocates the arraylist when some elements are deleted
 * @param newsize is the size of the new array
*/
char	**reallocate_arraylist(char **arr, int newsize)
{
	char	**new_arr;
	int		i;
	int		j;

	new_arr = (char **)ft_calloc((newsize + 1), sizeof(char *));
	if (!new_arr)
		return (NULL);
	i = 0;
	j = 0;
	while (j < newsize)
	{
		if (arr[i])
		{
			new_arr[j] = arr[i];
			j++;
		}
		i++;
	}
	free(arr[i]);
	free(arr);
	return (new_arr);
}

/* int	main(int argc, char *argv[], char *envp[])
{
	t_pipe 	data;

	copy_double_array(envp, &data.envp);
} */