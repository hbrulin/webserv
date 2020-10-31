/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdurand <jdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 13:25:50 by jdurand           #+#    #+#             */
/*   Updated: 2020/10/31 11:20:32 by jeromedurand     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
//# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <ctype.h>

# define STATIC_BUFFER_SIZE 32

static size_t	ft_strlen(char const *str)
{
	size_t len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}


static char	*ft_strdup(char const *src)
{
	unsigned int	len;
	unsigned int	i;
	char			*dest;

	i = 0;
	len = ft_strlen(src);
	if (!(dest = (char*)malloc((len + 1) * sizeof(char))))
		return (NULL);
	else
	{
		while (src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
		return (dest);
	}
}

static char	*ft_strndup(char const *src, size_t n)
{
	size_t	i;
	char	*dest;

	i = 0;
	if (!(dest = (char*)malloc((n + 1) * sizeof(char))))
		return (NULL);
	else
	{
		while (src[i] != '\0' && i < n)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
		return (dest);
	}
}

static char	*ft_strncat_op(char *dest, char const *src, size_t n, size_t l)
{
	size_t	i;
	size_t	j;

	i = l;
	j = 0;
	while (src[j] != 0 && j < n)
		dest[i++] = src[j++];
	dest[i] = 0;
	return (dest);
}

static char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			pc;
	char			*str;

	str = (char *)s;
	i = 0;
	pc = (char)c;
	while (str[i] != 0)
	{
		if (str[i] == pc)
			return (&str[i]);
		i++;
	}
	if (str[i] == 0 && pc == 0)
		return (&str[i]);
	else
		return (NULL);
}

static char	*ft_strcpy(char *dest, char const *src)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}



static char	*ft_strnjoin(char const *s1, char const *s2, size_t n)
{
	size_t			megasize;
	char			*b;
	size_t			len_1;

	if (!s1 && s2)
		return (ft_strndup(s2, n));
	else if (!s2 && s1)
		return (ft_strdup(s1));
	else if (!s1 && !s2)
		return (NULL);
	len_1 = ft_strlen(s1);
	megasize = len_1 + n;
	if (!(b = (char *)malloc((megasize + 1) * sizeof(char))))
		return (NULL);
	b[0] = 0;
	ft_strcpy(b, s1);
	free((char*)s1);
	ft_strncat_op(b, s2, n, len_1);
	return (b);
}

static int	parse_line2(char *buffer, char *b_el, char **line, char *b_line)
{
	if (!(*line = ft_strnjoin(b_line, buffer, b_el - buffer)))
		return (-1);
	ft_strcpy(buffer, b_el + 1);
	b_line = NULL;
	return (1);
}

static int	parse_line(char *buffer, char *buffer_el, char **line, char *b_line)
{
	if (!(*line = ft_strndup(b_line, buffer_el - b_line)))
		return (-1);
	ft_strcpy(buffer, buffer_el + 1);
	free(b_line);
	b_line = NULL;
	return (1);
}

static int	parse_line3(char **line, char *b_line)
{
	if (!(*line = ft_strdup(b_line)))
		return (-1);
	free(b_line);
	b_line = NULL;
	return (0);
}

int			get_next_line(int fd, char **line)
{
	static char	buffer[STATIC_BUFFER_SIZE + 1];
	char		buffer_nul[1];
	char		*b_line;
	char		*buffer_el;
	int			ret;

	if (fd < 0 || read(fd, buffer_nul, 0) < 0 ||
		!(b_line = ft_strndup(buffer, STATIC_BUFFER_SIZE)))
		return (-1);
	if ((buffer_el = ft_strchr(b_line, '\n')) != NULL)
		return (parse_line(buffer, buffer_el, line, b_line));
	while ((ret = read(fd, buffer, STATIC_BUFFER_SIZE)) > 0)
	{
		buffer[ret] = 0;
		if ((buffer_el = ft_strchr(buffer, '\n')) != NULL)
			return (parse_line2(buffer, buffer_el, line, b_line));
		if (!(b_line = ft_strnjoin(b_line, buffer, STATIC_BUFFER_SIZE)))
			return (-1);
	}
	buffer[ret] = 0;
	return (parse_line3(line, b_line));
}
