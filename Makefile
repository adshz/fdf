# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 14:14:29 by szhong            #+#    #+#              #
#    Updated: 2024/08/14 16:05:02 by szhong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	fdf
CC		:=	cc 
CFLAGS		:=	-Wall -Werror -Werror -Wno-unused-function -g
MLX		:=	-I ./minlibx-linux 
SRC_DIR		:=	./src
SRCS		:=	\
			main.c \
			utils/debug/debug.c \
			utils/parse_file/get_whole_file.c \
			utils/error_and_free.c \
			utils/init.c \
			utils/parse_file/parse_utils.c \
			utils/parse_file/parse_data.c \
			utils/parse_file/parse_line.c \
			utils/utils.c \
			utils/key_handler.c \
			utils/render_data.c \
			utils/rotate.c \
			utils/project.c \
			utils/transform.c \
			utils/colour_handler.c \
			utils/prompt.c

OBJ_DIR		:=	./obj
OBJS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))

LIBFT_PATH	:=	./libft
ALL_LIBS	:=	-L$(LIBFT_PATH) -lft -L./minilibx-linux -lmlx -lXext -lX11 -lm
INCLUDE		:=	-I ./libft/inc -I ./inc $(MLX)
TAG_FILE	:=	tags
CTAG_FILES := $(shell find src -name '*.c') $(shell find libft/src -name '*.c') $(shell find inc -name '*.h') $(shell find libft/inc -name '*.h')

all:$(NAME) 

$(NAME): $(OBJS) LIBFT ctags
	@$(CC) $(CFLAGS) $(OBJS) $(ALL_LIBS) -o $(NAME) 
	@echo "[FDF] Build Completed"

LIBFT:
	@make --no-print-directory -C $(LIBFT_PATH) all

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

mlx_lib:
	@make -no-print-directory -C ./minilibx-linux ./configure

ctags: $(CTAG_FILES)
	@ctags -R --exclude=.git --exclude=build --languages=c,c++ -f $(TAG_FILE) .

$(TAGS_DIR):
	@mkdir -p TAGS_FILE

run:
	@./fdf ./maps/42.fdf

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

.PHONY: all clean fclean re LIBFT ctags
