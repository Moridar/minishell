/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:39:02 by bsyvasal          #+#    #+#             */
/*   Updated: 2023/11/27 14:06:37 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*str;
	int		s1len;
	int		s2len;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (ft_substr(s2, 0, ft_strlen(s2)));
	if (!s2)
	{
		free(s1);
		return (ft_substr(s1, 0, ft_strlen(s1)));
	}
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	str = malloc (s1len + s2len + 1);
	if (str)
	{
		ft_memcpy(str, s1, s1len);
		ft_memcpy(str + s1len, s2, s2len + 1);
	}
	free(s1);
	return (str);
}

static char	*read_line(int fd)
{
	char	*buffer;
	char	*res;
	int		bytes;

	buffer = malloc (BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	res = NULL;
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			break ;
		buffer[bytes] = 0;
		res = ft_strjoin_and_free(res, buffer);
		if (!res || (res && ft_strchr(res, '\n')))
			break ;
	}
	free(buffer);
	return (res);
}

static char	*cache_get_line(char *cache)
{
	int	i;

	i = 0;
	if (!*cache)
		return (0);
	while (cache[i] && cache[i] != '\n')
		i++;
	return (ft_substr(cache, 0, i + 1));
}

static char	*trim_cache(char *cache)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = NULL;
	while (cache[i] && cache[i] != '\n')
		i++;
	if (cache[i] == '\n')
		i++;
	tmp = ft_substr(cache, i, ft_strlen(cache) - i);
	free(cache);
	if (!tmp)
		return (NULL);
	if (!*tmp)
	{
		free(tmp);
		return (NULL);
	}
	return (tmp);
}

char	*get_next_line(int fd)
{
	static char	*cache;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE < 0 || read(fd, 0, 0) < 0)
	{
		while (cache)
			cache = trim_cache(cache);
		return (NULL);
	}
	if (!(cache && ft_strchr(cache, '\n')))
	{
		buffer = read_line(fd);
		if (!buffer)
			return (NULL);
		cache = ft_strjoin_and_free(cache, buffer);
		free(buffer);
	}
	if (cache)
	{
		buffer = cache_get_line(cache);
		cache = trim_cache(cache);
		return (buffer);
	}
	return (0);
}
