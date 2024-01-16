/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:28:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/10/30 16:30:05 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	intlen(int n)
{
	int	i;

	i = 1;
	while (ft_power(10, i) <= n)
		if (++i == 10)
			break ;
	return (i);
}

char	*ft_itoa(int n)
{
	int		neg;
	char	*str;
	int		i;

	neg = 0;
	if (n < 0)
	{
		if (n == -2147483648)
			return (ft_strdup("-2147483648"));
		n = n * -1;
		neg = 1;
	}
	i = intlen(n) + 1;
	str = malloc(neg + i--);
	if (!str)
		return (0);
	str[neg + i--] = 0;
	while (i >= 0)
	{
		str[neg + i--] = n % 10 + 48;
		n /= 10;
	}
	if (neg)
		str[0] = '-';
	return (str);
}
