/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:54:47 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/11/08 10:35:26 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	wordscount;
	int	foundword;

	i = 0;
	wordscount = 0;
	while (s[i])
	{
		foundword = 0;
		while (s[i] == c && s[i])
			i++;
		while (s[i] != c && s[i])
		{
			i++;
			foundword = 1;
		}
		if (foundword)
			wordscount++;
	}
	return (wordscount);
}

static int	arrayaddword(char **strarr, char const *s, int j, int i)
{
	char	*str;

	str = ft_substr(s, 0, i);
	if (!str)
	{
		while (--j >= 0)
		{
			free(strarr[j]);
		}
		free(strarr);
		return (0);
	}
	strarr[j] = str;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strarr;
	int		i;
	int		j;

	if (!s)
		return (0);
	strarr = malloc((count_words(s, c) + 1) * sizeof (char *));
	if (!strarr)
		return (0);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] != c)
			i++;
		if (i)
			if (!arrayaddword(strarr, s, j++, i))
				return (0);
		while (s[i] == c && s[i])
			i++;
		s += i;
		i = 0;
	}
	strarr[j] = 0;
	return (strarr);
}
