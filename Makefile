NAME = webserv

CC = clang++

SRCS = main.cpp parser/ConfigParser.cpp parser/Data.cpp listener/listener.cpp request/request.cpp request/ft_utils.cpp

CFLAGS = -g

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
