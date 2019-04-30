/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 13:33:36 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/30 01:23:48 by maki             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_P_H
# define FT_P_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <netdb.h>
# include <sys/socket.h>
# include <sys/wait.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <string.h>
# include <limits.h>
# include <ctype.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/mman.h>

# define BUFF_SIZE 1024

# define CMD_MAX 1024
# define CNV_MAX 64

# define CD_INV_ARG "[\e[38;5;1mERROR\e[0m] Invalid argument.\n"
# define CD_INV_DEST "[\e[38;5;1mERROR\e[0m] Invalid destination.\n"
# define CD_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] Current directory has changed.\n"

# define LS_ERROR "[\e[38;5;1mERROR\e[0m] Invalid argument.\n"
# define LS_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] Directory has been listed.\n"

# define PWD_SUCCESS "\n[\e[38;5;2mSUCCESS\e[0m] Pwd has been executed.\n"
# define PWD_ERROR "\n[\e[38;5;1mERROR\e[0m] Can't execute pwd.\n"

# define MISSING_ARG "[\e[38;5;1mERROR\e[0m] Argument is missing.\n"
# define INVALID_PATH_FILE "[\e[38;5;1mERROR\e[0m] Invalid path file.\n"
# define OPEN_ERROR "[\e[38;5;1mERROR\e[0m] Can't open file.\n"
# define FSTAT_ERROR "[\e[38;5;1mERROR\e[0m] Error with fstat.\n"
# define IS_DIR "[\e[38;5;1mERROR\e[0m] Is a directory.\n"
# define IS_SYM "[\e[38;5;1mERROR\e[0m] Is a symlink.\n"
# define ERROR "0"
# define SUCCESS "1"
# define SENDING_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] File has been send.\n"

# define CMD_ERROR "[\e[38;5;1mERROR\e[0m] Invalid command.\n"
# define GET_OPEN_ERROR "[\e[38;5;1mERROR\e[0m] Error with open.\n"
# define GET_MMAP_ERROR "[\e[38;5;1mERROR\e[0m] Error with mmap.\n"
# define GET_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] File has been send.\n"
# define FALSE "0"
# define TRUE "1"

# define STAT_ERROR "[\e[38;5;1mERROR\e[0m] Can't stat.\n"


# define PUT_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] Command put success.\n"
typedef struct			s_client
{
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
}						t_client;

typedef struct			s_cmd
{
	char				str[CMD_MAX];
	char				home[PATH_MAX];
	int					len;
}						t_cmd;

typedef struct			s_file
{
	int					fd;
	char				*name;
	size_t				size;
	char				content[1024];
}						t_file;

void					usage(char *bin);
void					error(char *err);
int						local_command(int sock, t_cmd cmd);
void					command_ls(int client_socket, t_cmd cmd);
void					command_cd(int client_socket, t_cmd cmd);
void					command_pwd(int client_socket);
int						arg(char *cmd);
char					*filename(char *cmd);
int						ftp_request_status(int sock, char *status, int ret);
int						ftp_listen_status(int sock);
void					ftp_get_file(int sock, t_cmd cmd);
int						ftp_send_file(int sock, t_cmd cmd);
int						ftp_open_file(int sock, t_cmd cmd);
int						ftp_get_file_size(int sock);
void					ftp_get_file_content(int sock, t_file file);
size_t					ftp_send_file_size(int sock, int fd);
void					ftp_send_file_content(int sock, size_t size, int fd);
int						ftp_get_file_header(int sock);
void					command_quit(int client_socket);
#endif
