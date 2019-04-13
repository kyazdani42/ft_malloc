ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc

INC = includes/
PREP_INCS = -Iincludes/

SRC = malloc.c \
	  free.c \
	  realloc.c

PATH_SRC = ./src/
SRCS = $(addprefix $(PATH_SRC), $(SRC))

FLAGS = -Wall -Werror -Wextra -fPIC -pedantic
FLAGS_SO = -shared -o

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(INC)
	$(CC) $(FLAGS) $(PREP_INCS) $(SRCS) $(FLAGS_SO) $@
	@echo "Library $@ created"

clean:
	@rm -f $(NAME)
	@echo "Library deleted"

fclean: clean

re: fclean all
