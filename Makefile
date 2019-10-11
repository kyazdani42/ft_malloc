ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc

INC = includes/
PREP_INCS = -I$(INC)

SRC = malloc.c \
	  free.c \
	  realloc.c \
	  show_alloc_mem.c

PATH_SRC = ./src/
SRCS = $(addprefix $(PATH_SRC), $(SRC))

FLAGS_SO = -fPIC -shared
FLAGS = -Wall -Wextra

ifndef NOERR
	FLAGS += -Werror
endif


.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(INC) $(SRCS)
	$(CC) $(FLAGS) $(PREP_INCS) $(SRCS) $(FLAGS_SO) -o $@
	@echo "Library $@ created"

clean:
	@rm -f $(NAME)
	@echo "Library deleted"

fclean: clean

re: fclean all
