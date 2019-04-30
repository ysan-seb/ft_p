# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maki <maki@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/23 22:00:58 by ysan-seb          #+#    #+#              #
#    Updated: 2019/04/30 19:06:42 by ysan-seb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

C_NAME = client
S_NAME = server

SRC_PATH = ./src/

C_SRCS = $(SRC_PATH)ftp_client.c \
		 $(SRC_PATH)ftp_client_prompt.c \
		 $(SRC_PATH)ftp_usage.c \
		 $(SRC_PATH)ftp_error.c \
		 $(SRC_PATH)ftp_arg.c \
		 $(SRC_PATH)ftp_local_command.c \
		 $(SRC_PATH)ftp_get_file.c \
		 $(SRC_PATH)ftp_send_file.c \
		 $(SRC_PATH)ftp_open_file.c \
		 $(SRC_PATH)ftp_request_status.c \
		 $(SRC_PATH)ftp_listen_status.c \
		 $(SRC_PATH)ftp_get_file_header.c \
		 $(SRC_PATH)ftp_quit.c \

S_SRCS = $(SRC_PATH)ftp_server.c \
		 $(SRC_PATH)ftp_fork.c \
		 $(SRC_PATH)ftp_usage.c \
		 $(SRC_PATH)ftp_error.c \
		 $(SRC_PATH)ftp_signal.c \
		 $(SRC_PATH)ftp_arg.c \
		 $(SRC_PATH)ftp_ls.c \
		 $(SRC_PATH)ftp_cd.c \
		 $(SRC_PATH)ftp_pwd.c \
		 $(SRC_PATH)ftp_get_file.c \
		 $(SRC_PATH)ftp_send_file.c \
		 $(SRC_PATH)ftp_open_file.c \
		 $(SRC_PATH)ftp_request_status.c \
		 $(SRC_PATH)ftp_listen_status.c \
		 $(SRC_PATH)ftp_get_file_header.c \
		 $(SRC_PATH)ftp_quit.c \

C_OBJS = $(C_SRCS:.c=.o)

S_OBJS = $(S_SRCS:.c=.o)

CC = gcc

FLAGS = -Wall -Wextra -Werror

HEADER = -I./inc

LIBFT = libft/libft.a

RM = rm -rf

all	: lib $(C_NAME) $(S_NAME)

lib	:
	@make -C libft/

$(C_NAME):$(C_OBJS)
	$(CC) $(FLAGS) -o $(C_NAME) $(C_OBJS) $(HEADER) $(LIBFT)

$(S_NAME):$(S_OBJS)
	$(CC) $(FLAGS) -o $(S_NAME) $(S_OBJS) $(HEADER) $(LIBFT)

%.o: %.c ./inc/ft_p.h ./inc/libft.h
	gcc $(FLAGS) -o $@ -c  $< $(HEADER)

clean	:
	@make -C libft/ clean
	$(RM) $(C_OBJS) $(S_OBJS)

fclean: clean
	@make -C libft/ fclean
	$(RM) $(C_NAME) $(S_NAME)

re	:	fclean
	make

.PHONY: all lib clean fclean re
