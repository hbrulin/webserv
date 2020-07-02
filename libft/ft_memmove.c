/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 09:35:53 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/23 13:30:06 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*p;
	const unsigned char	*q;
	size_t				i;

	p = (unsigned char *)dst;
	q = (unsigned char *)src;
	i = 0;
	if (!dst || !src)
		return (NULL);
	if (q < p)
	{
		while (++i <= n)
			p[n - i] = q[n - i];
	}
	else
	{
		while (n-- > 0)
			*p++ = *q++;
	}
	return (dst);
}
