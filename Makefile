C_NAME = client
S_NAME = server

C_SRCS =	tcp_client.c
S_SRCS =	tcp_server.c

C_OBJS = $(C_SRCS:.c=.o)
S_OBJS = $(S_SRCS:.c=.o)

CC = gcc

FLAGS = -Wall -Werror -Wextra -g

# HEADER = -I./inc

RM = rm -rf

all	: $(C_NAME)

$(C_NAME):$(C_OBJS)
	$(CC) $(FLAGS) -o $(C_NAME) $(C_OBJS) #$(HEADER)

all	: $(S_NAME)

$(S_NAME):$(S_OBJS)
	$(CC) $(FLAGS) -o $(S_NAME) $(S_OBJS) #$(HEADER)

%.o: %.c
	gcc $(FLAGS) -o $@ -c  $< #$(HEADER)

clean	:
	$(RM) $(C_OBJS) $(S_OBJS)

fclean: clean
	$(RM) $(C_NAME) $(S_NAME)

re	:	fclean
	make

.PHONY: all lib clean fclean re