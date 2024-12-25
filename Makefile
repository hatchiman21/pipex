LIBFT_DIR = libft
SRC_DR = src
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main.c find_path.c helper_functions.c limiter.c
SRCS := $(addprefix $(SRC_DR)/, $(SRCS))
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = pipex

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) inc/pipex.h
	$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(LIBFT) $(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean:
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(OBJS)
	$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
