/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_helpers_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:06:52 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/11/15 11:38:01 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

char	*freeandreturn(char *freethis, char *retur)
{
	free(freethis);
	return (retur);
}

char	*checkprecsion(char *str, t_flags *flags)
{
	char	*zerostr;
	char	*newstr;
	int		n;
	int		sign;

	newstr = NULL;
	if (*str == '0' && flags->precsion == 0)
		return (freeandreturn(str, ft_strdup("")));
	if (flags->precsion < 0 || flags->precsion < (int) ft_strlen(str))
		return (str);
	sign = 0;
	if (*str == '-')
		sign = 1;
	n = flags->precsion - (int) ft_strlen(str + sign);
	zerostr = malloc(n + 1 + sign);
	if (!zerostr)
		return (freeandreturn(str, NULL));
	zerostr[n + sign] = 0;
	ft_memset(zerostr + sign, '0', n);
	if (sign)
		zerostr[0] = *str;
	newstr = ft_strjoin(zerostr, str + sign);
	free(zerostr);
	free(str);
	return (newstr);
}

char	*checkzero(char *str, t_flags *flags)
{
	char	*zerostr;
	char	*newstr;
	int		nzero;
	int		sign;

	if (!str || !flags->zero || flags->leftjustify
		|| flags->width - 2 * flags->hexsign <= (int) ft_strlen(str))
		return (str);
	sign = 0;
	if (*str == '-' || *str == '+' || *str == ' ')
		sign = 1;
	nzero = flags->width - (int) ft_strlen(str) - 2 * flags->hexsign;
	zerostr = malloc(nzero + sign + 1);
	if (!zerostr)
		return (freeandreturn(str, NULL));
	zerostr[nzero + sign] = 0;
	ft_memset(zerostr + sign, '0', nzero);
	if (sign)
		zerostr[0] = *str;
	newstr = ft_strjoin(zerostr, str + sign);
	free(zerostr);
	free(str);
	return (newstr);
}

char	*checksign(char *str, t_flags *flags)
{
	char	*tmp;

	if (!str || *str == '-')
		return (str);
	if (flags->sign)
	{
		tmp = str;
		str = ft_strjoin("+", str);
		free(tmp);
		return (str);
	}
	if (flags->space)
	{
		tmp = str;
		str = ft_strjoin(" ", str);
		free(tmp);
		return (str);
	}
	return (str);
}

char	*addprefix(char *hexstr, int upper, t_flags *flags)
{
	char	*prefix;
	char	*str;

	prefix = "";
	if (flags->hexsign && upper == 1)
		prefix = "0X";
	else if (flags->hexsign || upper == 2)
		prefix = "0x";
	str = ft_strjoin(prefix, hexstr);
	free(hexstr);
	return (str);
}
