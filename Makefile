#	https://www.gnu.org/software/make/manual/html_node/Flavors.html
NAME		:=	doom-nukem

#	http://nuclear.mutantstargoat.com/articles/make/#multiple-source-directories
SOURCES		:=	$(wildcard *.c) $(wildcard renderer/*.c) $(wildcard editor/*.c) $(wildcard game/*.c) $(wildcard commons/*.c)
OBJECTS		:=	$(SOURCES:.c=.o)

#	http://nuclear.mutantstargoat.com/articles/make/#automatic-include-dependency-tracking
DEPENDENCY	:=	$(OBJECTS:.o=.d)

LIBFT_PATH	:= ./libft
LIBFT		:= -I $(LIBFT_PATH) -L $(LIBFT_PATH) -l ft

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
SDL_MAIN	:= -I $(SDL_PATH) -L $(SDL_PATH) -l SDL2
SDL_IMAGE	:= -I $(SDL_PATH) -L $(SDL_PATH) -l SDL2_image
SDL_MIXER	:= -I $(SDL_PATH) -L $(SDL_PATH) -l SDL2_mixer
endif

#	http://nuclear.mutantstargoat.com/articles/make/#improved-automatic-dependency-tracking
FLAGS		:= -Wall -Wextra -MMD -g
INCLUDES	:= -I . -I ./renderer -I ./editor -I ./commons

FLAGS		+= $(INCLUDES) $(LIBFT) $(SDL_MAIN) $(SDL_IMAGE) $(SDL_MIXER)

ifeq ($(SHELL_NAME), Linux)
FLAGS		+= -lm -ldl -pthread
endif

MSG = \033[38;5;214m
END = \033[0m

.PHONY: all clean fclean re

#	SDL framework requires that it exists in Library. (See SDL README)
#	Only if frameworks are MISSING, copy them. Otherwise cp outputs errors.
#	https://stackoverflow.com/a/20566812/12215093
all: $(NAME)
ifeq ($(SHELL_NAME), Darwin)
	@mkdir -p ~/Library/Frameworks
ifeq ("$(wildcard ~/Library/Frameworks/SDL2.framework)","")
	@cp -R ./lib/SDL2/SDL2.framework ~/Library/Frameworks
	@cp -R ./lib/SDL2/SDL2_image.framework ~/Library/Frameworks
	@cp -R ./lib/SDL2/SDL2_mixer.framework ~/Library/Frameworks
	@echo "$(MSG)SDL installed!$(END)"
endif
endif

#	https://www.gnu.org/software/make/manual/html_node/Include.html
-include $(DEPENDENCY)

$(NAME): $(LIBFT_PATH)/libft.a $(OBJECTS)
	@gcc $(OBJECTS) -o $(NAME) $(FLAGS)
	@echo "$(MSG)Done! (Shell: $(SHELL_NAME))$(END)"

prod: $(LIBFT_PATH)/libft.a $(OBJECTS)
	@gcc $(OBJECTS) -o $(NAME) $(FLAGS) -DNDEBUG
	@echo "$(MSG)Done! Assert calls were disabled! (Shell: $(SHELL_NAME))$(END)"

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
