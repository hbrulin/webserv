#include "libft.h"
#include "iostream"

int		ft_whitespace(std::string str)
{
	int i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\r' ||
			str[i] == '\v' || str[i] == '\t' || str[i] == '\f')
		i++;
	return (i);
}

int		ft_neg(int i, std::string str)
{
	if (str[i] == '-')
		return (-1);
	else
		return (1);
}

int		check_base(std::string base)
{
	int i;
	int j;
	if (base.size() == 1)
		return (0);
	i = 0;
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-')
			return (0);
		j = 0;
		while (base[j])
		{
			if (base[j] == base[i] && j != i)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int		ft_check(char c, std::string base)
{
	int	i;

	i = 0;
	while (base[i] != '\0')
	{
		if (base[i] == c)
			return (i);
		i++;
	}
	return (-1);
}


long int		ft_atoi_base(std::string str, std::string base)
{
	long int nbr;
	int i;
	long int neg;
	int len;

	if (str.empty() || base.empty() || check_base(base) == 0)
		return (0);
	i = ft_whitespace(str);
	nbr = 0;
	neg = 1;
	len = base.size();
	if (str[i] == '+' || str[i] == '-')
	{
		neg = ft_neg(i, str);
		i++;
	}
	while (str[i] >= base[0] && str[i] <= base[len - 1])
	{
		nbr = nbr * len + ft_check(str[i], base);
		i++;
	}
	return (nbr * neg);
}