/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 16:04:41 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/23 13:44:32 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
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
