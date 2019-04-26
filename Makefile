# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maki <maki@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/23 22:00:58 by ysan-seb          #+#    #+#              #
#    Updated: 2019/04/26 13:32:15 by maki             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

C_NAME = client
S_NAME = server

C_SRCS = ftp_client.c
S_SRCS = ftp_server.c \
		 ftp_usage.c \
		 ftp_error.c \
		 ftp_ls.c\
		 ftp_cd.c \
		 ftp_pwd.c \

C_OBJS = $(C_SRCS:.c=.o)
S_OBJS = $(S_SRCS:.c=.o)

CC = gcc

FLAGS = -Wall -Werror -Wextra -g

HEADER = -I./inc

#LIBFT = libft/libft.a

RM = rm -rf

all	: lib $(C_NAME) $(S_NAME)

lib	:
#	@make -C libft/

$(C_NAME):$(C_OBJS)
	$(CC) $(FLAGS) -o $(C_NAME) $(C_OBJS) #$(LIBFT)

$(S_NAME):$(S_OBJS)
	$(CC) $(FLAGS) -o $(S_NAME) $(S_OBJS) #$(LIBFT)
%.o: %.c
	gcc $(FLAGS) -o $@ -c  $< #$(HEADER)

clean	:
#	@make -C libft/ clean
	$(RM) $(C_OBJS) $(S_OBJS)

fclean: clean
#	@make -C libft/ fclean
	$(RM) $(C_NAME) $(S_NAME)

re	:	fclean
	make

.PHONY: all lib clean fclean re
