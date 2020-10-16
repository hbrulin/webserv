# **************************************************************************** #
#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bothilie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/10 13:23:17 by bothilie          #+#    #+#              #
#    Updated: 2019/10/10 13:48:13 by bothilie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a

CC=gcc

CFLAGS=-Wall -Wextra -Werror

RM=rm -f

LDFLAGS=-L.

LDLIBS=-lft

SRC=ft_atoi.c ft_strchr.c ft_isalnum.c ft_memset.c  \
ft_putchar_fd.c \
ft_tolower.c \
ft_toupper.c \
ft_atoi.c \
	ft_split.c \
	ft_strdup.c \
	ft_isdigit.c \
	ft_isalpha.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_bzero.c \
	ft_strlcat.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strncmp.c\
	ft_memcpy.c \
	ft_memcmp.c \
	ft_memchr.c \
	ft_memccpy.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_isascii.c \
	ft_strrchr.c \
	ft_strlcpy.c \
	ft_strnstr.c \
	ft_strmapi.c \
	ft_putstr_fd.c \
	ft_putnbr_fd.c \
	ft_putendl_fd.c \
	ft_memmove.c \
	ft_strtrim.c \
	ft_itoa.c \
	ft_calloc.c \

SRCBONUS = ft_lstnew_bonus.c \
	ft_lstadd_front_bonus.c \
	ft_lstsize_bonus.c \
	ft_lstlast_bonus.c \
	ft_lstadd_back_bonus.c \
	ft_lstiter_bonus.c \
	ft_lstmap_bonus.c \
	ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c \

OBJ=$(SRC:.c=.o)

OBJBONUS=$(SRCBONUS:.c=.o)

$(NAME):
	$(CC) $(CFLAGS) -c $(SRC)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

all: $(NAME) bonus

bonus : 
	$(CC) $(CFLAGS) -c $(SRCBONUS)
	ar rc $(NAME) $(OBJ) $(OBJBONUS)

clean:
	$(RM) $(OBJ) $(OBJBONUS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean, fclean

