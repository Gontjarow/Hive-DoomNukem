# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/08 19:59:39 by msuarez-          #+#    #+#              #
#    Updated: 2020/09/11 13:29:56 by msuarez-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = doom-nukem

SOURCES = doom_nukem.c input.c image.c texture.c

OBJECTS = $(subst .c,.o,$(SOURCES))

LIBFT = libft/libft.a

FLAGS = -Wall -Wextra -Werror

LINUX_LINKS = -I libft -L libft -l ft \
		-I ./mlx -L ./mlx -l mlx \
		-lm -lXext -lX11 -lSDL2 -lSDL2_mixer -lSDL2_image

MSG = \033[38;5;214m
END = \033[0m

.PHONY: all clean fclean re linux

all: $(NAME)

$(NAME): $(OBJECTS)
	@gcc $(FLAGS) -Imlx/mlx.h $(SOURCES) $(LIBFT) \
	-lmlx -lSDL2 -lSDL2_mixer -lSDL2_image \
	-framework OpenGL -framework AppKit -o $(NAME)
	@echo "$(MSG)Done!$(END)"

linux: $(OBJECTS)
	@gcc $(OBJECTS) -o $(NAME) $(FLAGS) $(LINUX_LINKS)
	@echo "$(MSG)Done!$(END)"

$(OBJECTS): $(LIBFT) $(SOURCES)
	@echo "$(MSG)Compiling...$(END)"
	@gcc $(FLAGS) -c $(SOURCES)

$(LIBFT):
	@make -C libft

clean:
	@make -C libft clean
	@rm -f $(OBJECTS)
	@echo "$(MSG)Objects removed!$(END)"

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)
	@echo "$(MSG)Targets removed!$(END)"

re: fclean all
