/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 13:33:36 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 17:47:33 by ysan-seb         ###   ########.fr       */
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

# define CMD_MAX 1024
# define CNV_MAX 64

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
void					command_quit(int client_socket);
#endif
