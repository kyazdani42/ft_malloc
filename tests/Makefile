NAME = run_tests
CC = gcc

INC = ../includes/
PREP_INCS = -I$(INC)

SRC = malloc.c \
	  free.c \
	  realloc.c \
	  show_alloc_mem.c

PATH_SRC = ../src/
SRCS = $(addprefix $(PATH_SRC), $(SRC)) main.c

FLAGS = -Wall -Wextra

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): main.c
	$(CC) $(FLAGS) $(PREP_INCS) $(SRCS) -o $@
	@echo "Library $@ created"

clean:
	@rm -f $(NAME)
	@echo "Library deleted"

fclean: clean

re: fclean all
