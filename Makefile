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
	  show_alloc_mem.c \
	  utils.c

SRC_PATH = src
vpath %.c $(SRC_PATH)

SRCS = $(addprefix $(SRC_PATH), $(SRC))

OBJ_PATH = obj
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC:%.c=%.o))

FLAGS = -Wall -Wextra -Werror -fPIC

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -shared -o $@ $^
	@rm -f libft_malloc.so
	@ln -sf $@ libft_malloc.so
	@echo "Library $@ created"

$(OBJ_PATH)/%.o: %.c $(INC)
	@mkdir -p obj
	$(CC) $(FLAGS) $(PREP_INCS) -c $< -o $@

clean:
	@rm -rf obj
	@echo "objects deleted"

fclean: clean
	@rm -f $(NAME) libft_malloc.so
	@echo "lib deleted"

re: fclean all
