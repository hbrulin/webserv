/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 18:50:24 by bothilie          #+#    #+#             */
/*   Updated: 2019/10/23 13:37:45 by bothilie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *begin_list;
	t_list *tmp;

	if (!lst || !f || !del)
		return (NULL);
	if (!(tmp = ft_lstnew(f(lst->content))))
		return (NULL);
	begin_list = tmp;
	while (lst->next != NULL)
	{
		lst = lst->next;
		if (!(tmp->next = ft_lstnew(f(lst->content))))
			ft_lstclear(&lst, del);
		tmp = tmp->next;
	}
	return (begin_list);
}
