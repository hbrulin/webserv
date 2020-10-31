/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bothilie <bothilie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 14:26:25 by bothilie          #+#    #+#             */
/*   Updated: 2020/10/31 11:10:53 by jeromedurand     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
//# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <string>
#include <sys/types.h>
#include <sys/stat.h>


typedef struct	s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;
typedef struct	s_split
{
	int	i;
	int	j;
	int	str_nb;
}				t_split;
t_list			*add_link(t_list *list, char *str);
void			print_list (t_list *list);
void			ft_lstdelone(t_list *lst, void (*del)(void *));
void			ft_lstclear(t_list **lst, void (*del)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
				void (*del)(void *));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstnew(void *content);
void			ft_lstadd_front(t_list **alst, t_list *nv);
void			ft_lstadd_back(t_list **alst, t_list *nv);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
int				ft_atoi(const char *str);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_toupper(int c);
int				ft_tolower(int c);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *str, int fd);
void			ft_putnbr_fd(int nb, int fd);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
size_t			ft_strlcpy(char *dest, const char *src, size_t destsize);
char			*ft_strdup(const char *src);
size_t			ft_strlcat(char *dest, const char *src, size_t size);
size_t			ft_strlen(const char *str);
char			*ft_strnstr(const char *str, const char *needle, size_t len);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
void			*ft_memset(void *b, int c, size_t len);
void			ft_bzero(void *b, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memccpy(void *dst, const void *src, int c, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			ft_putendl_fd(char *s, int fd);
char			*ft_itoa(int nbr);
char			**ft_split(char const *s, char c);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_strsub(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s1, char const *set);
void			*ft_calloc(size_t count, size_t size);
void			ft_tabdel(void **tab);
long int		ft_atoi_base(std::string str, std::string base);
bool 			path_exists(std::string& s);
int				getline(std::string &buffer, std::string &line);
char			*ft_strstr(const char *haystack, const char *needle);

int				get_next_line(int fd, char **line);

#endif
