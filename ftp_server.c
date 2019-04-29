/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maki <maki@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 21:49:33 by ysan-seb          #+#    #+#             */
/*   Updated: 2019/04/29 20:17:48 by ysan-seb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

#define CMD_ERROR "[\e[38;5;1mERROR\e[0m] Invalid command.\n"
#define GET_OPEN_ERROR "[\e[38;5;1mERROR\e[0m] Error with open.\n"
#define GET_MMAP_ERROR "[\e[38;5;1mERROR\e[0m] Error with mmap.\n"
#define GET_SUCCESS "[\e[38;5;2mSUCCESS\e[0m] File has been send.\n"
#define FALSE "0"
#define TRUE "1"
#define BUFF_SIZE 1024

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

void	command_put(int client_socket, t_cmd cmd)
{
	printf("->\n");
	ftp_get_file(client_socket, cmd);
	printf("<-\n");
}

void	command_get(int client_socket, t_cmd cmd)
{
	ftp_send_file(client_socket, cmd);
}

int		builtins(int client_socket, t_cmd cmd)
{
	if (cmd.str[cmd.len - 1] == '\n')
		cmd.str[cmd.len - 1] = '\0';
	if (strncmp(cmd.str, "cd", 2) == 0)
		command_cd(client_socket, cmd);
	else if (strncmp(cmd.str, "ls", 2) == 0)
		command_ls(client_socket, cmd);
	else if (strcmp(cmd.str, "pwd") == 0)
		command_pwd(client_socket);
	else if (strncmp(cmd.str, "put", 3) == 0)
		command_put(client_socket, cmd);
	else if (strncmp(cmd.str, "get", 3) == 0)
		command_get(client_socket, cmd);
	else if (strcmp(cmd.str, "quit") == 0)
		command_quit(client_socket);
	else
		send(client_socket, CMD_ERROR, strlen(CMD_ERROR), 0);
	return (0);
}

int		main(int ac, char **av)
{
	int					port;
	int					sock;
	pid_t				child;
	t_cmd				cmd;
	t_client			c;

	if (ac != 2)
		usage(av[0]);
	port = atoi(av[1]);
	getcwd(cmd.home, PATH_MAX);
	if ((sock = create_server(port)) < 0)
		return (-1);
	while (1)
	{
		if ((c.cs = accept(sock, (struct sockaddr*)&c.csin, &c.cslen)) < 0)
			break ;
		if ((child = fork()) == 0)
		{
			while (1)
			{
				if ((cmd.len = recv(c.cs, cmd.str, CMD_MAX - 1, 0)) < 0)
					break ;
				cmd.str[cmd.len] = '\0';
				printf("%s", cmd.str);
				builtins(c.cs, cmd);
			}
		}
		// else
		// 	wait(&child);
	}
	close(c.cs);
	close(sock);
	return (0);
}
