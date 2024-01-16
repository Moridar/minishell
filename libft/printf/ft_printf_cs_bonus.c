/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format1_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 12:53:40 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/11/14 09:16:26 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	printc(char c, t_flags *flags)
{
	int		len;
	char	*filling;

	if (flags->zero && !flags->leftjustify)
		filling = "0";
	else
		filling = " ";
	len = 0;
	if (flags->leftjustify && write(1, &c, 1) < 0)
		return (-1);
	while (flags->width > len++ + 1)
		if (write(1, filling, 1) < 0)
			return (-1);
	if (!flags->leftjustify && write(1, &c, 1) < 0)
		return (-1);
	return (len);
}

int	prints(char *str, t_flags *flags)
{
	int		len;
	int		totallen;
	char	*filling;

	if (!str)
		str = "(null)";
	if (flags->zero && !flags->leftjustify)
		filling = "0";
	else
		filling = " ";
	len = ft_strlen(str);
	if (flags->precsion >= 0 && flags->precsion < len)
		len = flags->precsion;
	totallen = len;
	if (flags->leftjustify && write(1, str, len) < 0)
		return (-1);
	while (flags->width > totallen++)
		if (write(1, filling, 1) < 0)
			return (-1);
	if (!flags->leftjustify && write(1, str, len) < 0)
		return (-1);
	return (totallen - 1);
}
