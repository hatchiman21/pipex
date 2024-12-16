LIBFT_DIR = libft
SRC_DR = src
B_SRC_DR = bonus_src
LIBFT = $(LIBFT_DIR)/libft.a

SRCS =  main.c find_path.c helper_functions.c
SRCS:= $(addprefix $(SRC_DR)/, $(SRCS))
B_SRCS = main.c find_path.c helper_functions.c limiter.c
B_SRCS:= $(addprefix $(B_SRC_DR)/bonus_, $(B_SRCS))
OBJS = $(SRCS:.c=.o)
B_OBJS = $(B_SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = pipex
B_NAME = bonus_pipex

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) inc/pipex.h
	$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft

$(B_NAME): $(B_OBJS) inc/bonus_pipex.h
	$(CC) $(B_OBJS) -o $(B_NAME) -L$(LIBFT_DIR) -lft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(LIBFT) $(B_NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) $(B_OBJS)
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(B_OBJS)
	$(RM) $(NAME)
	$(RM) $(B_NAME)

re: fclean all

.PHONY: all bonus clean fclean re
