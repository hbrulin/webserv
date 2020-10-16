/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 16:17:05 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/22 10:04:33 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
