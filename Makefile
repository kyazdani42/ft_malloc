ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc

INC = includes/ libft/includes/
PREP_INCS = -Iincludes/ -Ilibft/includes/

LIB = libft/
LIBFT_STATIC = libft/libft.a

SRC = malloc.c \
	  free.c \
	  realloc.c
PATH_SRC = srcs
SRCS = $(addprefix $(PATH_SRC)/, $(SRC))

FLAGS = -Wall -Werror -Wextra -fPIC -pedantic
FLAGS_SO = -shared -o

.PHONY: all makelib clean fclean re

all: makelib $(NAME)

$(NAME): $(INC)
	@$(CC) $(FLAGS) $(PREP_INCS) $(SRCS) $(LIBFT_STATIC) $(FLAGS_SO) $@
	@echo "Library $@ created"

makelib:
	@make -C $(LIB)

clean:
	@rm -f $(NAME)
	@echo "Library deleted"

fclean: clean
	@make fclean -C $(LIB)

re: fclean all
