#include "libft.h"
#include <iostream>
#include <string.h>


int		ft_fill_line(char **s, char **line, int len)
{
	char	*tmp;

	if ((*s)[len] == '\n')
	{
		*line = ft_substr(*s, 0, len);
		tmp = ft_strdup((*s) + len + 1);
		free((*s));
		(*s) = tmp;
		if ((*s)[0] == '\0')
		{
			free(*s);
			*s = NULL;
		}
	}
	else if ((*s)[len] == '\0')
	{
		*line = ft_strdup(*s);
		free(*s);
		*s = NULL;
		return (0);
	}
	return (1);
}

int		ft_return(char **line, char **s, int ret)
{
	int len;

	len = 0;
	if (ret < 0)
	{
		return (-1);
	}
	else if (ret == 0 && (*s == NULL || (*s)[0]  == '\0'))
	{
		*line = ft_strdup("");
		return (0);
	}
	while ((*s)[len] != '\n' && (*s)[len])
		len++;
	return (ft_fill_line(s, line, len));
}

int	getBuf(char **file, char **buf, unsigned int len)
{
	//std::cout << file.size() << std::endl;
	if (ft_strlen(*file) == 0)
		*buf = ft_strdup("");
	else if (len <= ft_strlen(*file))
		*buf = ft_substr(*file, 0, len);
	else
		*buf = ft_substr(*file, 0, ft_strlen(*file));
	char *tmp = ft_substr(*file, ft_strlen(*buf), ft_strlen(*file) - ft_strlen(*buf));
	free(*file);
	*file = tmp;
	//std::cout << buf << std::endl;
	return ft_strlen(*buf);
}


int		get_next_line(char **file, char **line)
{
	static char		*s;
	char			*buf;
	char			*tmp;
	int				ret;

	while ((ret = getBuf(file, &buf, 1000000)) > 0)
	{
		buf[ret] = '\0';
		if (s == NULL)
			s = ft_strnew(1);
		tmp = ft_strjoin(s, buf);
		free(s);
		s = tmp;
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (ft_return(line, &s, ret));
}