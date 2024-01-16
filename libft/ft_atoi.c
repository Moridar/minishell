/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 11:16:21 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/10/31 09:55:26 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	long	i;
	int		sign;

	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
		sign *= -1;
	if (*str == '+' || *str == '-')
		str++;
	i = 0;
	while (ft_isdigit(*str))
	{
		i = i * 10 + *str - '0';
		if (i < 0)
		{
			if (sign == 1)
				return (-1);
			return (0);
		}
		str++;
	}
	return ((int) i * sign);
}
