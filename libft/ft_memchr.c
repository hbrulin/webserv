/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 11:48:40 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/23 13:46:16 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *src, int c, size_t n)
{
	unsigned char	*q;
	size_t			i;

	if (src == NULL)
		return (NULL);
	i = 0;
	q = (unsigned char *)src;
	while (i < n)
	{
		if (q[i] == (char)c)
			return ((char *)src + i);
		i++;
	}
	return (NULL);
}
