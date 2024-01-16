/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:07:09 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/11/27 14:01:42 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft.h"

typedef struct s_flags
{
	int	leftjustify;
	int	width;
	int	precsion;
	int	hexsign;
	int	sign;
	int	space;
	int	zero;
}		t_flags;
int		ft_printf(const char *str, ...);
int		convert(const char **str, va_list argptr, t_flags *flags);
int		printp(unsigned long l, t_flags *flags);
int		printd(int i, t_flags *flags);
int		printu(unsigned int ui, t_flags *flags);
int		printhex(unsigned long ui, int upper, t_flags *flags);
int		prints(char *str, t_flags *flags);
int		printc(char c, t_flags *flags);
char	*ft_itoa_hex(unsigned long n, int upper);
char	*ft_itoa_unsigned(unsigned long n);
char	*checkprecsion(char *str, t_flags *flags);
char	*checksign(char *str, t_flags *flags);
char	*checkzero(char *str, t_flags *flags);
char	*addprefix(char *hexstr, int upper, t_flags *flags);
char	*freeandreturn(char *freethis, char *retur);

#endif