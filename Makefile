# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/08 19:59:39 by msuarez-          #+#    #+#              #
#    Updated: 2020/09/25 13:36:21 by msuarez-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = doom-nukem

SOURCES = doom_nukem.c input.c texture.c alphabet.c menu.c editor.c \
		line.c pixel.c vec2d.c game.c mapfile.c load_model.c sounds.c \
		minimap.c 

OBJECTS = $(subst .c,.o,$(SOURCES))

LIBFT = libft/libft.a

SDL2 = libsdl/libSDL2.a

SDL2_IMAGE = libsdl/libSDL2_image.a

SDL2_MIXER = libsdl/libSDL2_mixer.a

FLAGS = -Wall -Wextra

LINUX_LINKS = -I libft -L libft -l ft \
		-I ./mlx -L ./mlx -l mlx \
		-lm -lXext -lX11 -lpthread \
        -I ./libsdl -L ./libsdl -ldl

LIBSDL2 = ./lib/SDL2

MAC_INCLUDES =	-I $(LIBSDL2)/SDL2.framework/Headers \
				-I $(LIBSDL2)/SDL2_image.framework/Headers \
				-I $(LIBSDL2)/SDL2_mixer.framework/Headers

MAC_FLAGS =		-framework SDL2 -F$(LIBSDL2)/ \
				-framework SDL2_image -F$(LIBSDL2)/ \
				-framework SDL2_mixer -F$(LIBSDL2)/

MSG = \033[38;5;214m
END = \033[0m

.PHONY: all clean fclean re linux

all: $(NAME)

$(NAME): $(OBJECTS)
	@gcc $(OBJECTS) $(FLAGS) -I libft -L libft -l ft -rpath $(LIBSDL2) $(MAC_FLAGS) $(MAC_INCLUDES) -o $(NAME)
	@echo "$(MSG)Done!$(END)"

linux: $(OBJECTS)
	@gcc $(OBJECTS) -o $(NAME) $(FLAGS) $(SDL2) $(SDL2_IMAGE) $(SDL2_MIXER) $(LINUX_LINKS)
	@echo "$(MSG)Done!$(END)"

# RPATH necessary for Mac OSX compiled binary to execute
# TEST IF -WERROR CAN BE ENABLED BACK AFTER SEPARATING OBJECTS WHICH HAVE NO NEED FOR SDL STUFF TO BE WITHOUT $(MAC_FLAGS) && $(MAC_INCLUDES)

$(OBJECTS): $(LIBFT) $(SOURCES)
	@echo "$(MSG)Compiling...$(END)"
	@gcc $(FLAGS) -I libft $(MAC_FLAGS) $(MAC_INCLUDES) -c $(SOURCES)

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
