NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS = 

OBJS = $(SRCS:.c=.o)
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

debug : CFLAGS += -g
debug : fclean $(NAME)

address_sanitize : CFLAGS += -fsanitize=address -g
address_sanitize : fclean $(NAME)

thread_sanitize : CFLAGS += -fsanitize=thread -g
thread_sanitize : fclean $(NAME)


.PHONY : clean fclean all re debug
.SECONDARY : $(OBJS)
