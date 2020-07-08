NAME = webserv

CC = clang++

SRCS = main.cpp ConfigParser.cpp Data.cpp listener.cpp request.cpp ft_utils.cpp

CFLAGS = -Wall -Wextra -Werror -g

opti :
	@make -j all

all : $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
	@echo "$(NAME) created"

clean:
	@/bin/rm -rf *.o
	@echo "Object files deleted"

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "Lib deleted"

re : 
	@make fclean
	@make opti

.PHONY: all, clean, fclean, re, opti
