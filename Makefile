# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: szhong <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 14:14:29 by szhong            #+#    #+#              #
#    Updated: 2024/08/19 21:07:47 by szhong           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
#Define colour
DF = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
MAGENTA = \033[0;95m
YELLOW = \033[0;93m
CYAN =\033[0;96m
WHITE = \033[0;97m
BOLD = \033[91m
ORANGE = \033[38;5;208m

NAME		:=	fdf
CC		:=	cc 
CFLAGS		:=	-Wall -Werror -Werror -g
MLX		:=	-I ./minlibx-linux 
SRC_DIR		:=	./src
SRCS		:=	\
			main.c \
			modules/ft_colour/colour_handler.c \
			modules/ft_colour/colour_utils.c \
			modules/ft_init/init.c \
			modules/ft_init/init_utils.c \
			modules/ft_interact/key_functions.c \
			modules/ft_interact/key_handler.c \
			modules/ft_interact/prompt.c \
			modules/ft_parse/get_whole_file.c \
			modules/ft_parse/parse_utils.c \
			modules/ft_parse/parse_data.c \
			modules/ft_parse/parse_line.c \
			modules/ft_render/render_data.c \
			modules/ft_render/draw_slope.c \
			modules/ft_render/render_utils.c \
			modules/ft_transform/rotate.c \
			modules/ft_transform/project.c \
			modules/ft_transform/view_transform.c \
			modules/ft_utils/error_and_free.c \
			modules/ft_utils/win_free.c

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
	@echo "$(BOLD)$(YELLOW)[FDF] $(GREEN)Build Completed$(DF)"

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
	@echo "$(CYAN)[FDF] Objects Removed$(DF)"

fclean: clean
	@make --no-print-directory -C $(LIBFT_PATH) fclean
	@echo "$(BOLD)$(RED)[FDF] Everything Removed$(DF)"
	@echo "$(BOLD)$(ORANGE)========Project Reset========$(DF)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re LIBFT ctags
