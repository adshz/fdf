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

# Colors
DF = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
MAGENTA = \033[0;95m
YELLOW = \033[0;93m
CYAN = \033[0;96m
WHITE = \033[0;97m
BOLD = \033[1m
ORANGE = \033[38;5;208m

# Project
NAME		:= fdf
CC		:= cc
CFLAGS		:= -Wall -Wextra -Werror -g

# Directories
SRC_DIR		:= ./src
OBJ_DIR		:= ./obj
LIBFT_PATH	:= ./libft
LIBFT_REPO	:= https://github.com/adshz/libft.git

# MLX42
MLX42_DIR	:= ./MLX42
MLX42_REPO	:= https://github.com/codam-coding-college/MLX42.git
MLX42_BUILD	:= $(MLX42_DIR)/build
MLX42_LIB	:= $(MLX42_BUILD)/libmlx42.a

# GLFW (built from source if not available)
GLFW_DIR	:= ./glfw
GLFW_REPO	:= https://github.com/glfw/glfw.git
GLFW_BUILD	:= $(GLFW_DIR)/build
GLFW_LIB	:= $(GLFW_BUILD)/src/libglfw3.a

# OS Detection
UNAME_S := $(shell uname -s)

# Check if system GLFW is available
SYSTEM_GLFW := $(shell pkg-config --exists glfw3 2>/dev/null && echo "yes" || echo "no")

# Platform-specific configuration
ifeq ($(UNAME_S),Darwin)
    # macOS (Intel and Apple Silicon)
    ifeq ($(SYSTEM_GLFW),yes)
        GLFW_FLAGS := $(shell pkg-config --libs glfw3)
        GLFW_INC := $(shell pkg-config --cflags glfw3)
        USE_LOCAL_GLFW := no
    else
        GLFW_FLAGS := -L$(GLFW_BUILD)/src -lglfw3
        GLFW_INC := -I$(GLFW_DIR)/include
        USE_LOCAL_GLFW := yes
    endif
    MLX_FLAGS := -L$(MLX42_BUILD) -lmlx42 $(GLFW_FLAGS) -framework Cocoa -framework OpenGL -framework IOKit -framework QuartzCore
    PLATFORM := macOS
else
    # Linux
    ifeq ($(SYSTEM_GLFW),yes)
        GLFW_FLAGS := $(shell pkg-config --libs glfw3)
        GLFW_INC := $(shell pkg-config --cflags glfw3)
        USE_LOCAL_GLFW := no
    else
        GLFW_FLAGS := -L$(GLFW_BUILD)/src -lglfw3
        GLFW_INC := -I$(GLFW_DIR)/include
        USE_LOCAL_GLFW := yes
    endif
    MLX_FLAGS := -L$(MLX42_BUILD) -lmlx42 $(GLFW_FLAGS) -ldl -pthread -lm
    PLATFORM := Linux
endif

# Sources
SRCS		:= \
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

OBJS		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(SRCS)))

# Libraries and includes
ALL_LIBS	:= -L$(LIBFT_PATH) -lft $(MLX_FLAGS)
INCLUDE		:= -I./libft/inc -I./inc -I$(MLX42_DIR)/include $(GLFW_INC)

# ============================================================================ #
#                                   RULES                                      #
# ============================================================================ #

# Check for required tools
check_cmake:
	@command -v cmake >/dev/null 2>&1 || { \
		echo "$(RED)[FDF] Error: cmake is required but not installed.$(DF)"; \
		echo "$(YELLOW)Install it with:$(DF)"; \
		if [ "$(UNAME_S)" = "Darwin" ]; then \
			echo "  brew install cmake"; \
		else \
			echo "  sudo apt-get install cmake    # Debian/Ubuntu"; \
			echo "  sudo dnf install cmake        # Fedora"; \
			echo "  sudo pacman -S cmake          # Arch"; \
		fi; \
		exit 1; \
	}

all: check_cmake deps $(NAME)

# Download and build dependencies
deps: glfw mlx42 libft

# Clone and build GLFW (only if system GLFW not available)
glfw:
ifeq ($(USE_LOCAL_GLFW),yes)
	@if [ ! -f "$(GLFW_LIB)" ]; then \
		if [ ! -d "$(GLFW_DIR)" ]; then \
			echo "$(YELLOW)[FDF] Downloading GLFW...$(DF)"; \
			git clone --depth 1 $(GLFW_REPO) $(GLFW_DIR); \
		fi; \
		echo "$(YELLOW)[FDF] Building GLFW...$(DF)"; \
		cmake -S $(GLFW_DIR) -B $(GLFW_BUILD) \
			-DGLFW_BUILD_EXAMPLES=OFF \
			-DGLFW_BUILD_TESTS=OFF \
			-DGLFW_BUILD_DOCS=OFF \
			-DGLFW_INSTALL=OFF > /dev/null 2>&1; \
		cmake --build $(GLFW_BUILD) -j4 > /dev/null 2>&1; \
		echo "$(GREEN)[FDF] GLFW ready$(DF)"; \
	fi
else
	@echo "$(GREEN)[FDF] Using system GLFW$(DF)"
endif

# Clone and build MLX42
mlx42: glfw $(MLX42_LIB)

$(MLX42_LIB):
	@if [ ! -d "$(MLX42_DIR)" ]; then \
		echo "$(YELLOW)[FDF] Downloading MLX42...$(DF)"; \
		git clone $(MLX42_REPO) $(MLX42_DIR); \
	fi
	@echo "$(YELLOW)[FDF] Building MLX42...$(DF)"
ifeq ($(USE_LOCAL_GLFW),yes)
	@cmake -S $(MLX42_DIR) -B $(MLX42_BUILD) -DDEBUG=1 \
		-DGLFW_FETCH=OFF \
		-Dglfw3_DIR=$(GLFW_BUILD)/src > /dev/null 2>&1 || \
		cmake -S $(MLX42_DIR) -B $(MLX42_BUILD) -DDEBUG=1 > /dev/null 2>&1
else
	@cmake -S $(MLX42_DIR) -B $(MLX42_BUILD) -DDEBUG=1 > /dev/null 2>&1
endif
	@cmake --build $(MLX42_BUILD) -j4 > /dev/null 2>&1
	@echo "$(GREEN)[FDF] MLX42 ready$(DF)"

libft:
	@if [ ! -d "$(LIBFT_PATH)" ] || [ ! -f "$(LIBFT_PATH)/Makefile" ]; then \
		echo "$(YELLOW)[FDF] Downloading libft...$(DF)"; \
		git clone $(LIBFT_REPO) $(LIBFT_PATH); \
	fi
	@$(MAKE) --no-print-directory -C $(LIBFT_PATH) all
	@echo "$(GREEN)[FDF] libft ready$(DF)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(ALL_LIBS) -o $(NAME)
	@echo "$(BOLD)$(GREEN)[FDF] Build completed for $(PLATFORM)$(DF)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "$(GRAY)[FDF] Compiling $<$(DF)"

# Quick run target
run: all
	@./fdf ./maps/42.fdf

# Clean object files
clean:
	@$(MAKE) --no-print-directory -C $(LIBFT_PATH) clean 2>/dev/null || true
	@rm -rf $(OBJ_DIR)
	@echo "$(CYAN)[FDF] Objects removed$(DF)"

# Full clean including binary
fclean: clean
	@$(MAKE) --no-print-directory -C $(LIBFT_PATH) fclean 2>/dev/null || true
	@rm -f $(NAME)
	@echo "$(BOLD)$(RED)[FDF] Everything removed$(DF)"

# Clean including downloaded dependencies
mrproper: fclean
	@rm -rf $(MLX42_DIR)
	@rm -rf $(GLFW_DIR)
	@rm -rf $(LIBFT_PATH)
	@echo "$(ORANGE)[FDF] All dependencies removed$(DF)"

re: fclean all

# Show current configuration
info:
	@echo "$(BOLD)$(CYAN)FDF Build Configuration$(DF)"
	@echo "$(YELLOW)Platform:$(DF)      $(PLATFORM)"
	@echo "$(YELLOW)Compiler:$(DF)      $(CC)"
	@echo "$(YELLOW)Flags:$(DF)         $(CFLAGS)"
	@echo "$(YELLOW)MLX42 Dir:$(DF)     $(MLX42_DIR)"
	@echo "$(YELLOW)System GLFW:$(DF)   $(SYSTEM_GLFW)"
	@echo "$(YELLOW)Local GLFW:$(DF)    $(USE_LOCAL_GLFW)"

.PHONY: all check_cmake deps glfw mlx42 libft clean fclean mrproper re run info
