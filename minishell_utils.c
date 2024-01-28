/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:19:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/28 02:09:43 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
void	copy_double_array(char **arr1, char ***arr2, int increase_size)
{
	int	size;
	int	i;
	int	len;

	i = 0;
	size = get_string_array_size(arr1);
	*arr2 = (char **)ft_calloc((size + 1 + increase_size), sizeof(char *));
	while (i < size)
	{
		len = (ft_strlen(arr1[i]) + 1);
		(*arr2)[i] = (char *)ft_calloc(len, sizeof(char));
		ft_memmove((*arr2)[i], arr1[i], len);
		i++;
	}
}

char	**reallocate_arraylist(char **arr, int size)
{
	char	**new_arr;
	int		i;
	int		j;

	new_arr = (char **)ft_calloc((size), sizeof(char *));
	i = 0;
	j = 0;
	while (i < size - 1)
	{
		if (arr[i])
		{
			new_arr[j] = arr[i];
			j++;
		}
		i++;
	}
	freeall(arr);
	return (new_arr);
}

/* int	main(int argc, char *argv[], char *envp[])
{
	t_pipe 	data;

	copy_double_array(envp, &data.envp);
} */