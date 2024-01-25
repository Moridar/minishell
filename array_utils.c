/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:19:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/25 18:28:52 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @return size of the double dimension string array, for example, created by ft_split().
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
void copy_double_array(char **arr1, char ***arr2)
{
	int	size;
	int i;
	int len;
	
	i = 0;
	size = get_string_array_size(arr1);
	// printf("%d\n", size);
	*arr2 = (char **)malloc((size + 1) * sizeof(char *));
	(*arr2)[size] = NULL;
	while (i < size - 1)
	{
		len = (ft_strlen(arr1[i]) + 1);
		(*arr2)[i] = (char *)malloc(len * sizeof(char));
		ft_memmove((*arr2)[i], arr1[i], len - 1);
		// printf("%s\n", (*arr2)[i]);
		i++;
	}
}