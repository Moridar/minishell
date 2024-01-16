/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 10:18:44 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/10/30 16:46:09 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	int		i;

	i = -1;
	ptr = NULL;
	while (s[++i])
		if (s[i] == (char) c)
			ptr = (char *) &s[i];
	if ((char) c == 0)
		ptr = (char *) &s[i];
	return (ptr);
}
