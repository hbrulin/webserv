NAME = webserv

CC = clang++

SRCS = main.cpp parser/ConfigParser.cpp parser/Data.cpp listener/listener.cpp request/request.cpp request/ft_utils.cpp parser/Config.cpp headers/head_resp.cpp headers/head_req.cpp
BUILD_DIR = obj
CFLAGS = -Wall -Wextra -Werror -MMD -g

OBJS = $(SRCS:.cpp=.o)
OBJ = $(addprefix $(BUILD_DIR)/,$(OBJS))
DPD = $(SRCS:.cpp=.d)

opti :
	@make -j all

all : $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(NAME) created"

$(BUILD_DIR)/%.o: %.cpp Makefile
	@mkdir -p $(BUILD_DIR) $(BUILD_DIR)/parser $(BUILD_DIR)/listener $(BUILD_DIR)/request
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@/bin/rm -rf $(BUILD_DIR)
	@/bin/rm -rf webserv.dSYM
	@echo "Object and DPD files deleted"

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "Lib deleted"

re :
	@make fclean
	@make opti

flagless:
	clang++ $(SRCS)


.PHONY: all, clean, fclean, re, opti

-include $(DPD)
