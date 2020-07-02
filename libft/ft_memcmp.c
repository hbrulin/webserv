/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 09:35:53 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/11 12:10:28 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

int	ft_memcmp(void *dst, const void *src, size_t n)
{
	unsigned char	*p;
	unsigned char	*q;
	size_t			i;

	i = 0;
	p = (unsigned char *)dst;
	q = (unsigned char *)src;
	while (i < n && p[i] == q[i])
		i++;
	if (i == n)
		return (0);
	return (p[i] - q[i]);
}
