# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/22 17:07:31 by msuarez-          #+#    #+#              #
#    Updated: 2019/12/05 17:57:59 by msuarez-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a

SRCS=ft_strlcat.c ft_strnew.c ft_strcpy.c ft_strcat.c ft_strchr.c \
	ft_strdup.c ft_strncat.c ft_strncpy.c ft_strnstr.c ft_strstr.c \
	ft_strmap.c ft_strmapi.c ft_strjoin.c ft_strsub.c ft_itoa.c \
	ft_strtrim.c ft_strdel.c ft_strclr.c ft_striter.c ft_strrchr.c \
	ft_striteri.c ft_memdel.c ft_bzero.c ft_memset.c ft_memalloc.c \
	ft_memcpy.c ft_memccpy.c ft_memmove.c ft_memchr.c ft_strequ.c \
	ft_strnequ.c ft_strncmp.c ft_strlen.c ft_strcmp.c ft_atoi.c \
	ft_memcmp.c ft_isalpha.c ft_islower.c ft_isupper.c ft_isdigit.c \
	ft_isalnum.c ft_toupper.c ft_tolower.c ft_isprint.c ft_isascii.c \
	ft_putchar.c ft_putchar_fd.c ft_putnbr.c ft_putnbr_fd.c ft_putstr.c \
	ft_putstr_fd.c ft_putendl.c ft_putendl_fd.c ft_strsplit.c \
	ft_isspace.c ft_intlen.c ft_abs.c ft_lstnew.c ft_lstdelone.c \
	ft_lstdel.c ft_lstadd.c ft_lstiter.c ft_lstmap.c get_next_line.c

OBJECTS= ft_strlcat.o ft_strnew.o ft_strcpy.o ft_strcat.o ft_strchr.o \
	ft_strdup.o ft_strncat.o ft_strncpy.o ft_strnstr.o ft_strstr.o \
	ft_strmap.o ft_strmapi.o ft_strjoin.o ft_strsub.o ft_itoa.o \
	ft_strtrim.o ft_strdel.o ft_strclr.o ft_striter.o ft_strrchr.o \
	ft_striteri.o ft_memdel.o ft_bzero.o ft_memset.o ft_memalloc.o \
	ft_memcpy.o ft_memccpy.o ft_memmove.o ft_memchr.o ft_strequ.o \
	ft_strnequ.o ft_strncmp.o ft_strlen.o ft_strcmp.o ft_atoi.o \
	ft_memcmp.o ft_isalpha.o ft_islower.o ft_isupper.o ft_isdigit.o \
	ft_isalnum.o ft_toupper.o ft_tolower.o ft_isprint.o ft_isascii.o \
	ft_putchar.o ft_putchar_fd.o ft_putnbr.o ft_putnbr_fd.o ft_putstr.o \
	ft_putstr_fd.o ft_putendl.o ft_putendl_fd.o ft_strsplit.o \
	ft_isspace.o ft_intlen.o ft_abs.o ft_lstnew.o ft_lstdelone.o \
	ft_lstdel.o ft_lstadd.o ft_lstiter.o ft_lstmap.o get_next_line.o

all: $(NAME)

$(NAME): $(SRCS) libft.h get_next_line.h
	@gcc -Wall -Wextra -Werror -c $(SRCS)
	@ar rc $(NAME) $(OBJECTS)
	@ranlib $(NAME)

clean:
	@/bin/rm -f $(OBJECTS)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
