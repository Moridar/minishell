/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 14:48:35 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/11/13 16:59:30 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

t_flags	*initflags(void)
{
	t_flags	*flags;

	flags = malloc(sizeof(t_flags));
	if (flags)
	{
		flags->leftjustify = 0;
		flags->width = 0;
		flags->precsion = -1;
		flags->hexsign = 0;
		flags->sign = 0;
		flags->space = 0;
		flags->zero = 0;
	}
	return (flags);
}

void	setnumber(const char **str, int *var, va_list argptr)
{
	if (**str == '.')
		(*str)++;
	if (**str == '*')
	{
		*var = va_arg(argptr, int);
		return ;
	}
	*var = ft_atoi(*str);
	if (!ft_isdigit(**str))
	{
		--*str;
		return ;
	}
	while (ft_isdigit(*(*str + 1)))
		(*str)++;
}

int	checkflags(const char **str, va_list argptr, t_flags *flags)
{
	if (!flags)
		return (-1);
	while (*(++*str))
	{
		if (**str == '0')
			flags->zero = 1;
		else if (**str == '-')
			flags->leftjustify = 1;
		else if (ft_isdigit(**str))
			setnumber(str, &flags->width, argptr);
		else if (**str == '.')
			setnumber(str, &flags->precsion, argptr);
		else if (**str == '#')
			flags->hexsign = 1;
		else if (**str == '+')
			flags->sign = 1;
		else if (**str == ' ')
			flags->space = 1;
		else if (**str == '*')
			flags->width = va_arg(argptr, int);
		else
			break ;
	}
	return (convert(str, argptr, flags));
}

int	convert(const char **str, va_list argptr, t_flags *flags)
{
	if (**str == '%')
		return (printc(**str, flags));
	if (**str == 'c')
		return (printc(va_arg(argptr, int), flags));
	if (**str == 's')
		return (prints(va_arg(argptr, char *), flags));
	if (**str == 'p')
		return (printp((unsigned long)va_arg(argptr, void *), flags));
	if (**str == 'd' || **str == 'i')
		return (printd(va_arg(argptr, int), flags));
	if (**str == 'u')
		return (printu(va_arg(argptr, unsigned int), flags));
	if (**str == 'x')
		return (printhex(va_arg(argptr, unsigned int), 0, flags));
	if (**str == 'X')
		return (printhex(va_arg(argptr, unsigned int), 1, flags));
	--*str;
	return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	argptr;
	int		i;
	int		checksum;
	t_flags	*flags;

	va_start(argptr, str);
	i = 0;
	checksum = 0;
	while (*str && checksum >= 0)
	{
		if (*str != '%')
			checksum = write(1, str, 1);
		else
		{
			flags = initflags();
			checksum = checkflags(&str, argptr, flags);
			free(flags);
		}
		i += checksum;
		str++;
	}
	va_end(argptr);
	if (checksum == -1)
		return (-1);
	return (i);
}
