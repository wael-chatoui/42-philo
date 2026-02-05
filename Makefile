NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror

OBJS_DIR = obj
SRCS_DIR = .
INC_DIR = .

SRC_FILES = main
SRCS = $(addsuffix .c, $(SRC_FILES))

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

LPTHREAD_FLAGS = -pthread
# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@ $(LPTHREAD_FLAGS)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LPTHREAD_FLAGS)
	@echo "✓ $(NAME) created successfully!"

clean:
	rm -f $(OBJS_DIR)/*.o
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
