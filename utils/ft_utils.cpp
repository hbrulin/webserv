#include "libft.h"
#include <limits.h>
#include "definitions.hpp"

#include <iostream>

static unsigned int		ft_len(int nb)
{
	unsigned int len;

	len = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		nb = -nb;
		len++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

char					*ft_itoa(int n)
{
	unsigned int	size;
	unsigned int	nb;
	unsigned int	sign;
	unsigned char	*tab;

	nb = (unsigned int)n;
	size = ft_len(n);
	if (n == INT_MIN)
		return (ft_strdup(INT_MIN_STR));
	if (!(tab = (unsigned char*)malloc(size + 1)))
		return (NULL);
	nb = n < 0 ? n * -1 : n * 1;
	sign = n < 0 ? 1 : 0;
	while (size > sign)
	{
		tab[size - 1] = (nb % 10) + '0';
		nb = nb / 10;
		size--;
	}
	if (n < 0)
		tab[0] = '-';
	tab[ft_len(n)] = '\0';
	return ((char *)tab);
}

static int		ft_nb(char const *str, char c)
{
	unsigned int i;
	unsigned int tab_size;

	if (!str)
		return (0);
	i = 0;
	tab_size = 0;
	while (str[i])
	{
		if (str[i] != c)
			tab_size++;
		while (str[i] != c && str[i])
			i++;
		while (str[i] == c && str[i])
			i++;
	}
	return (tab_size);
}

static char		*ft_strcpy(char *dest, char const *src, int pos, int str_len)
{
	int i;

	i = 0;
	while (pos + i < str_len)
	{
		dest[i] = src[pos + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char		**ft_free(char **tab, int nb)
{
	int i;

	i = 0;
	while (i < nb)
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
	return (0);
}

char			**ft_split(char const *s, char c)
{
	t_split	p;
	char	**tab;

	if (!(tab = (char **)malloc(sizeof(char*) * (ft_nb(s, c) + 1))) || !s)
		return (NULL);
	p.i = 0;
	p.str_nb = 0;
	tab[ft_nb(s, c)] = NULL;
	while (s[p.i] != '\0')
	{
		p.j = 0;
		while (s[p.i + p.j] != '\0' && s[p.i + p.j] != c)
			p.j = p.j + 1;
		if (p.j != 0)
		{
			if (!(tab[p.str_nb] = (char *)malloc(sizeof(*s) * (p.j + 1))))
				return (ft_free(tab, p.str_nb));
			tab[p.str_nb] = ft_strcpy(tab[p.str_nb], s, p.i, p.i + p.j);
			p.str_nb = p.str_nb + 1;
		}
		while (s[p.i] && s[p.i] == c)
			p.i = p.i + 1;
		p.i = p.i + p.j;
	}
	return (tab);
}

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	unsigned int		i;
	unsigned int		j;
	char				*str;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
		i++;
	str = (char *)malloc((1 + i) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (j < i)
	{
		str[j] = s1[j];
		j++;
	}
	str[j] = '\0';
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t		i;
	size_t		j;
	char		*str;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	else if (s1 == NULL)
		return (ft_strdup(s2));
	else if (s2 == NULL)
		return (ft_strdup(s1));
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = 0;
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		j;
	int		k;
	char	*str;
	int		i;

	i = 0;
	k = 0;
	if (!s1)
		return (NULL);
	while (ft_strchr(set, s1[i]) != NULL && s1[i] != '\0')
		i++;
	j = ft_strlen(s1);
	while (ft_strchr(set, s1[j - 1]) != NULL && j > i)
		j--;
	if (!(str = (char *)malloc(j - i + 1)))
		return (NULL);
	while (i < j && s1[i])
	{
		str[k] = s1[i];
		i++;
		k++;
	}
	str[k] = '\0';
	return (str);
}

char	*ft_strchr(const char *s, int c)
{
	size_t i;

	i = 0;
	while (i <= ft_strlen(s))
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	return (NULL);
}

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}

void	ft_tabdel(void **tab)
{
	int i;

	i = 0;
	if (tab)
	{
		while (tab[i])
			free(tab[i++]);
		free(tab);
	}
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char *p;

	p = (unsigned char *)b;
	while (len > 0)
	{
		*(p++) = (unsigned char)c;
		len--;
	}
	return (b);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*p;
	const unsigned char	*q;
	size_t				i;

	i = 0;
	if (!(dst || src))
		return (dst);
	p = (unsigned char *)dst;
	q = (const unsigned char *)src;
	while (n)
	{
		p[i] = q[i];
		i++;
		n--;
	}
	return (dst);
}

bool path_exists(std::string& s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

int		getline(std::string &buffer, std::string &line)
{
	size_t					pos;

	pos = buffer.find("\n");
	if (pos != std::string::npos)
	{
		line = std::string (buffer, 0, pos++);
		buffer = buffer.substr(pos);
	}
	else
	{
		if (buffer[buffer.size() - 1] == '\n')
			buffer = buffer.substr(buffer.size());
		else
		{
			line = buffer;
			buffer = buffer.substr(buffer.size());
		}
	}
	//std::cout << line.size() << std::endl;
	return line.size();
}