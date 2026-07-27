NAME = codexion

CC = cc
FLAGS = -Wall -Werror -Wextra -g -Icoders -MMD -MP

SRC_DIR = coders
OBJ_DIR = objects

SRC = parse_args.c utilities_a.c sim_routines.c monitor_routines.c \
heap_routines.c heap_routines_b.c dongles_routines.c coder_routines.c \
codexion.c 

SRCS = $(SRC:%.c=$(SRC_DIR)/%.c)
OBJ = $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEP = $(OBJ:%.o=%.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@


-include $(DEP)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
