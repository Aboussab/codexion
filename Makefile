NAME = codexion
CFLAGS = -Wall -Werror -fsanitize=thread -Wextra -pthread
CC = cc
SRC = dongles.c inisialization.c log_coder_routine.c \
	main.c parsing_and_error.c push_bubble_pop.c \
	clean_up.c \

OBJS = $(SRC:.c=.o)
H_FILE = $(NAME).h

all : $(NAME)

%.o : %.c $(H_FILE)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean : 
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all 

.PHONY: all fclean clean re 