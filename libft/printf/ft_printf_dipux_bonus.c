/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dipux_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:57:14 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/11/14 09:38:39 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	printp(unsigned long l, t_flags *flags)
{
	int		len;

	flags->hexsign = 1;
	len = printhex(l, 2, flags);
	if (len < 0)
		return (-1);
	return (len);
}

int	printd(int i, t_flags *flags)
{
	int		len;
	char	*str;

	str = ft_itoa(i);
	if (!str)
		return (-1);
	if (flags->precsion >= 0)
		flags->zero = 0;
	flags->hexsign = 0;
	str = checkprecsion(str, flags);
	str = checksign(str, flags);
	str = checkzero(str, flags);
	if (!str)
		return (-1);
	flags->precsion = -1;
	len = prints(str, flags);
	free(str);
	return (len);
}

int	printu(unsigned int ui, t_flags *flags)
{
	int		len;
	char	*str;

	str = ft_itoa_unsigned(ui);
	if (!str)
		return (-1);
	if (flags->precsion >= 0)
		flags->zero = 0;
	str = checkprecsion(str, flags);
	if (!str)
		return (-1);
	flags->precsion = -1;
	len = prints(str, flags);
	free(str);
	return (len);
}

int	printhex(unsigned long ui, int upper, t_flags *flags)
{
	int		len;
	char	*hexstr;

	hexstr = ft_itoa_hex(ui, upper);
	if (!hexstr)
		return (-1);
	if (flags->precsion >= 0)
		flags->zero = 0;
	if (ui == 0 && upper != 2)
		flags->hexsign = 0;
	hexstr = checkprecsion(hexstr, flags);
	hexstr = checkzero(hexstr, flags);
	if (!hexstr)
		return (-1);
	hexstr = addprefix(hexstr, upper, flags);
	if (!hexstr)
		return (-1);
	flags->precsion = -1;
	len = prints(hexstr, flags);
	free(hexstr);
	return (len);
}
