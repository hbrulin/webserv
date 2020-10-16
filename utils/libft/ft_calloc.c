/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 14:24:47 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/23 13:45:27 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void *p;

	if (size == 0 || count == 0)
	{
		size = 1;
		count = 1;
	}
	if (!(p = (void *)malloc(size * count)))
		return (0);
	ft_bzero(p, size * count);
	return (p);
}
