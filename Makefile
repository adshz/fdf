# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 14:14:29 by szhong            #+#    #+#              #
#    Updated: 2024/07/18 10:29:38 by szhong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	fdf
CC		:=	cc 
CFLAGS		:=	-Wall -Werror -Werror -g
SRC_DIR		:=	./src
SRCS		:=	main.c
OBJ_DIR		:=	./obj
OBJS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))

LIBFT_PATH	:=	./libft
INCLUDE		:=	-I ./libft/inc -I ./inc

all:$(NAME) 

$(NAME): $(OBJS) LIBFT
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft -o $(NAME)
	@echo "[FDF] Build Completed"

LIBFT:
	@make --no-print-directory -C $(LIBFT_PATH) all

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@make --no-print-directory -C $(LIBFT_PATH) clean
	@rm -rf $(OBJ_DIR)
	@echo "[FDF] Objects Removed"

fclean: clean
	@make --no-print-directory -C $(LIBFT_PATH) fclean
	@echo "[FDF] Everything Removed"
	@echo "========Project Reset========"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re LIBFT
