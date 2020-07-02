/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <bothilie@stduent.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 16:07:50 by bothilie          #+#    #+#             */
/*   Updated: 2020/07/01 17:51:16 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
