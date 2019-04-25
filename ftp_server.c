/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:49:33 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/25 23:21:16 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define CMD_MAX 1024
#define ERROR "[\e[38;5;1mERROR\e[0m]\n"
#define SUCCESS "[\e[38;5;2mSUCCESS\e[0m]\n"

typedef struct	s_cmd
{
	char		str[CMD_MAX];
	int			len;
}				t_cmd;

void	usage(char *bin)
{
	printf("Usage: %s <port>\n", bin);
	exit(-1);
}

void	error(char *err)
{
	dprintf(2, "%s", err);
	exit(-1);
}

int		create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in connection.\n");
	printf("[\e[38;5;2m+\e[0m]Server socket is created.\n");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		error("[\e[38;5;1m-\e[0m]Error in binding.\n");
	printf("[\e[38;5;2m+\e[0m]Bind to port %d.\n", port);
	if (listen(sock, 42) == 0)
		printf("Listening ...\n");
	else
		error("[\e[38;5;1m-\e[0m]Error in binding.\n");
	return (sock);
}

typedef struct			s_client
{
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
}						t_client;

void	command_pwd(int client_socket)
{
	char	path[PATH_MAX];

	getcwd(path, PATH_MAX);
	send(client_socket, path, strlen(path), 0);
}

void	command_ls(int client_socket, t_cmd cmd)
{
	int 	i;
	pid_t 	pidchild;
	char	arg[1024];

	i = 0;
	memset(&arg, 0, 64);
	while (cmd.str[i] && !isblank(cmd.str[i]))
		i++;
	while (cmd.str[i] && isblank(cmd.str[i]))
		i++;
	if (cmd.str[i] == '-')
		strncat(arg, cmd.str + i, 1024);
	else if (cmd.str[i] == '\0')
		strncat(arg, ".", 1024);
	else
	{
		send(client_socket, "?Invalid command.", 17, 0);
		return;		
	}
	if ((pidchild = fork()) == 0)
	{
		dup2(client_socket, 1);
		dup2(client_socket, 2);
		execl("/bin/ls", "ls", arg, NULL);
		exit(EXIT_SUCCESS);
	}
	else if (pidchild > 0)
		wait(&pidchild);
}

void	command_cd(int client_socket, t_cmd cmd)
{
	int 		i;
	char		path[PATH_MAX];
	char		new_path[PATH_MAX];
	static char	old_path[PATH_MAX];

	i = 0;
	if (!strlen(old_path))
		getcwd(old_path, PATH_MAX);
	if (strcmp(cmd.str, "cd") == 0)
		return ;
	while (cmd.str[i] && !isblank(cmd.str[i]))
		i++;
	while (cmd.str[i] && isblank(cmd.str[i]))
		i++;
	getcwd(path, PATH_MAX);
	chdir(cmd.str + i);
	getcwd(new_path, PATH_MAX);
	if (!strstr(new_path, path))
	{
		send(client_socket, ERROR, strlen(ERROR), 0);
		chdir(path);
		return ;
	} 
	else {
		strcpy(old_path, path);
		send(client_socket, SUCCESS, strlen(SUCCESS), 0);
	}
}

void	command_put(int client_socket, t_cmd cmd)
{
	(void)cmd;
	send(client_socket, "put", 3, 0);
}

void	command_get(int client_socket, t_cmd cmd)
{
	(void)cmd;
	send(client_socket, "get", 3, 0);
}

void	command_quit(int client_socket)
{
	send(client_socket, "quit", 4, 0);
	close(client_socket);
}

int		builtins(int client_socket, t_cmd cmd)
{
	if (cmd.str[cmd.len - 1] == '\n')
		cmd.str[cmd.len - 1] = '\0';
	if (strcmp(cmd.str, "cd") == 0 || strncmp(cmd.str, "cd ", 3) == 0)
		command_cd(client_socket, cmd);
	else if (strcmp(cmd.str, "ls") == 0 || strncmp(cmd.str, "ls ", 3) == 0)
		command_ls(client_socket, cmd);
	else if (strcmp(cmd.str, "pwd") == 0)
		command_pwd(client_socket);	
	else if (strcmp(cmd.str, "put") == 0)
		command_put(client_socket, cmd);
	else if (strcmp(cmd.str, "get") == 0)
		command_get(client_socket, cmd);	
	else if (strcmp(cmd.str, "quit") == 0)
		command_quit(client_socket);
	else
		send(client_socket, "?Invalid command.", 17, 0);
	return (0);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;
	pid_t				child;
	t_cmd				cmd;
	t_client			client;

	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	if ((sock = create_server(port)) < 0)
		return (-1);
	while (1)
	{
		if ((client.cs = accept(sock, (struct sockaddr*)&client.csin, &client.cslen)) < 0)
			break ;
		if ((child = fork()) == 0)
		{
			while (1)
			{
				if ((cmd.len = recv(client.cs, cmd.str, CMD_MAX - 1, 0)) < 0)
					break ;
				cmd.str[cmd.len] = '\0';
				printf("%s", cmd.str);
				// -> check command and send result
				builtins(client.cs, cmd);
				// if (send(client.cs, "good", 4, 0) < 0)
				// 	break ;
			}
		}
	}
	close(client.cs);
	close(sock);
	return (0);
}
