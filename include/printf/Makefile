# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: phofer <phofer@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/04 16:40:04 by phofer            #+#    #+#              #
#    Updated: 2025/06/04 16:40:26 by phofer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= libftprintf.a

SRC		= ft_printf_utils.c ft_printf_utils2.c libftprintf.c
OBJ		= $(SRC:.c=.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -fPIC
AR		= ar rcs
RM		= rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)
	ranlib $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re