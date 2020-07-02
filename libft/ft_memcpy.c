/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 09:35:53 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/23 13:45:54 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
