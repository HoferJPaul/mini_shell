NAME		= minishell

# Directories
SRC_DIR		= src
OBJ_DIR		= objs
LIBFT_DIR	= libft

# Files
SRC_FILES	=	main.c \
				utils/cleanup.c \
				utils/handle_signals.c \
				utils/init.c \
				utils/refresh_paths.c \
				built_in/cd.c \
				built_in/echo.c \
				built_in/env.c \
				built_in/export.c \
				built_in/pwd.c \
				parser/tokenize_words.c \
				parser/tokenize_utils.c \
				parser/tokenize.c \
				parser/expansion.c \
				parser/expansion_char.c \
				parser/expansion_utils.c \
				parser/parse.c \
				parser/parse_redir.c \
				parser/parse_cmd.c \
				parser/command_utils.c \
				copy_env/copy_env.c \
				copy_env/env_utils.c \
				copy_env/print_env.c \
				tests/test_print.c \

SRC			= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Compiler
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I$(LIBFT_DIR)

# Libs
LIBFT		= $(LIBFT_DIR)/libft.a

LDLIBS		= -lreadline

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) $(LDLIBS) -o $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
