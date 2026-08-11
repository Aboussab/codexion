NAME = codexion
CFLAGS = -Wall -Werror -fsanitize=thread -Wextra  -g 
#
CC = cc
SRC = clean_up.c   dongles.c inisialization.c  main.c\
	parsing_and_error.c coders.c display.c coders_thread.c\
	stop_simulation.c\

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