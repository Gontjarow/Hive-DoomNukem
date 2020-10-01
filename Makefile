NAME		=	doom-nukem

SOURCES		=	$(wildcard *.c) \
				$(wildcard renderer/*.c)

OBJECTS		=	$(SOURCES:.c=.o)

DEPENDENCY	=	$(OBJECTS:.o=.d)

LIBFT		= libft/libft.a

UNIX_SDL	= ./libsdl
UNIX_SDL2	= $(UNIX_SDL)/libSDL2.a
UNIX_SDLIMG	= $(UNIX_SDL)/libSDL2_image.a
UNIX_SDLMIX	= $(UNIX_SDL)/libSDL2_mixer.a

OSX_SDL		= ./lib/SDL2
OSX_SDL2	= $(OSX_SDL)/SDL2.framework/Headers -framework SDL2 -F $(OSX_SDL)/
OSX_SDLIMG	= $(OSX_SDL)/SDL2_image.framework/Headers -framework SDL2_image -F$(OSX_SDL)/
OSX_SDLMIX	= $(OSX_SDL)/SDL2_mixer.framework/Headers -framework SDL2_mixer -F$(OSX_SDL)/

FLAGS		= -Wall -Wextra -g -MMD
INCLUDES	= -I . -I ./renderer -rpath .
LINKS		= -L libft -l ft

OSX_FLAGS	= $(FLAGS) $(INCLUDES) -I$(OSX_SDL2) -I$(OSX_SDLIMG) -I$(OSX_SDLMIX) $(LINKS)

MSG = \033[38;5;214m
END = \033[0m

.PHONY: all clean fclean re

all: $(NAME)

-include $(DEPENDENCY)

$(NAME): $(OBJECTS) $(LIBFT)
	@gcc $(OBJECTS) $(OSX_FLAGS) -o $(NAME)

$(LIBFT):
	@make -C libft

%.o: %.c
	@gcc $(FLAGS) -w $(OSX_FLAGS) -o $@ -c $<

clean:
	@make -C libft clean
	@rm -f $(OBJECTS)
	@rm -f $(DEPENDENCY)
	@echo "$(MSG)Objects removed!$(END)"

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)
	@echo "$(MSG)Targets removed!$(END)"

re: fclean all
