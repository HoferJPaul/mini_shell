NAME		= minishell

# Directories
SRC_DIR		= src
OBJ_DIR		= objs
INC_DIR		= include
LIBFT_DIR	= include/libft
PRINTF_DIR	= include/printf

# Files
SRC_FILES	= main.c
SRC			= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Compiler
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_DIR)/include -I$(PRINTF_DIR)/include
# ↑ assuming libft/ and printf/ are top-level peers, and their headers are in include/

# Libs
LIBFT		= $(LIBFT_DIR)/libft.a
PRINTF		= $(PRINTF_DIR)/libftprintf.a

LDLIBS		= -lreadline

.DELETE_ON_ERROR:

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(PRINTF):
	@make -C $(PRINTF_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(PRINTF) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) $(PRINTF) $(LDLIBS) -o $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(PRINTF_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@make -C $(PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
