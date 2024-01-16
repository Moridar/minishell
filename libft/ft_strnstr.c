/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 11:00:24 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/10/27 19:04:06 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;

	if ((!haystack && !len) || (!*needle))
		return ((char *) haystack);
	while (*haystack)
	{
		if (len == 0)
			break ;
		i = 0;
		while (haystack[i] && haystack[i] == needle[i] && i < len)
			i++;
		if (!needle[i])
			return ((char *) haystack);
		haystack++;
		len--;
	}
	return (NULL);
}
