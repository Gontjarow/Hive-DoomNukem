#	https://www.gnu.org/software/make/manual/html_node/Flavors.html
NAME		:=	doom-nukem

#	http://nuclear.mutantstargoat.com/articles/make/#multiple-source-directories
SOURCES		:=	$(wildcard *.c) $(wildcard renderer/*.c)
OBJECTS		:=	$(SOURCES:.c=.o)

#	http://nuclear.mutantstargoat.com/articles/make/#automatic-include-dependency-tracking
DEPENDENCY	:=	$(OBJECTS:.o=.d)

LIBFT_PATH	:= ./libft
LIBFT		:= -I $(LIBFT_PATH) -L $(LIBFT_PATH) $(LIBFT_PATH)/libft.a

#	http://nuclear.mutantstargoat.com/articles/make/#handling-cross-platform-differences
#	https://www.gnu.org/software/make/manual/html_node/Shell-Function.html
SHELL_NAME	:= $(shell uname -s)

ifeq ($(SHELL_NAME), Darwin)
SDL_PATH	:= ./lib/SDL2
SDL_MAIN	:= -I $(SDL_PATH)/SDL2.framework/Headers -framework SDL2 -F $(SDL_PATH)
SDL_IMAGE	:= -I $(SDL_PATH)/SDL2_image.framework/Headers -framework SDL2_image -F $(SDL_PATH)
SDL_MIXER	:= -I $(SDL_PATH)/SDL2_mixer.framework/Headers -framework SDL2_mixer -F $(SDL_PATH)
else
SDL_PATH	:= ./libsdl
SDL_MAIN	:= -I $(SDL_PATH) -L $(SDL_PATH) $(SDL_PATH)/libSDL2.a
SDL_IMAGE	:= -I $(SDL_PATH) -L $(SDL_PATH) $(SDL_PATH)/libSDL2_image.a
SDL_MIXER	:= -I $(SDL_PATH) -L $(SDL_PATH) $(SDL_PATH)/libSDL2_mixer.a
endif

#	http://nuclear.mutantstargoat.com/articles/make/#improved-automatic-dependency-tracking
FLAGS		:= -Wall -Wextra -MMD -g
INCLUDES	:= -I . -I ./renderer

FLAGS		+= $(INCLUDES) $(LIBFT) $(SDL_MAIN) $(SDL_IMAGE) $(SDL_MIXER)

MSG = \033[38;5;214m
END = \033[0m

.PHONY: all clean fclean re

#	SDL framework requires that it exists in Library. (See SDL README)
#	Only if frameworks are MISSING, copy them. Otherwise cp outputs errors.
#	https://stackoverflow.com/a/20566812/12215093
all: $(NAME)
ifeq ($(SHELL_NAME), Darwin)
ifeq ("$(wildcard ~/Library/Frameworks/SDL2.framework)","")
	@cp -R ./lib/SDL2/SDL2.framework ~/Library/Frameworks
	@cp -R ./lib/SDL2/SDL2_image.framework ~/Library/Frameworks
	@cp -R ./lib/SDL2/SDL2_mixer.framework ~/Library/Frameworks
	@echo "$(MSG)SDL installed!$(END)"
endif
else
	@echo "$(MSG)This isn't MacOS. (debug message)$(END)"
endif

#	https://www.gnu.org/software/make/manual/html_node/Include.html
-include $(DEPENDENCY)

$(NAME): $(LIBFT_PATH)/libft.a $(OBJECTS)
	@gcc $(FLAGS) $(OBJECTS) -o $(NAME)
	@echo "$(MSG)Done! (Shell: $(SHELL_NAME))$(END)"

$(LIBFT_PATH)/libft.a:
	@make -C libft

%.o: %.c
	@printf "gcc %25s ==> %s\n" $< $@
	@gcc $(FLAGS) -w -c $< -o $@

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
