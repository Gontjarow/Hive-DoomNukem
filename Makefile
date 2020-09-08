NAME = doom_nukem

SOURCES =	doom_nukem.c \
			input.c \
			image.c

OBJECTS = $(subst .c,.o,$(SOURCES))

LIBFT = libft/libft.a

FLAGS = -Wall -Wextra -g
LINKS = -I libft -L libft -l ft \
		-I ./mlx -L ./mlx -l mlx \
		-framework OpenGL -framework Appkit

LINUX_LINKS = -I libft -L libft -l ft \
		-I ./mlx -L ./mlx -l mlx \
		-lm -lXext -lX11

MSG = \033[38;5;214m
END = \033[0m

.PHONY: all clean fclean re linux

all: $(NAME)

$(NAME): $(OBJECTS)
	@gcc $(OBJECTS) -o $(NAME) $(FLAGS) $(LINKS)
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
